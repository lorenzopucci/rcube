/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <map>
#include <iostream>
#include <cmath>

#include <opencv2/opencv.hpp>
#include <raspicam/raspicam_cv.h>

#include <rcube.hpp>
#include <utility.hpp>

#include "colorDetection.hpp"


std::map<char, cv::Scalar> defColors = {
	{'w', {240, 240, 240}},
	{'y', {0,   230, 230}},
	{'g', {35,  205, 0}},
	{'b', {250, 140, 50}},
	{'o', {0,   130, 250}},
	{'r', {0,   0,   250}},
	{'?', {75,  75,  75}}
};

struct Square
{
	cv::Point tl, br;
	int l = 60;

	inline int area() const;
	Square translate(int x, int y) const;
};

inline int Square::area() const
{
	return (br.x - tl.x) * (br.y - tl.y);
}

Square Square::translate(int x, int y) const
{
	Square s;
	s.tl = {tl.x + x, tl.y + y};
	s.br = {br.x + x, br.y + y};
	return s;
}

int get3DdistanceSquared(const cv::Scalar &c1, const cv::Scalar &c2)
{
	return pow((c1[0] - c2[0]), 2) + pow((c1[1] - c2[1]), 2) +
		pow((c1[2] - c2[2]), 2);
}

char detectColor(const cv::Scalar &val)
{
	if (val[2] < 70 && val[1] < 70 && val[0] < 70)
		return '?';
	
	char closestCol;
	int lowestDist = 10000;
	
	for (auto it = defColors.begin(); it != defColors.end(); ++it)
	{
		int dist = get3DdistanceSquared(val, it->second);
		
		if (dist < lowestDist)
		{
			lowestDist = dist;
			closestCol = it->first;
		}
	}
	
	return closestCol;
}

cv::Scalar sampleImage(const cv::Mat &img, const Square &square)
{
	cv::Scalar val;
	
	for (int y = square.tl.y; y < square.br.y; ++y)
	{
		for (int x = square.tl.x; x < square.br.x; ++x)
		{
			cv::Vec3b pxColor = img.at<cv::Vec3b>(y, x);

			for (int i = 0; i < 3; ++i)
				val[i] += (int)pxColor[i];
		}
	}

	for (int i = 0; i < 3; ++i)
		val[i] /= square.area();

	return val;
}

void drawRectangle(cv::Mat *img, const Square &square, char col, int thickness = 2)
{
	cv::rectangle(*img, square.tl, square.br, defColors[col],
		thickness, cv::LINE_8);
}

std::map<rcube::Orientation, rcube::MixedFace> scanCube()
{
	cv::Mat output;
	raspicam::RaspiCam_Cv cap;

	cap.open();
	if (!cap.isOpened())
	{
		std::cerr << "Cannot open the camera device" << std::endl;
		return 1;
	}

	Square sCtr = {{0,0}, {0,0}};
	Square sFrt = {{0,0}, {0,0}};
	Square squares[8];

	std::pair<rcube::Orientation, rcube::MixedFace> data[] = {
		{{Axis::Y, 1}, {Color::White, {{Axis::Z, -1}, {Axis::X, 1},
        {Axis::Z, 1}, {Axis::X, -1}}}},
		{{Axis::Z, 1}, {Color::Green, {{Axis::Y, 1}, {Axis::X, 1},
        {Axis::Y, -1}, {Axis::X, -1}}}},
		{{Axis::X, 1}, {Color::Red, {{Axis::Y, 1}, {Axis::Z, -1},
        {Axis::Y, -1}, {Axis::Z, 1}}}},
		{{Axis::Z, -1}, {Color::Blue, {{Axis::Y, 1}, {Axis::X, -1},
        {Axis::Y, -1}, {Axis::X, 1}}}},
		{{Axis::X, -1}, {Color::Orange, {{Axis::Y, 1}, {Axis::Z, 1},
        {Axis::Y, -1}, {Axis::Z, -1}}}},
		{{Axis::Y, -1}, {Color::Yellow, {{Axis::X, -1}, {Axis::Z, 1},
        {Axis::X, 1}, {Axis::Z, -1}}}}
	};

	int faceIdx = 0;
	char frtCol = 'g';

	while(faceIdx < 6)
	{
		cap.grab();
		cap.retrieve(output);		
		
		if (sCtr.tl.x == 0 && sCtr.tl.y == 0)
		{
			int startX = (output.cols - sCtr.l) / 2;
			int startY = (output.rows - sCtr.l) / 2;

			sCtr.tl = {startX, startY};
			sCtr.br = {startX + sCtr.l, startY + sCtr.l};

			sFrt.tl = {startX - 80, startY + sCtr.l + 280};
			sFrt.br = {startX + sCtr.l + 80, startY + sCtr.l + 300};

			squares[0] = sCtr.translate(-200, -200); // TL
			squares[1] = sCtr.translate( 0,   -200); // TC
			squares[2] = sCtr.translate( 200, -200); // TR
			squares[3] = sCtr.translate( 200,  0);   // CR
			squares[4] = sCtr.translate( 200,  200); // BR
			squares[5] = sCtr.translate( 0,    200); // BC
			squares[6] = sCtr.translate(-200,  200); // BL
			squares[7] = sCtr.translate(-200,  0);   // CL
		}

		char cCtr = detectColor(sampleImage(output, sCtr));
		drawRectangle(&output, sCtr, (char)data[faceIdx].second.center, -1);
		drawRectangle(&output, sFrt, frtCol, -1);

		for (int i = 0; i < 8; ++i)
		{
			char col = detectColor(sampleImage(output, squares[i]));
			drawRectangle(&output, squares[i], col);
			data[faceIdx].second.stickers[i] = static_cast<Color>(col);
		}
		
		cv::imshow("Input", output);


		char c = (char)cv::waitKey(10);
		if (c == 27) break;
		if (c == 10)
		{
			if ((char)data[faceIdx].second.center != cCtr)
			{
				std::cout << "Rejecting scan: wrong face color\n";
				continue;
			}
			std::cout << "Face scanned\n";
			++faceIdx;

			for (int x = 0; x < 6; ++x)
			{
				if (data[x].first == data[faceIdx].second.adjacentFaces[2])
				{
					frtCol = (char)data[x].second.center;
				}
			}
		}
	}

	cap.release();
	cv::destroyAllWindows();
	
	std::cout << std::endl;

    std::map<rcube::Orientation, rcube::MixedFace> mapData;
	if (faceIdx != 6) return mapData;

    for (int i = 0; i < 6; ++i)
        mapData.insert(data[i]);

    return mapData;
}
