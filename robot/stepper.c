/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

/*
* My configuration:
* - D, L, F, U motors connected to two 74HC595N shift registers, with a common
* clock and latch but different SD inputs (to double the writing speed)
* - R and B motors connected directly to the GPIO ports on my Raspberry Pi
* (because I did not have a third shift register)
* Therefore the two sets of motors are controlled differently (see performMove())
*/

#include <wiringPi.h>

#include <stdio.h>
#include <stdint.h>

#include "stepper.h"


const uint16_t  CWsteps[4] = {0x01, 0x02, 0x04, 0x08};
const uint16_t  CCWsteps[4] = {0x08, 0x04, 0x02, 0x01};

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

void shiftOut (uint8_t d1Pin, uint8_t d2Pin, uint8_t cPin, uint8_t order,
    uint16_t data)
{
	if (order == MSBFIRST)
	{
		for (int i = 7; i >= 0; --i)
		{
			digitalWrite(d1Pin, data & (0x01 << i));
			digitalWrite(d2Pin, data & (0x01 << i + 8));
			
			digitalWrite(cPin, HIGH);
			delayMicroseconds(5);
			digitalWrite(cPin, LOW);
			delayMicroseconds(5);
		}
	}
	else
	{
		for (int i = 0; i < 8; ++i)
		{
			digitalWrite(d1Pin, data & (0x01 << i));
			digitalWrite(d2Pin, data & (0x01 << i + 8));
			
			digitalWrite(cPin, HIGH);
			delayMicroseconds(5);
			digitalWrite(cPin, LOW);
			delayMicroseconds(5);
		}
	}
}

void pushData(uint16_t data)
{
	shiftOut(DATA1_PIN, DATA2_PIN, CLOCK_PIN, MSBFIRST, data);

	digitalWrite(LATCH_PIN, HIGH);
	delayMicroseconds(10);
    digitalWrite(LATCH_PIN, LOW);
}

void handleStepper(char face, int step)
{
	int *pins;

	if (face == 'R')
	{
		pins = RPins;
	}
	else if (face == 'B')
	{
		pins = BPins;
	}
	else return;

	MotorCycle cycle = stepCycle(0, step/abs(step));

	for (int i = 0; i < CYCLES_90D * abs(step); ++i)
    {
        for (int x = 0; x < 4; x++)
        {
            for (int y = 0; y < 4; ++y)
            {
            	digitalWrite(pins[y], cycle.steps[x] & (0x01 << y));
            }
            delayMicroseconds(2000);
        }
    }
}

void performMove(char face, int step)
{
    int motorIdx;

    switch (face)
    {
        case 'R':
        case 'B':
        	handleStepper(face, step); break;
        	
        case 'L': motorIdx = L_MOTOR; break;
        case 'U': motorIdx = U_MOTOR; break;
        case 'D': motorIdx = D_MOTOR; break;
        case 'F': motorIdx = F_MOTOR; break;
    }

    MotorCycle cycle = stepCycle(motorIdx, step/abs(step));

    for (int i = 0; i < CYCLES_90D * abs(step) * 2; ++i)
    {
        for (int x = 0; x < 4; x++)
        {
            pushData(cycle.steps[x]);
            delayMicroseconds(2000);
        }
    }
}

void setupGPIO()
{
    wiringPiSetup();

    pinMode(DATA1_PIN, OUTPUT);
    pinMode(DATA2_PIN, OUTPUT);
    pinMode(LATCH_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);

		
	for (int i = 0; i < 4; ++i)
	{
		pinMode(RPins[i], OUTPUT);
        pinMode(BPins[i], OUTPUT);
	}
}

/*
// TEST:

int main()
{
	setupGPIO();

    performMove('R', 1);
    delay(500);
    performMove('L', -1);
    delay(500);
    performMove('U', 2);
    delay(500);
    performMove('D', 1);
    delay(500);
    performMove('F', -1);
    delay(500);
    performMove('B', 2);
}
*/
