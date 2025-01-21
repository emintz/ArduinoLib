# GPIO Change Detector Demonstration

The sketch shows how to use the `GpioChangeDetector` class. It
implements a roundabout way to blink a red LED twice per second
and a yellow LED once per second. 

## ESP32 Configuration

Wire the ESP32 as follows:

| GPIO Pin | Connection  |
| -------- | ----------- |
| 13       | Red LED     |
| 14       | Yellow LED  |
| 25       | GPIO pin 26 |
| 26       | GPIO pin 25 |

Note that the sketch also uses the ESP32 board's built-in LED,
which is presumed to be connected to pin 2. If your board's
built-in LED is connected to a different pin, change the
value of `BUILTIN_LED_PIN` to match. If your board lacks a
built-in LED, you will need to provide a substitute.

To wire the LEDs

1. Connect the ESP32 pin the positive LED lead.
2. Connect one lead of a 510 Ohm resister to ground.
3. Connect the other resister lead to the negative LED pin.

## Components

The demo provides the following components

1. `GpioChangeDemo`: the sketch whose `startup()` function assembles and
   starts the demo components and whose `loop()` function raises and lowers
   GPIO 12 (connected to the red LED) and GPIO 26 to drive processing
2. `GpioChangeHandler`: a `VoidFunction` that notifies a `TaskWithAction`
   when the voltage on GPIO pin 26 rises from `LOW` to `HIGH`. Change
   handlers run in an interrupt service routine (ISR), so it notifies
   the task by invoking `notify_from_ISR()`. Since
   GPIO 26 is wired directly to GPIO 25, its voltage rises twice per second,
   so the handler notifies its `TaskWithAction` every 500 milliseconds.
3. `GpioPinChangeAction`, a `TaskAction` that changes the level on pin 14
   (connected to the yellow LED), then waits to be notified.
   
The `GpioChangeDemo` sketch has customary three sections.

1. Static declarations
2. `setup()`
3. `loop()`

### Static Declarations

The sketch assembles the following `static` components

1. `pin_change_task`: a `TaskWithAction` that invokes the `PinChangeAction`s
   `apply()` function when it receives a notification.
2. `change_detector`: a `GpioChangeDetector` that notifies `pin_change_task`
   when the voltage on GPIO pin 26 changes from `LOW` to `HIGH`.
3. `red_level`, the curent output voltage on GPIO pin 13, which is connected
   to the red LED.
   
In addtion, it contains `halt_and_catch_fire()`: a `static` error handling
   function that prints an error message and
   hangs, effectively halting the ESP32. The sketch calls this when it
   detects an unrecoverable error.

### `setup()`

The `setup()` function

1. Configures serial output and prints a welcome message
2. Configures the GPIO pins
3. Enables GPIO change detection by starting the GPIO change detection
   service
4. Starts `pin_change_task` so that it will respond to notifications.
   Note that notification receivers should be started before
   the first notification arrives.
5. Prints a completion message

### `loop()`

The `loop()` function

1. Changes GPIO pin 13 and 25's output, which changes the red LED and the 
   voltage on pin 26. If the pins are currently `LOW`, `loop()` sets them `HIGH`
   and vice versa.
2. Waits for 500 milliseconds

## Control Flow

When the Arduino framework invokes `loop()`, `pin_change_task` is
waiting for a notification. If `loop()` raises GPIO pin 25's output
from `LOW` to `HIGH`, it triggers `change_detector` whose action
notifies `pin_change_task`, which changes the output voltage on
GPIO pin 14, changing the state of the yellow LED. The net result
is that the red LED changes state twice during the time when the
yellow LED changes once.
