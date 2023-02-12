# Bridge of Enlightenment

The bridge of enlightenment is an art project initated by [Gregor Faubel](https://www.studio-faubel.de) 
in cooperation with Julius Faubel. It is a bridge with many arcs which are illuminated when a metal ball
is passing them. The bridge infinitely rolls the ball from one side to the other. The main idea behind this project is:

<blockquote>
On the road to enlightenment one must constantly take different points of view in order to see the bigger picture.
</blockquote>

## Getting Started

This repository contains the source code of the Arduino controller. It can be compiled with 
[Arduino Studio 2.0](https://docs.arduino.cc/software/ide-v2/tutorials/getting-started/ide-v2-downloading-and-installing) 
and does not require any external libraries.

1. Checkout or download this repository into a folder named `bridge`
1. Open the `bridge.ino` file in Arduino Studio
1. Hit `CTRL + R` to compile the source code

To upload the code you must do the following:

1. Attach the Arduino board via USB
1. Select the `Arduino Uno` board in the [Arduino Studio Board Selector](https://support.arduino.cc/hc/en-us/articles/4406856349970-Select-board-and-port-in-Arduino-IDE).
1. Hit `CTRL + U`to compile and upload the code

Note: On Linux you must ensure that your user has write privileges to the USB device `/dev/usb/tty0`.

## Making Changes
The timing of the bridge movement is delicate but essential for the ball not to become to fast or to get stuck along the way. In order to make changes to the timing you can change the following lines of code:

```cpp
// Defines potentiometer values that should be moved to in the given sequence.
// When the last value was reached the list will skip the first value and repeat from the second.
int targetList[5] = { 0, 160, 50, -160, -50 };

// Defines delays in ms that should be waited after a target was reached. The
// sequence corresponds to the values in the above target list.
int targetDelay[5] = { 5000, 0, 7200, 0, 7200 };
```

### Changing timing values
The `targetList` variable contains the potentiometer values which represent the relative offset in *mm* from the levelled or zero position if the brige in each step.

The bridge moves to the first step and once the programmed value was reached will wait for the time in *ms* specified by the corresponding `targetDelay` variable.

Once this is done, we move to the next step until we reach the last target. After reaching the last target the controller returns to the second value in the list, skipping the inital zero target.

### Adding or removing value
When altering the list one must ensure that the array size of both, the `targetList[5]` and `targetDelay[5]` match the number of values and are equal.

## Notes
### Stepper Initalization Pin Order
The wiring of the used `28BYJ 48` stepper motor does not match the standard (BLUE, PINK, RED, ORANGE, YELLOW).

<img src="https://www.electrokit.com/uploads/productimage/41014/41014801.png" width="400">

Instead it is wired like this:

<img src="https://www.makerfabs.com/image/cache/makerfabs/28BYJ-48%20Stepper%20motor-5V/28BYJ-48%20Stepper%20motor-5V_3-1000x750.JPG" width="400">

This is why the Stepper motor is initialized in the following sequence:

```cpp
Stepper stepper(STEPS_PER_REVOLUTION, 9, 11, 8, 10);
```

As opposed to the sequence mentioned in most articles:

```cpp
Stepper stepper(STEPS_PER_REVOLUTION, 8, 10, 9, 11);
```

## Links

- https://community.element14.com/members-area/personalblogs/b/blog/posts/arduino-controlling-stepper-motor-28byj-48-with-accelstepper-library
- https://lastminuteengineers.com/28byj48-stepper-motor-arduino-tutorial/