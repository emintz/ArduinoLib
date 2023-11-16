# DoubleBlink, Blink Two LEDs Independently

The `DoubleBlink` sketch generalizes the
[Blink sketch](https://github.com/emintz/ArduinoLib/tree/main/RTOSAid/examples/Blinky)
by:

1. Providing a caller-configurable blink action
2. Creating and running two blink tasks, one that blinks the built in LED
   with a 500 millisecond on and off time, and a second that blinks the LED
   connected to GPIO pin 13 with a 333 millisecond on and off time.

Since 500, 2<sup>2</sup> * 5<sup>3</sup>, is relatively prime to 333,
3<sup>2</sup> * 37, implementing
this under the Arduino library's `setup()`/`loop()` software architecture
poses considerable challenge.

