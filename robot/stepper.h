/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <stdint.h>

#define DATA1_PIN 0
#define DATA2_PIN 7
#define LATCH_PIN 2
#define CLOCK_PIN 3

const int RPins[4] = {6, 5, 4, 1};
const int BPins[4] = {21, 22, 23, 25};

#define L_MOTOR 0
#define D_MOTOR 1
#define U_MOTOR 2
#define F_MOTOR 3

#define CYCLES_90D 128

#define CW_DIR 1
#define CCW_DIR -1

#define MSBFIRST 1
#define MSBLAST  2


typedef struct MotorCycle
{
	uint32_t steps[4];
} MotorCycle;


void setupGPIO();

void performMove(char face, int step);
