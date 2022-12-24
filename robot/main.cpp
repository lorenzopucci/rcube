/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <iostream>
#include <map>

#include <rcube.hpp>
#include <utility.hpp>

#include "../ui/ui.hpp"
#include "colorDetection.hpp"

#include <opencv2/opencv.hpp>
#include <raspicam/raspicam_cv.h>


int main(int argc, char** argv)
{
	std::map<rcube::Orientation, rcube::MixedFace> mapData = scanCube();		
	rcubeUI::runUI(new rcube::Cube(mapData));
}