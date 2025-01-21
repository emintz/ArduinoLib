# Microsecond Timer Demonstration

## Overview

The Microsecond Timer sketch demonstrates timer
use. It employs the entire public API: construction, `begin()`,
`start()`, and `stop()`. It also gives an example
`VoidFunction`, providing subclass that raises a specified
GPIO pin to illuminate an LED.

## Sketch Logic

The sketch defines four key static variables:

1.  `builtin_led_timer`: a `MicrosecondTimer` that illuminates the
    builtin LED when it fires, and is used to demonstrate
    straightforward timer use
2.  `red_led_timer`: a `MicrosecondTimer` that illumintes the
    red LED when it fires and is used to demonstrate stopping
    a timer
3.  `yellow_led_timer`: a `MicrosecondTimer` that illuminates the
    yellow LED when it fires and is used to demonstrate resetting
    a timer
4.  `turn_red_led_on`: a `bool` flag whose value determines
    if `red_led_timer` should run to completion. If
    `turn_red_led_on` is `false`, the sketch stops the
    red LED timer 250 milliseconds after it starts, so the
    timer never fires and the red LED does not illuminate.
    If `turn_red_led_on` is `true`, the sketch lets the
    timer run to completion so it illuminates the red LED
    when it fires.

The `loop()` function

1.  Turns all LEDs off
2.  Starts all three timers with 500000 microsecond (.5 second)
    delays
3.  Waits 250 milliseconds
4.  Negates `turn_red_led_on` and stops the red LED timer if
    the result is `false`.
5.  Restarts the yellow LED timer with a 500000 microsecond
    (.5 second) delay
6.  Waits 750 millisecods.

## Expected Behavior

* The built in LED blinks once per second, 500 milliseconds on,
  500 milliseconds off.
* The red LED blinks once every other iteration.
* The yellow LED blinks once per second, 250 milliseconds
  on, 750 milliseconds off

## ESP32 Pin Use and Wiring

The sketch uses the following LEDs:

1. Builtin, the LED on the ESP32 development board. The sketch
   is configured to use pin 2. If your board uses a different pin,
   change the value of `BUILTIN_LED_PIN` in `TimerDemo.ino`.
2. Red, connected to pin 13
3. Yellow, connected to pin 14

LED anodes (a.k.a. the side with the longer lead, sometimes called the positive side)
must connect directly to its GPIO pin. The cathode (the side with the shorter lead) must
be connected to ground via a 510 Ohm resistor.

Connecting the LED is connected backwards will not harm it. It will simply remain dark.
The LED will illuminate once you reverse it.

Note that you *must* connect interpose a 510 Ohm resistor between the LED's cathode
and ground. Connecting the cathode directly to ground will destroy it.

