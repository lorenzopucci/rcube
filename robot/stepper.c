/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <wiringPi.h>

#include <stdio.h>
#include <stdint.h>

#include "stepper.h"


const uint16_t  CCWsteps[4] = {0x01, 0x02, 0x04, 0x08};
const uint16_t  CWsteps[4] = {0x08, 0x04, 0x02, 0x01};

MotorCycle stepCycle(int motorIdx, int direction)
{
    MotorCycle data;

    for (int i = 0; i < 4; ++i)
    {
        if (direction == CW_DIR)
        {
            data.steps[i] = CWsteps[i] << (motorIdx * 4);
        }

        if (direction == CCW_DIR)
        {
            data.steps[i] = CCWsteps[i] << (motorIdx * 4);
        }
    }

    return data;
}

void handleStepper(const int* pins, int step)
{
	MotorCycle cycle = stepCycle(0, step/abs(step));

	for (int i = 0; i < CYCLES_90D * abs(step) + 15; ++i)
    {
        for (int x = 0; x < 4; x++)
        {
            for (int y = 0; y < 4; ++y)
            {
            	digitalWrite(pins[y], cycle.steps[x] & (0x01 << y));
            }
            delayMicroseconds(2500);
        }
    }
    
	MotorCycle invCycle = stepCycle(0, -step/abs(step));
    
    for (int i = 0; i < 15; ++i)
    {
        for (int x = 0; x < 4; x++)
        {
            for (int y = 0; y < 4; ++y)
            {
            	digitalWrite(pins[y], invCycle.steps[x] & (0x01 << y));
            }
            delayMicroseconds(2500);
        }
    }
}

void performMove(char face, int step)
{
    switch (face)
    {
        case 'R':
        	handleStepper(RPins, -step); break;
        case 'B':
        	handleStepper(BPins, step); break;
        case 'D':
        	handleStepper(DPins, step); break;
        case 'L':
            handleStepper(LPins, step); break;
        case 'F':
            handleStepper(FPins, step); break;
        case 'U':
            handleStepper(UPins, step); break;
    }
}

void setupGPIO()
{
    wiringPiSetup();
		
	for (int i = 0; i < 4; ++i)
	{
		pinMode(RPins[i], OUTPUT);
        pinMode(BPins[i], OUTPUT);
        pinMode(DPins[i], OUTPUT);
		pinMode(LPins[i], OUTPUT);
		pinMode(FPins[i], OUTPUT);
		pinMode(UPins[i], OUTPUT);
	}
}
