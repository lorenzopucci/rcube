/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <iostream>
#include <map>
#include <thread>
#include <queue>

#include <rcube.hpp>
#include <utility.hpp>

#include "../ui/ui.hpp"
#include "colorDetection.hpp"


extern "C"
{
	#include "stepper.h"
}


void robotWorker(std::queue<rcube::Move> *moveQueue)
{
	using namespace std::literals::chrono_literals;
	rcube::Algorithm rotations;

	while (true)
	{
		if (moveQueue->empty())
		{
			std::this_thread::sleep_for(200ms);
			continue;
		}

		rcube::Move move = moveQueue->front();

		if (move.direction == 0) return;
		
		switch (move.face)
		{
			case ROTATE_X:
			case ROTATE_Y:
			case ROTATE_Z:
				rotations.push(move); break;
			
			default:
			{
				if (rotations.length() > 0)
				{
					rcube::Algorithm newAlgo = rotations;
					newAlgo.push(move);
					newAlgo.removeRotations();
					move = newAlgo.algorithm[0];
				}

				performMove((char)move.face, move.direction);
			}
		}
		moveQueue->pop();
	}
}

struct MoveHandler
{
	std::queue<rcube::Move> *queue;
	void onMove(rcube::Move);
};

void MoveHandler::onMove(rcube::Move move)
{
	queue->push(move);
}

int main(int argc, char** argv)
{
	std::map<rcube::Orientation, rcube::MixedFace> mapData = scanCube();

	setupGPIO();
	
	std::queue<rcube::Move> *moveQueue = new std::queue<rcube::Move>;
	std::thread robotThread(robotWorker, moveQueue);

	MoveHandler handler {moveQueue};
	auto callback = std::bind(&MoveHandler::onMove, &handler,
		std::placeholders::_1);
	
	rcubeUI::runUI(new rcube::Cube(mapData), callback);

	robotThread.join();
}