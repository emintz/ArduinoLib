# Blinky, a Simple FreeRTOS Task-Based Blink Sketch

Blinky is the FreeRTOS task-based equivalent to the Arduino
[blink example](https://github.com/esp8266/Arduino/blob/master/libraries/esp8266/examples/Blink/Blink.ino)
sketch. It has two components:

1. The `BlinkAction` class, which provides the code that blinks the built-in LED
2. The `Blinky` sketch, which creates and starts the FreeRTOS task in which
   `BlinkAction` runs.
 
The sketch is written to run on an
[AITRIP 38 Pin ESP-32S Development Board](https://www.amazon.com/AITRIP-ESP-WROOM-32-Development-Microcontroller-Integrated/dp/B0B12822SF?pd_rd_w=Cx1AN&th=1)
which drives its built in LED with pin 2. Please change the pin number to fit your
board.

## The BlinkAction Class

The `BlinkAction` class inherits from the library's `TaskAction` class
and overrides and implements the latter's `run()` function. The implementation

1. Sets the built in LED pin to `OUTPUT`.
2. Loops indefinitely, turning the LED on and off, waiting 500 milliseconds
   between changes.

## The Blinky Sketch

The Blinky sketch, which creates and runs the task that blinks the LED,
has three sections:

1. Static variable declarations
2. Initialization
3. Sketch logic

### Static Variable Declarations

The sketch declares the following variables.

* `blink_action_stack`, 1024 bytes of raw, untyped memory for the blink tasks's
   stack.
* `blink_action`, an instance of the `BlinkykAction`, a sketch-specific 
  `TaskAction` that blinks the LED
* `blink_task`, a `TaskWithAction` instance that runs the logic implmeented in `blink_action`

### Initialization

As always, the sketch's `setup()` function initializes the sketch. It prints a
greeting, starts `blink_action`, prints a status message, then returns.

### Sketch Logic

Once the `blink_action` task starts, the sketch has nothing left to do.
Instead of loading the CPU by running unnecessary `loop()` iterations,
the loop waits for the longest allowable time, `0xFFFFFFFF` or 4,294,967,295
milliseconds, just shy of 50 days.

## Using the Sketch

If your ESP32 board does not provide a built-in LED, wire an LED to `BUILTIN_LED_PIN`.
Attach the positive LED lead to the pin, and the negative lead to a 510 Ohm resister that
is wired to ground.

The sketch is configured to toggle pin 2. If you need to toggle another pin, change
the `#define` of `BUILTIN_LED_PIN` to match.

Compile and upload the sketch.

