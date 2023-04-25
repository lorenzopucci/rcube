# Robot solver
This is the code I used to build a Rubik's cube solver robot with 6 stepper
motors and a Raspberry Pi.

![Demo](media/demo.gif)

Currently what the code does is:

- Scanning the cube's faces with a Raspberry Pi camera and OpenCV
- Opening a GUI window with the current status of the cube. All the moves
entered with the keyboard will be forwarded to the motors (including `ctrl+D`
to solve the cube).

## Compiling
Before compiling the robot part, the `lib` and `ui` targets of `../Makefile`
must have been met. Then, move to this folder and run `make robot`. The
results will be in `./bin`. To run the program, use:
```sh
cd ..
LIBGL_ALWAYS_SOFTWARE=true robot/bin/robot.o
```