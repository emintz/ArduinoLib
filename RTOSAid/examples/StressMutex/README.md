# Mutex Stress Test

Stress test of the mutual exclusion semaphore (mutex) that
attempts to create and detect a race condition. The test
succeeds if a deadlock does not occur.

In spite of egregiously violating good practice, the test
does show how to use the `Mutex` and `MutexLock` classes.

## Test Components

The test contains the following components:

1. `BlinkTask`, a `TaskAction` that blinks the built in LED
    once per second. The task runs at priority 1, the lowest non-idle priority,
    and blinks the built in LED to signal that the test is live. If the blink
    becomes erratic or stops, the test is failing.
2. `TargetClass`, whose `have_a_go()` method contains critical code that is vulnerable
    to race conditions
3.  `AssailantAction`, a `TaskAction` that continuously invokes
   `TargetClass::have_a_go()`. To provoke a race condition, the sketch runs two
    tasks that run this action. See below for details.
4.  `StressMutex.ino`, the sketch that assembles and runs the test.

# The `TargetClass`

## Fields

`TargetClass` contains the following fields.

| Name    | Contents                                                         |
| ------- | ---------------------------------------------------------------- |
| `count` | The invocation count that `have_a_go()` increments               |
| `mutex` | The `Mutex` that serializes the critical code in `have_a_go()`   |

## Core Logic -- The Race Condition

The `TargetTask` and its `have_a_go()` method design deliberately implements a
big, fat race condition vulnerability. If the `have_a_go()` logic were not
protected by a `Mutex`, it would certainly exhibit flaky, non-deterministic
behavior.


:warning: **Warning**: to ensure that it contains a potential race condition,
`have_a_go()` holds the semaphore for far too long. **DO NOT** do this in
your application.

## Maximizing Volatility

To start, the `TargetTask` `count` field is declared `volatile`, meaning
that external forces like other tasks or I/O operations can its value can
change its value without warning. Marking the field `volitale` forbids the
code from taking shortcuts that could ignore a change.

## Global State Mutation

The critical code, that is the logic containing the potential race condition,
does the following.


1.  Sets the `result` parameter's `entry_count` to the current value of `count`.
    Because `count` is declared `volatile`, the compiler will not optimize field
    access, and the code will move the latest `count` value to `result->entry_count`.
2.  Invokes `vTaskDelay()` to suspend execution for a caller-specified duration,
    which allows ready task, including the idle task, to run. The highest priority
    ready task will run first.
3.  Increment `count`, which, if access to the critical code is unrestricted, could
    have been incremented, and move its new value to the `result` parameter's
    `exit_count` field. Because `count` is declared `volitile` and its access is not
    optimized, the logic is guaranteed to increment the field's current value.

When a race condition occurs, another task will increment `count` field's value while
this task is suspended, so the `result` parameter's `exit_count` will
be greater than its `entry_count` +  1.

# `AssailantAction` Class

## Fields

The `AssailantAction` class, which implements a `TaskAction`, contains the following fields.

| Name      | Contents                                                  |
| --------- | --------------------------------------------------------- |
| `target`  | The global `TargetClass` instance                         |
| `delay`   | The delay for `TargetClass::have_a_go()`, in milliseconds |
| `led_pin` | The pin connected to the action's indicator LED           |

## Core Logic -- the Assault

The `AssailantAction` run loop

1.  Illuminates the indicator LED specified by `led_pin`
2.  Invokes the target's `have_a_go()` function passing the delay set during
    construction and a local `RaceConditionStruct`
3.  Extinguishes the indicator LED
4.  Verifies that the `exit_count` field in the passed `RaceConditionStruct` 
    equals its `entry_count` field plus one. If not, either a race condition
    occurred or the semaphore failed to lock. In either case, it illuminates
    the red LED.

The red LED is never extinghished, so it remains illuminated when a race
condition occurrs.

# `StressMutex.ino`, the Main Sketch

`StressMutex.ino` configures and starts the stress test in `setup()`. Once the test starts,
the sketch has nothing else to do, so its `loop()` method waits for the longest
allowable time and waits again on expiration.

## Static Variables

The static variables create the following components:

1.  A blink task, a background task that blinks the builtin LED once per second to
    indicate that the test is live.
2.  A `TargetClass` that contains the `Mutex` to test and the critical code that it protects.
3.  Two `TaskWithAction` instances bound to `AssailantAction`. These tasks exercise the
    global `TargetClass` and, if the target's critical code were not protected by a
    mutex, would  exhibit flaky behavior. One assailant runs at low priority and displays activity on
    the test system's yellow LED. The other runs at high priority and displays activity
    on the test system's green LED.

## Logic

The sketch starts the test and goes to sleep. `setup()` initializes all components in proper order, and `loop()` suspends itself for the longest allowable time.

# Running the Test

## Assemble The Test System

You'll need: 

* An ESP32 develpment environment, a computer, an IDE, and the proper USB cable.
* An ESP32 development board
* 1 each red, yellow, green, and blue LED
* 4 510 5% (green, brown, brown, gold) 1/4 watt resistors
* Hookup wire or Dupont cables
* A solderless breadboard or equivalent.

Wire the positive (long) LED leads as follow

| LED color | GPIO (Pin No.) |
| --------- | -------------- |
| Red       | 13             |
| Yellow    | 14             |
| Green     | 15             |
| Blue      | 16             |

Connect a resistor from the LED's negative (short) lead to ground.

## Build and Run the Test

Install your ESP32 IDE of choice if you haven't already done so. There are many
fine ones online, free for the taking. If you're looking for one, you might
want to consider
[Sloeber](https://eclipse.baeyens.it/).
I'm fond of it in spite of its many quirks.
[_De gustibus non est disputandum_](https://en.wikipedia.org/wiki/De_gustibus_non_est_disputandum)
so please use what suits you.

Install the RTOSAid Library. At the time of this writing, you'll
need to download it manually from
[GitHub](https://github.com/emintz/ArduinoLib/tree/main/RTOSAid).
We hope to promote it to an official Arduino library soon, but cannot
yet give a date.

Download the test from the respository's example folder. Make sure that
the BUILTIN_LED_PIN, currently set to 2, is correct for your development
board. Change its value if need be. When all is in readyness, build
and upload the sketch. The test starts with a lamp test, during which
all LEDs should illuminate for one second. When the lamp test finishes,
the builtin LED should blink once per second to indicate that the test
is live, and
the green and yellow LEDs should illuminate, to indicate that the
assailants are attacking their target. All the other LEDs should
go dark. In particular, the red led, which illuminates on failure, should
remain off.


Walk away for a few hours. Relax. Seriously, for the test to work properly,
you need to pamper yourself a bit. Go for
a walk or for coffee if you're so inclined, visit a friend,
play with the cat, do whatever you enjoy that turns your mind away from computing.
Come back when you feel completely restored and check the LEDs. If red is off and
yellow and green are lit, the test has passed.

If the test passes, take a victory lap. If it fails, please let us know.

Enjoy!



