# Robot solver
This is the code I used to build a Rubik's cube solver robot with 6 stepper
motors and a Raspberry Pi.

## Setup
- Initially, a Raspberry Pi Camera using OpenCV scans the faces of the cube.
- The motors controlling the D, L, F, U faces are connected to two 74HC595N
shift registers, with a common clock and latch but different SD inputs (to
double the writing speed).
- The R and B motors are connected directly to the GPIO ports on my Raspberry
Pi (because I did not have a third shift register).

## Compiling
Before compiling the robot part, the `lib` and `ui` targets of `../Makefile`
must have been met. Then, move to this folder and run `make robot`. The
results will be in `./bin`. To run the program, use:
```sh
cd ..
LIBGL_ALWAYS_SOFTWARE=true robot/bin/robot.o
```