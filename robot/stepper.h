/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <stdint.h>

const int RPins[4] = {8, 9, 7, 0};
const int BPins[4] = {15, 16, 1, 4};
const int DPins[4] = {2, 3, 12, 13};
const int LPins[4] = {5, 6, 10, 11};
const int FPins[4] = {14, 30, 21, 22};
const int UPins[4] = {24, 25, 28, 29};


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
