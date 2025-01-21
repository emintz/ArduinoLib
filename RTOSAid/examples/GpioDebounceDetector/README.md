# The Switch Bounce Detector

The Switch Bounce Detector sketch counts switch opens and
closures including bounces, if any.

## Switch Bounce

The contacts of a mechanical switch vibrate when they change so what
we consider a single change -- open to closed or closed to open --
actually generates a sequence of opens and closing that resembles the
following switch closure.

```
 Open   --> -------+  +---+      +--------+
                   |  |   |      |        |
                   |  |   |      |        |
 Closed -->        +--+   +------+        +----------------------- ...

                   |<---- Bounce Time --->|
```
Note that the sketch does not debounce the switch signal.

## Test Platform

The test platform is an ESP32 wired as follows.

| GPIO Pin | Connected to                                              |
| -------- | --------------------------------------------------------- |
| 13       | Red LED with a 510 Ohm current limiting resistor          |
| 14       | Yellow LED with a 510 Ohm current limiting resistor       |
| 27       | Switch to ground                                          |

Pin 27 has a pullup resistor so its voltage is HIGH when the switch is
open and LOW when the switch is closed. This configuration is called
"active low".


## Principles of Operation

The sketch counts switch makes (closures) and breaks (opens) and prints them
every 15 seconds. It also

* Toggles (turns off if on and on if off) the yellow LED when it prints the
  count.
* Sets the voltage on pin 13 to the voltage on pin 27 so the red LED turns
  on when the switch is open and off when the switch is closed.

## Using the Sketch

1. Wire the ESP32 board as described above.
2. Compile and upload the sketch.
3. Wait for the sketch to print the initial count, which should be 0.
4. Repeat the following as often as desired:
   1. Close the switch and wait for the count to print.
   2. Open the switch and wait for the action to print.

When the switch bounces, the count increase will be greater than one.
