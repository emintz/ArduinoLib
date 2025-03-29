# The RTOSAid Library

Welcome to the RTOSAid library, a collection of C++ convenience classes
that simplify the use of advanced FreeRTOS features for ESP32
developers.

## Licence

RTOSAid is open source software, free as in beer and as in speech. It has
been released under the 
[Affero General Public License](https://www.gnu.org/licenses/agpl-3.0.html)
whose text resides
[here](https://www.gnu.org/licenses/agpl-3.0.txt).

# To the Reader

Welcome to the RTOSAid library, the classes that I wish I had
when I started to develop ESP32 software. I hope that it
simplifies your development and gives good service.

At the time of this writing, late 2023, the software is new,
incomplete, and probably contains defects. I would
be grateful if you would draw my attention to either.

In the meantime, enjoy!

## Intended Use

RTOSAid is intended for use in open source ESP32 software projects that
are based on the Arduino runtime, especially for event driveen projects
that have outgrown the Arduino's single task `setup()`/`loop()`
architecture.

## What You Need To Know

Users should be comfortable programming C++ and have a
working understanding of interrupts, timers, and other low
level hardware features. Familiarity with
the ESP32 FreeRTOS APIs is helpful, but not required.

Since the library is (meant to be, at least) self-documenting, it
features long class, function, and variable names. We hope that 
doesn't overly inconvenience you.

### Arduino-Related Prerequisites

Users must have an Arduino-compatible software development environment
that supports the ESP32. The Arduino IDE is an obvious candidate because it is
small, simple, easy to learn, and ideal for small projects. Unfortunately,
it scales poorly as projects grow.
If your project contains more than a half dozen files, you might want to
consider upgrading to [Sloeber](https://eclipse.baeyens.it/)
or another more powerful IDE.

### Logic Design-Related Prerequisites

Users should be comfortable using

* Breadboards
* LEDs and current limiting resistors
* Pullup resistors
* Active low logic
* ESP32 GPIO pins

Since the documentation does not include diagrams,
users need to be able to assemble simple circuits
from their descriptions.

### C++-Related Prerequisites

Users should understand and be comfortable using

* C++ classes -- constructors, destructors, functions, and fields
* Virtual and non-virtual functions
* Virtual destructors
* Pure virtual functions
* Class and function templates
* Enumerations, both `enum` and `enum class`
* Storage classes: static, automatic, and dynamic
* Volatile fields

Explanation of C++ and its many features requires hundreds of pages,
and is beyond the scope of this document. In view of the many first-rate
C++-related resources already available, any such effort would be
redundant. The library does, however, provide extensive examples which we hope
you find informative. 

### Multitasking on the ESP32

Even though RTOSAid hides low-level complexities, it would be helpful to
have a basic understanding of:

* Tasks
* Semaphores
* Queues

These are core features that support multi-tasking, the ability to
perform, or at least appear to perform, many operations concurrently.

### Low Level ESP32 Features

#### Background

Even though RTOSAid abstracts technical details away, it would be helpful to
be familiar with:

* Interrupts
* Timers
* EEPROM memory

#### Interrupts

Since some RTOSAid classes use interrupts, a brief explanation is in order.

Interrupts are a hardware feature that support efficient event handling,
especially signals from peripheral devices. A
[real-time clock](https://www.analog.com/media/en/technical-documentation/data-sheets/ds3231.pdf),
for example, can notify a microcontroller every second with extremely high
accuracy. Other sensors, such as the
[BNO055 Gyroscope](https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bno055-ds000.pdf),
can notify their microcontroller when they have data
available.

While a program can check if a BNO055 has data, doing this in
software has significant disadvantages.

* The checks occupy the CPU. If the application has to check
  for input continuously, the resulting CPU load can make the
  application run unacceptably slowly.
* It is possible for the application to miss available data.

Interrupts provide a way to minimizing CPU involvement by
offloading event detection to hardware, which

* Frees the CPU to do other work
* Minimizes response time
* Minimizes the possibility of missing events

To use interrupts,

1. Write an interrupt service routing (ISR) that responds
   to the expected event. ISRs need to complete quickly,
   so they usually notify an event handler and return.
   See below for details.
2. Implement an event handler.
3. Enable the appropriate interrupt during application startup.

The library provides examples.

ISR implementations must

* Have no parameters
* Return nothing
* Complete quickly, the quicker the better. ISRs block their
  containing applications.
* **Never ever block**
* Invoke **only** ISR-compatible API methods. Note that most widely used
  library capabilities are **not** usable from ISRs.

Unless otherwise noted, RTOSAid functions **cannot** be invoked from ISRs.
Interrupt-compatible function names usually end with `from_isr`, as in the
[`TaskWithAction`'s](#taskwithaction-class)
[`notify_from_isr()`](#notify_from_isr) function.

Pro tips:

* Respond to interrupts in application code wherever possible. The
  ideal ISR notifies the task which responds to the interrupt and returns.
* Do no I/O in ISRs. Using `Serial`, for example, is right out.
  [SPI](https://www.arduino.cc/reference/en/language/functions/communication/spi/),
  [Wire](https://www.arduino.cc/reference/en/language/functions/communication/wire/),
  WiFi, and other external communication are also out of bounds, as are
  the Advanced I/O Arduino functions.
* **Do not block, ever.** `vTaskDelay()`, `delay()`, and `delayMicroseconds()`
  and the like are incompatible with interrupt handling. Don't even think about
  invoking them.
* Consider encapsulating each interrupt handler in its own process.
  That way, the handler can wait for notifications, and all the ISR
  needs to do is invoke the task's
  [notify_from_isr()](https://github.com/emintz/ArduinoLib/tree/main/RTOSAid#notify_from_isr)
  method.

Interrupt service routines **must** reside in interrupt-accessible memory.
The ESP32 chip contains several memory types, including static RAM (SRAM),
the fastest available. Unfortunately, SRAM consumes a lot of chip
area, so the ESP32 contains only a modest amount. Sunce ISRs **must**
reside in SRAM (unless otherwise noted), we must use it carefully.

The `IRAM_ATTR` attribute, defined in `Arduino.h`, forces a function into
SRAM. It applies to free standing functions and to functions in classes.
Here's an how to place a free-standing function in SRAM.

```
#include "Arduino.h"

// ...

void IRAM_ATTR isr_handler(void) {
  // ...
}
```

In addition to being placed in SRAM, ISRs in classes must be declared `static`.

```
#include "Arduino.h"

class ShortLongRedGreenEventHandler {

  // Fields and other stuff

  // Interrupt service routines can be private, if desired.
  static void IRAM_ATTR take_interrupt(void);

  // ...
  }
};
```

and implemented 

```
#include "ShortLongRedGreenEventHandler"

void IRAM_ATTR ShortLongRedGreenEventHandler::take_interrupt(void) {
  // ...
}
```


### Hardware-Related Prerequisites

Users should also have a set of basic electronic tools and be able to
wire simple circuits from their descriptions.

#### Bill of Materials

Users should have the following parts on hand.

* 1 ESP32 development (a.k.a. breakout) board. Mine is an inexpensive
  generic ESP32 S2.
* 1 solderless breadboard
* Hookup wire or Dupont cables. 24 guage solid wire works well in solderless
  breadboards.
* LEDs in assorted colors, 1 each red, yellow, green, blue, and white
* 5 510 Ohm , 5 percent (green, brown, black, gold), 1/4 Watt resistors for
  LED current limiting
* 1 push button switch or equivalent

#### Tools

Users should have some basic electrical tools.

* A basic, light duty 
  [wire cutter] (https://www.amazon.com/DOWELL-Cutter-Professional-Cutting-Aluminum/dp/B077RTNXVP/ref=sr_1_75?c=undefined&th=1).
* A wire cutter, for example, this 
  [basic model](https://www.amazon.com/Jameco-Benchpro-JE-2022C-R-Cutter-Stripper/dp/B00B889B62/ref=sr_1_11?).
* A needle nose (_not_ long nose) pliers such as
  [this](https://www.amazon.com/Dykes-Needle-Pliers-Extra-6-Inch/dp/B0733NWRCS/ref=sr_1_9?)
  basic model -- not absolutely required, but extremely handy in a pinch.

:arrow_forward: **Note**: the examples are for illustration only, and do not
constitute endorsements. Tool philosophies and preferences are extremely
personal. Please use what suits you.

#### Wiring

The test system is wired as follows. LEDs and their current limiting resistors
are wired  by connecting

1. A GPIO pin to a lead of a 510 Ohm resistor
2. The other resistor lead to the long (positive) LED lead
3. The negative  (short) LED lead to ground

Connect the pins as follows:

| GPIO Pin | Connect to                   |
| -------- | ---------------------------- |
| 13       | Red LED                      |
| 14       | Yellow LED                   |
| 15       | Green LED                    |
| 16       | Blue LED                     |
| 23       | White LED                    |
| 25       | Pin 26                       |
| 26       | Pin 25                       |
| 27       | Push button switch to ground |

In addition to the foregoing, the example sketches use the development
board's built in LED, which is connected to GPIO pin 2 on my preferred
cheap and cheerful
[breakout board](https://www.amazon.com/AITRIP-ESP-WROOM-32-Development-Microcontroller-Integrated/dp/B09DPH1KXF/ref=sr_1_1_sspa?th=1).
If your board's built in LED is wired differently, change the sketch
accordingly. If your board does not have a builtin LED, you will
need to add an extra LED to serve in its place and change the example sketches as needed.

## RTOSAid Features

RTOSAid supports the following FreeRTOS features.

* Tasks, the basic unit of execution under FreeRTOS. 
* Mutual Exclusion Semaphores, a.k.a. "Mutexes", used to prevent tasks from
  interfering with each other.
* FIFO queues, which carry messages between tasks
* High resolution timers, which run specified logic after a specified
  delay
* GPIO change detectors, which run specified logic when the voltage on
  an input GPIO changea

# Design Philosophy

Like most microcontrollers, the ESP32 runs slower and has far less memory than
regular computers. It cannot support many commonly used libraries and
frameworks. Instead, they support low level lightweight support like the Arduino APIs
or, in the ESP32's case,
[FreeRTOS](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html).

In addition, real time embedded software is subject to stringent
requirements.

* Operation: production ESP32 software must run indefinitely without
  uogrades or user intervention.
* Criticality: failing embedded is far more likely to
  threaten life and property than failing application software.
* Resonsiveness: embedded systems must respond within strict
  deadlines. Prompt response can be vital to prevent damage, ensure user safety,
  and the foundation of a good UX.
* Expected service life: embedded software can remain in service for
  decades without support or upgrade.

To comply with the foregoing requirements, RTOSAid

* Supports minimal heap use by providing classes that allocate
  storage statically instead of via `new` or `malloc`. Users must provide
  storage to some of these classes.
* Supports efficient heap use by providing classes that
  automatically manage storage via ```new``` and ```delete```.
* Short functions: logic is short and simple. Most functions occupy
  a single printed page.
* Does not recurse to minimize the chance of stack overflow
* Each class does one thing well: RTOSAid classes have few,
  if any options. For example, the RTOSAid messasge queue is FIFO
  even though the FreeRTOS also supports
  LIFO. If LIFO or deques are needed, we will add classes that support
  them.
* Type safety: APIs are designed to minimize the risk of type errors,
  to ensure that functions only receive values of expected types. The
  library provides templates where appropriate.

Note that classes that allocate heap storage have names that end in `H`.
For example, the static memory-oriented task class is named `TaskWithAction`
and its heap-oriented counterpart is named `TaskWithActionH`. See
the `TaskWithAction` and `TaskWithActionH` documentation for more
detail.

In the author's experience, "do everything" classes
limit a library's usefulness. Designers cannot anticipate their
users' future needs, so over time, large, multifunction classes lose
their usefulness. Hence, each class in the RTOSAid library performs
one and only one a basic function. The classes are designed for
composition, so users can mix and match them as use cases require.

In addition, library classes are either abstract or final. Abstract
classes cannot be used directly, and can only serve as the parent class
of a subtype. Final classes, in contrast, cannot be subtyped and
can only be used directly. Concrete supertypes, classes that can be
used directly and that also can be subtyped, can complicate
maintenance and embrittle the library over time. 

## Documentation Conventions

The documentation provides the following information for each class:

1. Overview, a description of the class, what it does, and why it matters
2. Background, why the class is present in the library, including code
   examples where appropriate
3. Detailed explanation of each user-visible function, including
   1. Overview, what the function does and when to use it.
   2. Parameter types and contents
   3. Return value, where appropriate
   4. Example of use, where appropriate. We elide examples that provide no
      useful information

Documentation glosses over implementation details like private fields
and methods, especially when coverage would add little relevant information.
To make up for this, the code is extensively commented.

# Tasks

The task is the basic execution unit on the ESP32 and the central RTOSAid
class. FreeRTOS is _multitasking_, meaning that many tasks can appear to run
simultaneously. In reality, tasks have priorities, and RTOS runs the highest
priority available task. Since lower priority tasks run when higher priority
tasks are blocked, tasks in a well designed ensemble appear to run
simultaneously.

Since Arduino style, `setup()`/`loop()`-based sketches, run in
a task, your existing sketches already use them. 

## Overview

Let's start sidling up to tasks by considering a sketch that
blinks the built in LED once per second, 500 milliseconds on,
500 milliseconds off.

```c++
#include "Arduino.h"

#define BUILTIN_LED 2

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);
  Serial.println("Blinky initialized.");
}

void loop() {
  digitalWrite(BUILTIN_LED, HIGH);
  vTaskDelay(pdMS_TO_TICKS(500));
  digitalWrite(BUILTIN_LED, LOW);
  vTaskDelay(pdMS_TO_TICKS(500));
}
```

This is the embedded counterpart to "Hello, World", a
short, simple, easily understood program that produces
simple output.

Now let's blink two LEDs, the builtin as above, and another at
twice per second. One straightforward design splits the 500
millisecond delays into two 250 millisecond delays and interleaves
code that blinks the LEDs.

```c++
#include "Arduino.h"

#define BUILTIN_LED 2
#define RED_LED 13

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  Serial.println("Blinky initialized.");
}

void loop() {
  digitalWrite(BUILTIN_LED, HIGH);
  digitalWrite(RED_LED, HIGH);
  vTaskDelay(pdMS_TO_TICKS(250));
  
  digitalWrite(RED_LED, LOW);
  vTaskDelay(pdMS_TO_TICKS(250));
  
  digitalWrite(BUILTIN_LED, LOW);
  digitalWrite(RED_LED, HIGH);
  vTaskDelay(pdMS_TO_TICKS(250));
  
  digitalWrite(RED_LED, LOW);
  vTaskDelay(pdMS_TO_TICKS(250));
}
```

Now let's kick it up a notch. Instead of toggling the second LED
every 250 milliseconds, toggling it every 333 milliseconds.
This is far more challenging because, unlike 500 and 250,
333 (3^2 * 37)  and 500, (2^2 * 5^2) are relatively prime
This means that simple design shown above cannot work.
The resulting sketch is too complex to show here so we've placed it in
[Appendix I](#appendix-i-single-task-500333-milliseconds-blink).

Implementation would be simple if we could run two independent loops.

```c++
void loop_500_ms() {
  digitalWrite(BUILTIN_LED, HIGH);
  vTaskDelay(pdMS_TO_TICKS(500));
  digitalWrite(BUILTIN_LED, LOW);
  vTaskDelay(pdMS_TO_TICKS(500));
}

void loop_333_ms() {
  digitalWrite(BUILTIN_LED, HIGH);
  vTaskDelay(pdMS_TO_TICKS(333));
  digitalWrite(BUILTIN_LED, LOW);
  vTaskDelay(pdMS_TO_TICKS(333));
}
```

Tasks let us do just that. All that's needed is a bit of simple
boilerplate.

## Details

Task code resides in a function whose logic either runs in an endless loop
or, if it needs to stop, shuts itself down. If the function returns,
FreeRTOS restarts the application. To prevent his, the RTOSAid tas
shuts itself down if the task method returns, but it is extremely poor
practice to rely on this.

FreeRTOS never stops. It always runs _some_ task. If it has no real
work to do, FreeRTOS runs its built-in, do-little idle task.

The idle task has one important function, resetting the timer that
detects runaway tasks. Should this timer, the "watchdog timer", expire,
FreeRTOS assumes that a user process has entered an endless loop and
reboots the system. Resetting the watchdog timer is **vital**.
I cannot emphasize this enough: all tasks (except the idle task,
as explained above) **must** pause periodically
to avoid a panic restart. Tasks can pause by

1. Invoking
   [`vTaskDelay()`](https://docs.espressif.com/projects/esp-idf/en/v5.0/esp32/api-reference/system/freertos.html#_CPPv410vTaskDelayK10TickType_t), 
   [`delay()`](https://www.arduino.cc/reference/en/language/functions/time/delay/),
   or any other function that explicitly pauses the task
2. Suspending
3. Waiting for a notification from another task
4. Waiting for a message to arrive
5. Waiting to acquire a semaphore
6. Waiting for a timer to expire
7. Waiting on a hardware event such as a voltage change on an input GPIO

Paused tasks consume no CPU, though they continue to occupy storage.
If a task starts asychronous I/O before pausing,
the I/O operation can continue while the task waits and, if so directed,
wake the task upon completion.

A task can be in the following states

* Ready: ready to run
* Running: actively running
* Delayed: waiting for a set time to elapse
* Suspended: quiescent, waiting to be resumed
* Waiting: waiting for a notification or an event

:warning: **Warning:** be sure to wake a quiescent task appropriately.
For example, application code must not attempt to resume a task that is
waiting for a timer to expire.

## Creating a Task

To create a task, users must provide

1. `TaskWithAction` requires stack storage. The task uses to manage function
   invocations and to hold automatic variables. `TaskWithActionH` allocates
   its own stack.
2. Task logic, code that the tasks runs.
   resides in a 
   [`TaskAction`](#taskaction-class)
   subclass.
3. A 
   [`TaskWithAction`](#taskwithaction-class)  or
   [`TaskWithActionH`](#taskwithactionn-class)
   instance, which runs the task logic.
4. A priority, an unsigned integer between 1 and 24. Larger numbers indicate
   higher priority, and priority 0 is reserved for the idle task.

:warning: **Warning**: the system idle task runs at priority 0. User tasks
must run at priority 1 or higher.

The following `BlinkAction` shows how to implement task logic.

```c++
class BlinkAction : public TaskAction {
  uint8_t pin_no;
  uint32_t delay_ms;

public:
  BlinkAction(
      uint8_t pin_no,
      uint32_t delay_ms) :
        pin_no(pin_no),
        delay_ms(delay_ms) {
    pinMode(pin_no, OUTPUT);
    digitalWrite(pin_no, LOW);
  }

  virtual ~BlinkAction() {}

  virtual void run(void) {
    for (;;) {
      digitalWrite(pin_no, HIGH);
      vTaskDelay(pdMS_TO_TICKS(delay_ms));
      digitalWrite(pin_no, LOW);
      vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
  }
};
```

The `BlinkAction` constructor accepts the target GPIO number and
delay, and sets the specified GPIO to `OUTPUT`. Its `run()` implementation
provides our desired independently running blink logic.

Given the `BlinkAction`, we can blink as many LEDs as we please. Here's how
we would use it to blink the built-in LED.

Note that the library provides `BlinkAction`.

```c++
#define BUILTIN_LED_PIN 2

static BlinkAction builtin_action(BUILTIN_LED_PIN, 500);
static uint8_t builtin_led_stack[2048];
static TaskWithAction builtin_task(
    "BuiltIn",
    2,
    &builtin_action,
    builtin_led_stack,
    sizeof(builtin_led_stack));
```

The `builtin_led_stack` array provides the temporary storage
needed to run the task. Since this is a small, simple task, 2048 bytes
is more than enough. If we used `TaskActionH`, we would not need to
provide it.

When started, `builtin_task` runs `builtin_action.run()`, which blinks the LED.

We can also create a similar task that blinks the red LED. 

```c++
static BlinkAction red_action(RED_LED_PIN, 333);
static uint8_t red_led_stack[2048];
static TaskWithAction red_task(
    "Red",
    2,
    &red_action,
    red_led_stack,
    sizeof(red_led_stack));
```

All that remains is to start the two tasks, which we do in `setup()`.

```c++
void setup() {
  Serial.begin(115200);
  Serial.printf(
      "Dual task blink sketch compiled on %s at %s.\n",
      __DATE__,
      __TIME__);

  builtin_task.start();
  red_task.start();

  Serial.println("Setup completed.");
}
```

The two tasks do all the work, so there's nothing to do in the `loop()` function. To
minimize CPU load, we put it to sleep with `vTaskDelay()` as follows:

```c++
void loop() {
  vTaskDelay(portMAX_DELAY);
}
```

:arrow_forward: **Note**: `portMAX_DELAY` is the longest possible delay time.

Please refer to Appendex II for the entire sketch.

## `TaskWithAction` Class

The `TaskWithAction` class manages an ESP32 FreeRTOS task, and provides
an API that lets callers, start, stop, suspend, resume, and notify tasks.
The managed task can also for notification pause for a specified duration,
suspend itself, and yield to higher priority tasks.

### Constructor

```c++
TaskWithAction(
  const char *name,
  uint16_t priority,
  TaskAction *action,
  void *stack,
  size_t stack_size)
```

Creates a `TaskWithAction` that runs the logic provided in the specified
`TaskAction`. All required storage is provided by the caller
or within the newly created instance. The newly created task will be
stopped and will not run until the appliction invokes `start()` .

Parameters:

| Name         | Contents                                                          |
| ------------ | ------------------------------------------------------------------|
| `name`       | short, descriptive, `NULL`-terminated task name.                  |
| `priority`   | Task priority, 1 and 24, inclusive. The scheduler favors higher numbered priorities. |
| `action`     | The task's runtime logic, the program that the task runs          |
| `stack`      | Storage for function invocation and automatic variables           |
| `stack_size` | The `stack` size, e.g. `sizeof(stack)` in bytes.                  |

:arrow_forward: **Note**: `name` should be unique, as it identifies the
guilty task when an error occurrs.

:arrow_forward: **Note**: the stack is untyped. In the absence of compelling
reasons to the contrary, declare stack storage as `uint8_t` (a.k.a.
 `unsigned char`).
 
:arrow_forward: **Note**: determining the proper stack size is more an art
than a science. Simple tasks that use a few small automatic variables and do
not have deeply nested function calls need little stack storage, while
tasks that consume lots of automatic storage or perform deeply nested
function calls require more. 4096
bytes is a good starting size, though the very simplest tasks can get by with
2048 or even less.
### Destructor

Stops a task if it is running.

:arrow_forward: **Note:** tasks instances are seldom deleted, so the destructor
is implemented for the sake of completeness. Deleting a running task is
extremely poor practice. Prefer invoking [`stop()`](#stop) first.

### notify

Resumes a task that is waiting for a notification. Does nothing if the
task is suspended or not waiting for a notification. This method is **only** for
use by application code. ISR code **must** invoke 
[`notify_from_isr()`](#notify_from_isr)
instead.

### notify_from_isr

Resumes a task that is waiting for notification. Does nothing if the
task is suspended or not waiting for a notification. This method is **only** for
use by interupt service routines (ISRs). Application code **must**
invoke [`notify()`](#notify) instead.

### resume

Resumes a suspended task and does nothing if the task is running. 

### start

Starts a task. The task must be stopped. Invoking `start()` on a running
task will cause unspecified behavior. The task **must** be started
before any other methods can be invoked.

### stop

Stops and destroys a task. The task can be restarted with [`start()`](#start).

:warning: **Warning:** `stop()` will destroy a task in any state. Improper
use can leave the system in an invalud state or cause undesirable behavior.

### suspend

Suspends a running task and does nothing if the task is already suspended.
The 
[`resume()`](#resume)
 method (see above) resumes suspended tasks.

:arrow_forward: **Note**: suspending a suspended task has no effect. No matter
how many times the user invols `suspend()` on a suspended task, the next call
to [`resume()`](#resume) will reactivate it.

:arrow_forward: **Note:** suspended tasks do not respond to notifications.

## `TaskWithActionH` Class

`TaskWithActionH` allocates stack on the heap. Other than that, it behaves identically
to `TaskWithAction`.


### Constructor

```c++
TaskWithActionH(
  const char *name,
  uint16_t priority,
  TaskAction *action,
  size_t stack_size)
```

Creates a `TaskWithActionT` that runs the logic provided in the specified
`TaskAction`. All required storage is provided by the caller
or within the newly created instance. The newly created task will be
stopped and will not run until the appliction invokes `start()` .

Parameters:

| Name         | Contents                                                          |
| ------------ | ------------------------------------------------------------------|
| `name`       | short, descriptive, `NULL`-terminated task name.                  |
| `priority`   | Task priority, 1 and 24, inclusive. The scheduler favors higher numbered priorities. |
| `action`     | The task's runtime logic, the program that the task runs          |
| `stack_size` | The `stack` size, e.g. `sizeof(stack)` in bytes.                  |

### Other Methods

`TaskWithActionH` implements the same methods as `TaskWithAction`. (In fact, the
methods are implemented in `BaseTaskWithAction`, a common base class.) See
`TaskWithAction` above for details.

## `TaskAction` Class

Base class whose subclasses provide task logic. To use it:

1. Declare a concrete class that inherits `TaskAction` publicly.
   The concrete class **must** declare a `run()` function which
   must **not** be pure, which means that `= 0` does **not** follow
   its declaration.
2. Implement that logic loop in the class's `run()` function. Implementation
   is mandatory.

`TaskAction` has only one public function, `run()`, which is not implemented.
All of its implemented functions are `protected`, meaning that they only
child classes (i.e. classes that inherit from `TaskAction`) can use them.

### delay_millis

Protected function that stops the code from running for a specified time. The
code will resume running will run on or after the specified time. Since a
higher priority task could be running when when this task becomes ready to
run again, the actual elapsed time could be greater than requested.

Parameters:

| Name     | Contents                                                      |
| -------- | -------------------------------------------------------------
| `millis` | Delay in milliseconds, a non-negative value. Returns immediately if 0.|

Returns: nothing

:arrow_forward: **Note**: `delay_millis` is provided for `TaskAction` implementations.
If invoked from another task, it will _not_ delay the invoked task. It will delay the
_invoking_ task.

### run

A place holder for the task logic. `TaskAction` does **not** implement
this function. Classes that inherit `TaskAction` **must** implement it.
This is a hard requirement.

Parameters: none

Returns: nothing

### wait_for_notification

Stop running until the task receives a notification or a specified time
passes. Since a higher priority task could be running when the task
receives a notification or the wait times out, the task might not run
immediately when it becomes elegible.

Parameters:

| Name     | Contents                                                      |
| -------- | -------------------------------------------------------------
| `millis` | Delay in milliseconds, a non-negative value. Returns immediately if 0.  Omit for longest possible delay. |

Returns: 0 if the task was notified, non-zero if the wait timed out.


### yield

Relinquish control to higher priority tasks. Returns when all higher
priority tasks are waiting. 

:warning:**Warning**: the idle task will **not** run when `yield()` is called.
If you want the idle task to run, invoke `delay_millis()` with a strictly
positive delay.

# Pull Queues

Applications use pull queues, which transmit messages in first in, first out
(FIFO) order, to send data between tasks. Typically, a receiving task removes
messages from the queue and responds to them, while any number of sending
tasks add messages to the queue.

## Overview

Queues provide reliable transport between tasks. Their advantages include:

1. Asynchronous send and receive: queues automatically isolate sending
   and receiving, making it impossible for them to interfere with each
   other.
2. Overlap: sending and receiving take place simultaneously (nominally, at
   least) without requiring special application logic.
3. Scalability: developers can add queue capacity by enlarging the queue's
   buffer, which is extremely simple to do.
4. Load leveling: queues absorb load spikes by holding messages for later
   processing. This simplifies background processing, where the queue
   delivers messages to a low priority background task.
5. Fan in: any number of tasks can add messages to a queue, simplifying
   the implementation of client/server architectures
6. Multi-task and multi-thread safety: multiple tasks can send and receive
   queue entries simultaneously without colliding. This makes queues the
   preferred inter-task communication mechanism.

The only cost is storage, the memory required to store messages for later
processing, and managing queue overflow should the queue run out
of storage.

:arrow_forward: **Note**: as stated above, queues are the preferred
inter-task communication mechanism. Prefer them in wherever possible.

## Details

Pull queue code resides in two classes: `BasePullQueue` a base class that
implements the core queue logic, and `PullQueueT`, a templated wrapper class
that adds type safely. To enforce type safely, the base class a protected API,
making its services available only to child classes (i.e. classes,
like `PullQueueT` that inherit the base class). Since the two classes work
in concert, we document their public messages as a single API.

Users access queues via the `PullQueueT` class.

:arrow_forward: **Note**: to ensure type safety, the `PullQueueT` class is
templated by the message type, which is set at instance declaration.In principal,
each `PullQueueT` defines a unique class for each message type. The
class design minimizes the resulting overhead.

<!-- TODO: reorganize this!  -->

:warning: specify a message as plain, C-style `struct`, a structure (as opposed
to a class) without methods, not even a constructor or destructor, as in


```c++
struct LedCommand {
  uint16_t step_time;
  uint16_t inter_cycle_time;
  uint16_t repititions;
```

:arrow_forward: **Note**: in the example, `LedCommand` is declared in `LedCommand.h`.

Complete details are provided under the 
[`Flash32ReadOnlyNamespace`](#flash32readonlynamespace-class)
[`get_struct()`](#get_struct)
method.

Attempting to send a `class` is risky and likely to fail.
## Creating a Queue

To create a queue, users must provide

1. Its message type
2. Storage for enqueued messages
3. Storage capacity, the maximum number of messages that the queue can hold

Provide an array of `T` to hold enqueued messages. The resulting
queue capacity capacity is the array length.

```c++
#include "LedCommand.h"
#include "PullQueueT.h"

#define QUEUE_CAPACITY 5

static LedCommand queue_storage[QUEUE_CAPACITY];

static PullQueueT<LedCommand> led_command_queue(
    queue_storage,
    QUEUE_CAPACITY);
```

:arrow_forward: **Note**: `led_command_ queue` will be able to store 5
`LedCommand` messages.

## PullQueueT and BasePullQueue classes

### Constructor

```c++
PullQueueT(
    T *queue_storage,
    UBaseType_t queue_length);
```

Parameters:

| Name            | Contents |
| --------------- | ----------------------------------------------------------|
| `T`             | Message as a `struct`, the template avalue                |
| `queue_storage` | An array of `T` to hold enqueued messages                 |
| `queue_length`  | Number of elements in the `queue_storage` array           |

The constructor configures a newly created queue. The resulting queue is not
running. Applications must start it to make it usable.

### Destructor

The destructor stops the queue if it is running, and has no effect otherwise.

### peek_message (single parameter)

```c++
peek_message(
    T *message);
```

Receive a message from the queue, waiting forever for a message to arrive
without remoinving the message from the queue. The message remains
in the queue, available for reading.

Paremters:

| Name      | Contents                                           |
| --------- | -------------------------------------------------- |
| `message` | Holds the retrieved message when the call returns. |

Returns: `true`

:arrow_forward: **Note** this variant of the `peek_message` method returns a
`bool` for compatibility with its two parameter version documented below.

### `peek_message` (two parameters)

Waits a specified interval for a message arrives, and receives it from the
queue, without removing the message (if come) from the queue. Should a message
arrive, it remains in the queue, available for reading.
If a message is already waiting, the function returns immediately.

Paremters:

| Name          | Contents                                           |
| ------------- | -------------------------------------------------- |
| `message`     | Holds the retrieved message when the call returns. |
| `max_wait_ms` | The maximum time to wait in milliseconds.          |

Returns: `true` if a message arrived, `false` otherwise.

### pull_message (single parameter)

```c++
pull_message(
    T *messsage);
```

Receive and remove a message from the queue, waiting forever for a message
to arrive.

Paremters:

| Name      | Contents                                           |
| --------- | -------------------------------------------------- |
| `message` | Holds the retrieved message when the call returns. |

Returns: `true`

:arrow_forward: **Note** this variant of the `pull_message` method returns a
`bool` for compatibility with its two parameter version documented below.

### pull_message (two parameter)

Wait a specified time and pull a message from the queue should a message arrive
during time. If a message is received, it is removed from the queue. If a
message is already present in the queue, the function returns immediately.


Paremters:

| Name          | Contents                                           |
| ------------- | -------------------------------------------------- |
| `message`     | Holds the retrieved message when the call returns. |
| `max_wait_ms` | The maximum time to wait in milliseconds.          |

Returns: `true` if a message arrived, `false` otherwise.

### send_message (single parameter)

```c++
send_message(
    T *messsage);
```

Send a message to the rear of the queue, waiting forever for the queue
to accept the message. 

:arrow_forward: **Note**: massages arrive in order. The newly added message will
arrive at the receiver **after** previously added messages.

Paremters:

| Name      | Contents                                           |
| --------- | -------------------------------------------------- |
| `message` | Holds the retrieved message when the call returns. |

Returns: `true`

:arrow_forward: **Note** this variant of the `send_message` method returns a
`bool` for compatibility with its two parameter version documented below.

### send_message (two parameter)

Send a message to the rear of the queue, waiting the specified time for the queue
to accept the message. The newly added message will arrive at the receiver
**after** preexisting messages arrive.


Paremters:

| Name          | Contents                                           |
| ------------- | -------------------------------------------------- |
| `message`     | Holds the retrieved message when the call returns. |
| `max_wait_ms` | The maximum time to wait in milliseconds.          |

Returns: `true` if the queue accepts the message, `false` otherwise.

:arrow_forward: **Note**: the newly added message will arrive at the receiver
**after** preexisting messages arrive.

<!-- TODO: provide a PullQueueHT class. -->

# Mutual Exclusion Semaphore (Mutex)

Mutual Exclusion Semaphores, a.k.a. Mutexes, prevent multiple
tasks from colliding. When used properly, A Mutex ensures that logic
runs only in a single task by forcing other tasks to wait until the current
task finishes.

:arrow_forward: **Note**: mutexes are best avoided, and should be used only
when necessary. Prefer
[queues](#pullqueuet-and-basepullqueue-classes).

## Overview

A mutex can be in one of two states: unlocked and locked. When the
mutex is locked, a single task owns the lock, and is the only task
that should unlock it. Any task can
attempt to lock the mutex no matter its state, with two possible results:

If the mutex is currently unlocked, the attempt succeeds. The requesting
task acquires the lock and keeps running. When the task no longer needs to
hold the lock, the task that holds it must unlock the mutex to allow
other tasks that have requested the lock (if any) to run.

If, on the other hand, a task attempts to lock a mutex that is already
locked, it blocks until the holding task releases the lock.
It then acquires the lock as described above, surrendering it,
as always, when it no longer needs the lock.

:arrow_forward: **Note**: tasks should unlock semaphores as quickly as possible,
holding the mutex lock for the shortest possible time. Holding locks too long
will degrade performance and could trip the watchdog timer causing the system to
crash and reboot.

Mutexes are used as follows.

1. A task attempts to lock the mutex. The request can specify a
   maximum wait time or request that the call should wait
   indefinitely for the lock.
2. If the lock attempt fails, handle the error.
3. If the lock attempt succeeds, continue to perform
   the application logic
4. Release the mutex lock. If other tasks have attempted
   to lock the semaphore, select one to run and grant it
   the lock.

:warning: **Warning**: improper mutex use can cause
[deadlock](https://en.wikipedia.org/wiki/Deadlock).
Holding locks to multiple mutexes is extremely poor practice.
Limit tasks to holding at most one lock at a time.

:warning: **Warning**: a task holding a lock **MUST** release it
promptly. Holding a lock forever can hang the system, triggering the
watchdog timer and rebooting the system.

:warning: **Warning**: release locked mutexes promptly. Holding
a lock too long can hang the system, triggering the watchdog timer
and causing a reboot.

:warning: **Warning**: a task **MUST** lock a mutex **at most once**. The `Mutex`
class does **not** support recursive locks. Attempting to relock a
`Mutex` will cause unspecified and undesirable behavior.

The API is designed to assure that locks are released eventually,
and to be released automatically if if an exception is thrown. Users
are responsible for ensuring prompt release.

## `Mutex` Class

The `Mutex` class maintain a list of tasks that are waiting for its lock,
and doles out its lock to one task at a time.

:arrow_forward: **Note**: to ensure eventual `Mutex` release, especially 
when an unhandled exception is thrown, the `Mutex` class does not provide
user-accessible lock and unlock methods. Please see
[`MutexLock`(#mutexlock-class) below for details.


### `begin()`

Configures a `Mutex` and makes it available to receive lock requests.
Applications **MUST** invoke `begin()` **exactly once** before atempting
to use the mutex.

Returns: `true` if the `Mutex` is configured and ready to use, `false` otherwise.

### `valid()`

Checks if the mutex is valid

Returns: `true` if the mutex is ready to use, `false` otherwise. `valid()` always returns `false` until `begin()`
runs successfully.

## `MutexLock` Class

The `MutexLock` class locks `Mutex` instances. Its constructor takes the `Mutex`
to be locked, together with an optional time to wait. If the caller provides a
wait time, the lock will time out when the interval expires and the constructor
will return without locking the mutex. Otherwise, the constructor will
wait for the lock indefinitely.

:warning: **Warning**: check the lock status of a newly created `MutexLock`.

:warning: **Warning**: `MutexLock` instances **MUST** reside on the stack, i.e.
be automatic variables. Never create them with `new`.

:warning: **Warning** never create a `MutexLock` class field.

:warning: **Warning**: never subclass `MutexLock`.

To enforce the foregoing,

* All forms of the `new` operator have been declared `private`.
* `MutexLock` is declared `final`.

### Constructor (One Argument)

Lock a mutex, waiting indefinitely for the lock to be granted

Parameters:

| Name    | Contents            |
| ------- | ------------------- |
| `mutex` | The `Mutex` to lock |

:warning: **Warning**: check the lock status before proceeding.

### Constructor (Two Arguments)

Lock a `Mutex` with timeout. If the wait time exceeds the specified
timeout, return without locking.

Parameters:

| Name                  | Contents                                               |
| --------------------- | ------------------------------------------------------ |
| `mutex`               | The `Mutex` to lock                                    |
| `wait_time_in_millis` | The maximum time to wait for the lock, in milliseconds |

:warning: **Warning**: check the lock status before proceeding.

### Destructor

Release the `Mutex` lock if acquired, does nothing otherwise

### `succeeded()`

Check the lock status

Returns: `true` if the underlying `Mutex` is locked, `false` otherwise. Applications
should always invoke this after constructing a `MutexLock` with a timeout.

### Example of Use

Here's how to use a `MutexLock` in a method. Assume that
the containing class contains a `Mutex` field named `mutex`.

```c++
void SomeClass::do_something(void) {
  MutexLock lock(mutex);  // Try to lock the mutex
  if (lock.succeeded()) {
    // Lock succeeded; carry on processing. Note that the
    // current task is the only task that is running this
    // code ...
  } else {
    // Lock request failed. Handle the error ...
  }

  // All automatic variables, including the MutexLock lock, are
  // destroyed here. When the lock variable is destroyed, its
  // destructor releases the lock acquired during construction.
  // There is no other way to release the lock.
}
```

# Function Classes

A function class is a class that acts as a stand in for a function.

## Overview

Typically, a function class has a single pure virtual function where
subclasses place the function's logic. We have already seen an example
in
[`TaskAction`](#taskaction-class).

## VoidFunction Class

The `VoidFunction` class stands in for a void no-argument function,
one that takes no parameters and returns nothing. It is not usable
directly. Users must subclass it and implement the function.

### Constructor

The constructor is a placeholder that does nothing.

### Destructor

The destructor is a placeholder that does nothing.

### `apply()`

`apply()` is a pure virtual void, no argument function that the
`VoidFunction` class does not implement. Subclasses must provide an
implementation. Implementations can do anything including keeping
arbitrary state and causing arbitrary side effects. There is only
one restriction, implementatons **must** return eventually, and
might, in some circumstances, be required to return promptly.


# Timers

Timers provide a non-blocking mechanism to run an action after
a precise delay.

## Overview

So far, we have only one way to run a logic in the future: wait the
desired time, then proceed. Sometimes, though it is handy to have
a "fire and forget" mechanism, a way to stage a task to be run,
then get on with the application logic. Timers provide that
mechanism.

## MicrosecondTimer Class

A `MicrosecondTimer` allows applications to schedule a `VoidFunction`
invocation after a precise delay. The delay is specified in microseconds.
Contrast this with `vTaskDelay()` and `delay()` whose times are specified
in milliseconds.

### Constructor

The constructor creates a `MicrosecondTimer` instance that runs
a `VoidFunction` on expiration.

Parameters:

| Name              | Contents                                                    |
| ----------------- | ----------------------------------------------------------- |
| `timer_name`      | The timer's name, primarily used for debugging. Should be unique | 
| `target_function` | A `VoidFunction` subtype. The timer invokes its `apply()` function when it expires |

### Destructor

If the timer has been initialized (see `begin()` below), the destructor tears it down.

### `begin()`

`begin()` initializes the timer. Applications must call it **exactly once** before
attempting to use the timer.

Returns: `true` if timer initialization succeeded, `false` if it failed.

### `start()`

`start()` sets the timout delay and starts the timer. If the timer
is already running, `start()` resets the timeout delay, and the
timer keeps running.

Parameters:

| Name                      | Contents                                                |
|-------------------------- | ------------------------------------------------------- |
| `timeout_in_microseconds` | Expiration time in microseconds                         |

### `stop()`

`stop()` stops the timer if it is running and does nothing if it is stopped.

# GPIO Input Change Detector

A GPIO Change Detector monitors a GPIO input pin's voltage
and invokes a `VoidFunction` when it changes.

:arrow_forward: **Note**: a GPIO can support *at most one* input change detector.
Attempting to attach multiple change detectors to a pin will cause
unspecified behavior.

## Overview

The detectors can respond to

* `LOW` to `HIGH` transitions
* `HIGH` to `LOW` transitions
* Both `LOW` to `HIGH` and `HIGH` to `LOW` transitions

The detectors invoke a `VoidFunction` when they detect a
transition.

The change detector provides two classes, `GpioChangeService` to manage
the low level FreeRTOS GPIO change detection service, and
`GpioChangeDetector` which watches individual GPIO pins.

## `GpioChangeService`

`GpioChangeService` is a global singleton akin to the Arduino
library's `attachInterrupt` function. It installs the FreeRTOS low
level GPIO level change sensor, and must be started for change
detectors to work. It supports the two cannonical service
management functions: `begin()` and `end()`.

### begin()

`begin()` starts the low level FreeRTOS GPIO change detection service
if it is not already running. If the service is running,
`begin()` does nothing. Applications should invoke `begin()` before
starting any change detectors.

### `end()`

`end()` stops the low level FreeRTOS GPIO change detection service.
Be sure to stop all change detection before invoking `end()`.

## `GpioChangeType` Enumeration

`GpioChangeType` specifies the change to detect. It has the following
values.

| Name          | Transition                                                    |
| ------------- | ------------------------------------------------------------- |
| `LOW_TO_HIGH` | Voltage change from `LOW` to `HIGH`                           |
| `HIGH_TO_LOW` | Voltage change from `HIGH` to `LOW`                           |
| `ANY_CHANGE`  | Any voltage change, either `LOW` to `HIGH` or `HIGH` to `LOW` |

The enumeration is used to configure `GpioChangeDetector` instances as described below.

## `GpioChangeDetector` Class

`GpioChangeDetector` changes watch input GPIO pin voltage for a specified
transition and invoke a `VoidFunction` when a configured transition
occurs. It runs in an ISR, but its code does not need to reside in SRAM
but **must** conform to all other ISR constraints.

### `start()`

`start()` starts its `GpioChangeHandler`, making it respond when
the voltage on its associated GPIO pin changes. Make sure to invoke
`GpioChangeService.begin()` before invoking `start()` on any
`GpioChangeHandler`.

Returns: `true` if the handler started successfully, false if startup
failed.

### Constuctor

The constructor creates a `GpioChangeDetector` instance that watches
an input GPIO input pin for a specified voltage change and invokes a
`VoidFunction` when the change occurs.

Parameters

| Name             | Contents                                                                        |
| ---------------- | ------------------------------------------------------------------------------- |
| `pin_no`         | The GPIO pin to watch. The caller must configure the pin for input              |
| `change_type`    | the `GpioChangeType` to detect                                                  |
| `on_pin_changed` | The `VoidFunction` to invoke when the `change_type` occurs on GPIO pin `pin_no` |

`on_pin_changed->apply()` runs in an interrupt service routine. It need not reside
in SRAM, but must comply with all other ISR restrictions.

### Destructor

Invokes `stop()` to stop this `GpioChangeHandler`

### `stop()`

`stop()` stops its `GpioChangeHandler`, making it unresponsive
to voltage changes on its GPIO input pin. Be sure to invoke
`stop()` on every `GpioChangeHandler` before invoking
`GpioChangeService.end()`.

### `change_interrupt_handler()`

The low level `FreeRTOS` change handler invokes the static
`change_interrupt_handler()` method when the desired voltage change
occurs. Application code cannot invoke it.

Parameters:

| Name      | Contents                                             |
| --------- | ---------------------------------------------------- |
| `params`  | A `void` pointer containing an unspecified parameter |

`params` always points to the `GpioChangeDetector` configured to handle
voltage changes on the signaling the `GpioChangeDetector`. The
function invokes the target detector's `handle_pin_change()` function.

### `handle_pin_change()`

`handle_pin_change` invokes the `VoidFunction` that was bound to this
`GpioChangeDetector` instance.  Application code cannot invoke it.

# Debounced GPIO Input Change Detection

Like the [`GpioChangeDetector`](#gpiochangedetector-class) class, the
`GpioDebouncer` responds when the voltage changes on a GPIO input pin.
Unlike the [`GpioChangeDetector`](#gpiochangedetector-class), which is
sensitve to noise, the `GpioDebouncer` removes bounce, spurrious
switch closures and opens.

:arrow_forward: **Note**: since the debouncer uses a
[`GpioChangeDetector`](#gpiochangedetector-class),
a GPIO pin can support at most one. Attempts to attach
multiple debouncers to a pin will cause unspecified behavior.

## Background

Mechanical switches, including our beloved push buttons, are subject to
[contact bounce](https://en.wikipedia.org/wiki/Switch#Contact_bounce)
(a.k.a. "chatter"). Instead of generating clean makes (closes) and
breaks (opens), the switch generates sequences of makes and breaks
whenever it is opened or closed. If the switch is wired "active low" --
one lead connected to ground and the other connected to a GPIO pin
having a pullup resistor, a switch closure generates voltage swiggs
resembling

```
 High (Open)   --> -------+  +---+      +--------+
                          |  |   |      |        |
                          |  |   |      |        |
 Low (Closed) -->         +--+   +------+        +----------------------- ...

                          |<==== Bounce Time ===>|
```

where `---` indicates steady voltege, `|` indicates a changing
voltage, `+` makes the ASCII art look a bit better, and `|<===>|`
represents a time interval. We will also use periods to show the
time remaining on a reset timer.

The `GpioDebouncer` uses a timer to avoid responding to chatter,
the spurious signals resulting from contact bounce. It starts
the timer whenever the voltage changes with the timer configured
to invoke the desired `VoidFunction`, resulting in the following
behavior.


```
 High (Open)   --> -------+  +---+      +--------+
                          |  |   |      |        |
                          |  |   |      |        |
 Low (Closed) -->         +--+   +------+        +----------------------- ...
 Timer running -->        |<=........|
                             |<==.......|
                                 |<=====....|
                                        |<======...|
                                                 |<========>|
                                                            |
                                                            v
                                                            VoidFunction.apply()
```

The delay can be much shorter than the bounce time. As long as the delay
outwaits the longest bounce, the debouncer will work properly.

:warning: ***Warning**: under unusual conditiions such as violent
shaking or contamination, switches can chatter without changing state,
causing its `GpioDebouncer` to invoke its `VoidFunction` spurriously.
If there's any chance that this could happen, be sure that the
invoked `VoidFunction` checks that the switch has actually opened
or closed before proceeding.

## Constituents

The `GpioDebouncer` class assembles a
[`MicrosecondTimer`](#microsecondtimer-class),
[`TaskAction`](#taskaction-class),
[`TaskWithAction`](#taskwithaction-class),
[`VoidFunction`](#voidfunction-class),
[`GpioChangeDetector`](#gpiochangedetector-class),
and a few lines of custom logic to detect switch activity,
remove the ensuing chatter, and apply a user-provided
[`VoidFunction`](#voidfunction-class). The
custom logic resides in
[`VoidFunction`](#voidfunction-class)
and 
[`TaskAction`](#taskaction-class) implementations.

### `GpioDebouncerAction` Internal Class

`GpioDebouncerAction` is a
[`TaskAction`](#taskaction-class)
subclass whose `run()` function starts a
[`MicrosecondTimer`](#microsecondtimer-class)
when it receives a notification. The
[`MicrosecondTimer`](#microsecondtimer-class)
invokes the user-provided
[`VoidFunction`](#voidfunction-class)

:arrow_forward: **Note**: Access to the `GpioDebouncerAction` class
is restricted. Application code cannot use it.

### `GpioDebounceFunction` Internal Class

`GpioDebounceFunction` is a `VoidFunction` subclass whose
`apply()` function notifies the task running the `GpioDebouncerAction`.
Since `GpioDebounceFunction` runs as an ISR, it notifies the task via
its `notify_from_isr()` function.

:arrow_forward: **Note**: Access to the `GpioDebounceFunction` class
is restricted. Application code cannot use it. 

### GPIO Voltage Change Processing

```
                Change       Debounce     Debounce    Millisecond    User-Provided
  GPIO          Dector       Function       Task         Timer        VoidFunction
    |  Voltage    |            |             |             |              |
    +-  Change -->+            |             |             |              |
    |             |            |             |             |              |
    |             +-- Apply -->+             |             |              |
    |             |            |             |             |              |
    |             |            +-- Notify - >+             |              |
    |             |            |             |             |              |
    |             |            |             +--- Start -->+              |
    |             |            |             |             |              |
    |             |            |             |             +- Expire -+   |
    |             |            |             |             |          |   |
    |             |            |             |             |          v   |
    |             |            |             |             |<---------+   |
    |             |            |             |             |              |
    |             |            |             |             |              |
    |             |            |             |             +--- apply --->+
    |             |            |             |             |              |
```

If the GPIO input voltage changes before the timer expires, the timer is
reset and the wait restarts. When the voltage finally settles down, the
timer expires and invokes the user-provided
[`VoidFunction`](#voidfunction-class) implementation.


## `GpioDebouncer` Class

`GpioDebouncer` packages its components into an easily
used class. In additiion to solving a knotty problem, it
demonstrates RTOSAid library component aggregation into
an application-specific solution.

The `GpioDebouncer` is composed of the following RTOSAid classes
together with a few lines of custom logic

* [`GpioChangeDetector`](#gpiochangedetector-class)
* [`MicrosecondTimer`](#microsecondtimer-class)
* [`TaskAction`](#taskaction-class) subclass containing a about half
  the custom logic
* [`TaskWithAction`](#taskwithaction-class)
* [`VoidFunction`](#voidfunction-class) subclass containing the rest
  of the custom logic.

In addition to providing a vital function, the `GpioDebouncer` shows
how to implement complex functionality by assembling RTOSAid classes.

:arrow_forward: **Note**: since `GpioDebouncer` uses a
[`MicrosecondTimer`](#microsecondtimer-class),
be sure to invoke [`GpioChangeService.begin()`](#begin-2) before
using it.

:arrow_forward: **Note**: `GpioDebounce` provides stack for its debounce task,
which means it has a large memory footprint. Prefer allocating instances
statically.

:arrow_forward: **Note**: as stated above, violent acceleration can
cause a `GpioDebouncer` to detect a non-existant change, e.g a change
from closed to closed. If your project is subject to extreme
mechanical shock, be sure th check that the switch changed.

<!-- TODO: switch to TaskWithActionH -->

### Constructor

Configures a `GpioDebouncer`

Parameters


| Name                    | Contents                                  |
| ----------------------- | ----------------------------------------- |
| `pin_no`                | GPIO pin to monitor                       |
| `debounce_delay_micros` | The time to wait after a voltage change   |
| `task_name`             | Unique debounce task name.                |
| `priority`              | Timer task priority, which should be high |
| `timer_name`            | Name of the debounce timer. |
| `function_to_call`      | The [`VoidFunction`](#voidfunction-class) to apply when switch chatter settles down |

### Destructor

The destructor stops `GpioChangeDetector`. When this method succeeds, the
`GpioChangeDetector` does not monitor the GPIO input pin and will not respond
to switch activity.

Best practice is to invole `stop()` before destroying a `GpioDebouncer`.

### `start()`

Starts the detector. after this function succeeds, the `GpioDebouncer` notifies
its task whenever the pin voltage changes.

:arrow_forward: **Note**: be sure to invoke [
`GpioChangeService.begin()`](#begin-2)
before invoking `start()`.

### `stop()`

Tears down the `GpioDebouncer`. When this function returns, the
`GpioDebouncer` stops all notification. Users can restart it by
calling `start()`.

# Flash Memory

ESP32 software can persist data in flash memory. This is useful for storing
configurations and the like, infrequently changing data that must
be preserved when the system is powered down.

:warning: **Warning**: flash memory is not suitable for storing frequently
changed data because it degrades when its contents are altered, typically,
failing after 100,000 writes. Store volatile data in
robust media like SD cards, solid state disks, or
[FRAM](https://www.adafruit.com/product/4719). FRAM is extremely robust,
with claimed lifetimes of 10,000,000,000,000 read/write cycles.

## Background

The ESP32 supports a non-volatile storage (NVS) library that maintains key-value
pairs in flash memory. Key-value pairs reside in namespaces which, in turn,
reside in partitions. The ESP32 flash library supports advanced features
that simple applications don't need, like encryption, multiple memory
partitions, and loading flash memory from a CSV files. The Flash32
library supports basic functionaly needed by most applications:
maintaining namespaces and their contained key-value pairs in the
default partition.

:arrow_forward: **Note**: consistent with the RTOSAid design principles,
the flash memory API supports only essential functionality by

* Using only the default flash partition
* Storing key-value pairs only in cleartext. Encryption is not supported.


## `Flash32MemoryState` Enumeration

The `Flash32MemoryState` enumeration represents the overall state of the
flash API. It has the following values.

| Value       | State                                                               |
| ----------- | ------------------------------------------------------------------- |
| `CLOSED`    | Flash memory API has not been started and cannot be used.           |
| `OPENED`    | Flash memory is started and ready for use.                          |
| `CORRUPTED` | Flash memory is corrupt and cannot be used. This is a hard failure. |

## `Flash32Status` Enumeration

Most API functions return a `Flash32Status` enumeration which has the following
values.

| Value         | Indication                                                  |
| ------------- | ----------------------------------------------------------- |
| `OK`          | Invocation succeeded                                        |
| `NOT_FOUND`   | Entry not found                                             |
| `NO_ROOM`     | Flash memory is full.                                       |
| `INVALID_KEY` | Key has an invalid format or contains an invalid character. |
| `CLOSED`      | Namespace is closed.                                        |
| `NOT_STARTED` | Flash memory API has not been started.                      |
| `FAILED`      | Call failed.                                                |

## `Flash32`

`Flash32` is a globally-visible instance of the `HardwareFlash32` class.
It must be the only instance of the class.

:warning: **Warning**: do not create other `HardwareFlash32` instances.

### `begin()`

`begin()` initializes the Flash API. If the call succeeds, the API
becomes usable.

Returns: `true` if the call succeeds, `false` otherwise.

If the API is already open, the call does nothing and returns `true`.
Failure indicates that Flash memory is corrupt.

### `end()`

`end()` shuts down the Flash API. 

Returns: `true` if the call succeeds, `false` otherwise.

If the API is already closed, the call does nothing and returns
`true`. Failure indicates that flash memory is corrupt.

:arrow_forward: **Note**: be sure to invoke `end()` when you have
finished using the Flash32 API to release library resources.

### `ready()`

`ready()` checks if the Flash API is ready for use. 

Returns: `true` if the API is open and ready for use, `false` otherwise.

### `state()`

`state()` returns the current Flash API global state. It was included
primarily for testing. Prefer `ready()` where possible.

## `Flash32ReadOnlyNamespace` Class

The `Flash32ReadOnlyNamespace` class supports read only access to
key-value pairs in a flash namespace. The namespace is set during
construction.

### Constructor

The `Flash32ReadOnlyNamespace` constructor creates instance for
a specified namespace.

Parameters:

| Name   | Contents                                      |
| ------ | --------------------------------------------- |
| `name` | Points to a `null`-termanated namespace name. |

:arrow_forward: **Note**: be sure to invoke
[`Flash32.begin()`](#begin-3)
before you call any functions in this class.

### `close()`

`close()` closes an open namespace.

Returns: a `Flash32Status`

If the namespace is already closed, the call does nothing and returns
`Flash32Status::OK`.

### `get_int8()`

`get_int8()` retrieves a signed byte, a.k.a. `char` value from flash.

Parameters:

| Name     | Contents                                                                    |
| -------- | --------------------------------------------------------------------------- |
| `key`    | The value's key                                                             |
| `value`  | Points to a `int8_t` variable that receives the retrieved value, if found.  |

Returns: the call status, with `Flash32Status::OK` indicating success and
`Flash32Status::NOT_FOUND` indicating that the key does not exist in the namespace.
If the call fails, `value`'s content is unspecified.


### `get_uint8()`

`get_uint8()` retrieves an usigned byte, a.k.a. `unsigned char` value from flash.

Parameters:

| Name     | Contents                                                                    |
| -------- | --------------------------------------------------------------------------- |
| `key`    | The value's key                                                             |
| `value`  | Points to a `uint8_t` variable that receives the retrieved value, if found. |

Returns: the call status, with `Flash32Status::OK` indicating success and
`Flash32Status::NOT_FOUND` indicating that the key does not exist in the namespace.
If the call fails, `value`'s content is unspecified.

### `get_int16()`

`get_int16()` retrieves a signed 16 bit integer value from flash.

Parameters:

| Name     | Contents                                                                     |
| -------- | ---------------------------------------------------------------------------- |
| `key`    | The value's key                                                              |
| `value`  | Points to a `int16_t` variable that receives the retrieved value, if found.  |

Returns: the call status, with `Flash32Status::OK` indicating success and
`Flash32Status::NOT_FOUND` indicating that the key does not exist in the namespace.
If the call fails, `value`'s content is unspecified.


### `get_uint16()`

`get_uint16()` retrieves an unsigned 16 bit integer value from flash.

Parameters:

| Name     | Contents                                                                     |
| -------- | ---------------------------------------------------------------------------- |
| `key`    | The value's key                                                              |
| `value`  | Points to a `uint16_t` variable that receives the retrieved value, if found. |

Returns: the call status, with `Flash32Status::OK` indicating success and
`Flash32Status::NOT_FOUND` indicating that the key does not exist in the namespace.
If the call fails, `value`'s content is unspecified.

### `get_int32()`

`get_int32()` retrieves a signed 32 bit integer value from flash.

Parameters:

| Name     | Contents                                                                     |
| -------- | ---------------------------------------------------------------------------- |
| `key`    | The value's key                                                              |
| `value`  | Points to a `int32_t` variable that receives the retrieved value, if found.  |

Returns: the call status, with `Flash32Status::OK` indicating success and
`Flash32Status::NOT_FOUND` indicating that the key does not exist in the namespace.
If the call fails, `value`'s content is unspecified.

### `get_uint32()`

`get_uint32()` retrieves an unsigned 32 bit integer value from flash.

Parameters:

| Name     | Contents                                                                     |
| -------- | ---------------------------------------------------------------------------- |
| `key`    | The value's key                                                              |
| `value`  | Points to a `uint32_t` variable that receives the retrieved value, if found. |

Returns: the call status, with `Flash32Status::OK` indicating success and
`Flash32Status::NOT_FOUND` indicating that the key does not exist in the namespace.
If the call fails, `value`'s content is unspecified.

### `get_int64()`

`get_int64()` retrieves a signed 64 bit integer value from flash.

Parameters:

| Name     | Contents                                                                     |
| -------- | ---------------------------------------------------------------------------- |
| `key`    | The value's key                                                              |
| `value`  | Points to a `int64_t` variable that receives the retrieved value, if found.  |

Returns: the call status, with `Flash32Status::OK` indicating success and
`Flash32Status::NOT_FOUND` indicating that the key does not exist in the namespace.
If the call fails, `value`'s content is unspecified.

### `get_uint64()`

`get_uint64()` retrieves an unsigned 64 bit integer value from flash.

Parameters:

| Name     | Contents                                                                     |
| -------- | ---------------------------------------------------------------------------- |
| `key`    | The value's key                                                              |
| `value`  | Points to a `uint64_t` variable that receives the retrieved value, if found. |

Returns: the call status, with `Flash32Status::OK` indicating success and
`Flash32Status::NOT_FOUND` indicating that the key does not exist in the namespace.
If the call fails, `value`'s content is unspecified.

### `get_str()`

`get_str()` retrieves a `NULL`-terminated string from flash.

Parameters:

| Name      | Contents                                                                     |
| --------- | ---------------------------------------------------------------------------- |
| `key`     | The value's key                                                              |
| `value`   | Points to a `char` array that receives the retrieved value, if found.        |
| `buf_len` | The number of `char`s in `value`                                             |
| `out_len` | Points to a `size_t` that receives the actual number of characters retrieved |

Returns: the call status, with `Flash32Status::OK` indicating success and
`Flash32Status::NOT_FOUND` indicating that the key does not exist in the namespace.
If the call fails, `value`'s content is unspecified.

### `get_struct()`

`get_struct()` is a function template retrieves a flat `struct` from memory, that is,
a `struct` that whose fields occupy a single contiguous memory block. `get_struct()`'s
template parameter `S` is the `struct` data type.

| Name              | Contents                                                                     |
| ----------------- | ---------------------------------------------------------------------------- |
| `key`             | The value's key                                                              |
| `value`           | Points to a `struct` that receives the retrieved value, if found.            |
| `bytes_retrieved` | Points to a `size_t` that receives the actual number of characters retrieved |

Returns: the call status, with `Flash32Status::OK` indicating success and
`Flash32Status::NOT_FOUND` indicating that the key does not exist in the namespace.
If the call fails, `value`'s content is unspecified.

The `struct`'s field types are restricted to

1. Primitive types like `int`, `char`, `float`, `double`, `int64_t` and so forth.
2. Arrays of primitive types
3. `struct` types whose fields comply with these restrictions
4. Arrays of `struct` types whose fields comply with these restrictions

In particular, the `struct` **MUST NOT** contain any pointer-valued
fields. Prefer arrays instead.

If, for example, you are tempted to include a `char *` field in `S`, do
something like the following instead.

```c++
typedef struct {
  // ...
  char string_value[40];
  // ...
} MyStruct;
```

## `Flash32Namespace` Class

The `Flash32Namespace` class adds a flash memory mutation API to the
[`Flash32ReadOnlyNamespace`](#flash32readonlynamespace-class)
API. The read APIs of both classes are identical. Please
see the
 [`Flash32ReadOnlyNamespace`](#flash32readonlynamespace-class)
abive for details.

### Constructor

The constructor configures a new `Flash32Namespace` instance.

Parameters:

| Name          | Contents                                                 |
| ------------- | -------------------------------------------------------- |
| `name`        | Pointer to a `NULL`-terminated namespace name            |
| `autocommit ` | Automatic or delayed commit flag as described below. Defaults to `true`. |

Changes to flash memory must be committed. If `autocommit` is `true`,
the configured `Flash32Namespace` will commit each change immediately. If it is
`false`, users must invoke `commit()` to ensure that all requirested changes are
written to flash memory.

:arrow_forward: **Note**: be sure to invoke
[`Flash32.begin()`](#begin-3)
before you call any functions in this class.

### `commit()`

`commit()` writes all pending changes to flash memory. If autocommit was
set to `true` in the constructor, invocation does nothing. Otherwise,
be sure to invoke `commit()` before invoking
[`Flash32.end()`](#end-1).

:arrow_forward: **Note**: invoking `commit()` does nothing if there
are no outstanding changes. Redundant `commit()` are harmless. Enjoy!

Returns: a `Flash32Status` indicating success or failure

### `erase()`

`erase()` removes a key-value pair from flash memory.

Parameters

| Name  | Contents                                      |
| ----- | --------------------------------------------- |
| `key` | The target key as a `NULL`-terminated string. |

Returns: a `Flash32Status` indicating success or failure, with
`Flash32Status::OK` indicating that the key-value pair was
removed and `Flash32Status::NOT_FOUND` indicating that the
key-value pair did not exist.

### `erase_all()`

`erase_all()` removes all key-value pairs from the namespace. When
the invocation succeeds, the namespace will be completely empty.

Returns: a `Flash32Status` indicating success or failure

### `open()`

`open()` opens the namespace, making its contents available for
read and modification. Be sure to open the namespace before
attempting to access its contents.

Returns: a `Flash32Status` indicating success or failure

### `set_int8`

`set_int8` writes a key-`int8_t` value pair to flash. If the
the pair already exists, its value is changed.

Parameters:

| Name     | Contents                                               |
| -------- | ------------------------------------------------------ |
| `key`    | The key as a `NULL`-terminated string                  |
| `value`  | The `int8_t` value to persist                          |

Returns: a `Flash32Status` indicating success or failure

### `set_uint8`

`set_uint8` writes a key-`uint8_t` value pair to flash. If the
the pair already exists, its value is changed.

Parameters:

| Name     | Contents                                               |
| -------- | ------------------------------------------------------ |
| `key`    | The key as a `NULL`-terminated string                  |
| `value`  | The `uint8_t` value to persist                         |

Returns: a `Flash32Status` indicating success or failure

### `set_int16`

`set_int16` writes a key-`int16_t` value pair to flash. If the
the pair already exists, its value is changed.

Parameters:

| Name     | Contents                                               |
| -------- | ------------------------------------------------------ |
| `key`    | The key as a `NULL`-terminated string                  |
| `value`  | The `int16_t` value to persist                         |

Returns: a `Flash32Status` indicating success or failure

### `set_uint16`

`set_uint16` writes a key-`uint16_t` value pair to flash. If the
the pair already exists, its value is changed.

Parameters:

| Name     | Contents                                               |
| -------- | ------------------------------------------------------ |
| `key`    | The key as a `NULL`-terminated string                  |
| `value`  | The `uint16_t` value to persist                        |

Returns: a `Flash32Status` indicating success or failure

### `set_int32`

`set_int32` writes a key-`int32_t` value pair to flash. If the
the pair already exists, its value is changed.

Parameters:

| Name     | Contents                                               |
| -------- | ------------------------------------------------------ |
| `key`    | The key as a `NULL`-terminated string                  |
| `value`  | The `int32_t` value to persist                         |

Returns: a `Flash32Status` indicating success or failure

### `set_uint32`

`set_uint32` writes a key-`uint32_t` value pair to flash. If the
the pair already exists, its value is changed.

Parameters:

| Name     | Contents                                               |
| -------- | ------------------------------------------------------ |
| `key`    | The key as a `NULL`-terminated string                  |
| `value`  | The `uint32_t` value to persist                        |

Returns: a `Flash32Status` indicating success or failure

### `set_int64`

`set_int64` writes a key-`int64_t` value pair to flash. If the
the pair already exists, its value is changed.

Parameters:

| Name     | Contents                                               |
| -------- | ------------------------------------------------------ |
| `key`    | The key as a `NULL`-terminated string                  |
| `value`  | The `int64_t` value to persist                         |

Returns: a `Flash32Status` indicating success or failure

### `set_uint64`

`set_uint64` writes a key-`uint64_t` value pair to flash. If the
the pair already exists, its value is changed.

Parameters:

| Name     | Contents                                               |
| -------- | ------------------------------------------------------ |
| `key`    | The key as a `NULL`-terminated string                  |
| `value`  | The `uint64_t` value to persist                        |

Returns: a `Flash32Status` indicating success or failure

### `set_str()`

`set_str()` writes a key-string value to flash. If the pair exists,
its value is changed.

Parameters:

| Name     | Contents                                               |
| -------- | ------------------------------------------------------ |
| `key`    | The key as a `NULL`-terminated string                  |
| `value`  | The value to persist as a `NULL`-terminated string     |

Returns: a `Flash32Status` indicating success or failure

### `set_struct()`

`set_struct()` is a template function that writes a key-`struct` value
to flash. If the pair exits, its value is changed. Its template parameter,
`S`, is the `struct` type to write.

Parameters:

| Name     | Contents                                               |
| -------- | ------------------------------------------------------ |
| `key`    | The key as a `NULL`-terminated string                  |
| `value`  | The value to persist as a struct                       |

Returns: a `Flash32Status` indicating success or failure

`S` must comply with the requiremnts set forth under
[`get_struct()`](#get_struct)
above.

## `Flash32Iterator` Class

A `Flash32Iterator` traverses a flash namespace, retrieving
(key, value type) pairs one at a time. It offers a read-only API, so
it can traverse a
[`Flash32ReadOnlyNamespace`](#flash32readonlynamespace-class)
or a 
[`Flash32Namespace`](#flash32namespace-class).
A 'Flash32Iterator` can be configured to find all
key-value pairs, or just the key-value pairs having
a specified data type.

:arrow_forward: **Note**: the `Flash32Iterator` cannot read or write the value at
its current position. Use the access APIs described above.

A `Flash32Iterator` can only move forward, and becomes unusable
when its traversal completes. Be sure to discard it.

:arrow_forward: **Note**: best practice is to create instances on the stack.
Allocating instances via `new` is strongly discouraged.

### Constructor

The constructor configures a newly created `Flash32Iterator`

Parameters:

| Name              | Contents                                               |
| ----------------- | ------------------------------------------------------ |
| `flash_namespace` | Reference to the [`Flash32ReadOnlyNamespace`](#flash32readonlynamespace-class) or [`Flash32Namespace`](#flash32namespace-class) to traverse |
| `data_type`       | The desired [`nvs_type_t`](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/storage/nvs_flash.html#_CPPv410nvs_type_t) data type. Defaults to [`NVS_TYPE_ANY`](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/storage/nvs_flash.html#_CPPv4N10nvs_type_t12NVS_TYPE_ANYE) if omitted. |

A newly created `Flash32Iterator` is positioned *before* the first available key-value
pair. If the configured data type is omitted or explicitly set to
[`NVS_TYPE_ANY`](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/storage/nvs_flash.html#_CPPv4N10nvs_type_t12NVS_TYPE_ANYE),
the iterator retrieves *all* key-value pairs. Otherwise, it only retrieves
pairs whose value has the specified data type.

:arrow_forward: **Note**: `struct` values are stored as blobs, so you can either
retrieve all `struct` valued pairs or none at all.

### Destructor

The destructor releases all resources used to traverse the namespace.

### `next()`

`next()` traverses to the next available key-value pair, if any.

:arrow_forward: **Note**: be sure to invoke `open()` on the underlying
namespace before you invoke `next()`.

Returns: `true` if the next key-value was found, `false` otherwise

### `key()`

Returns: a pointer to the current entry's key. If the iterator is
not positioned on a key-value pair, the returned value is unspecified.

### `type()`

Returns: the
[`nvs_type_t`](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/storage/nvs_flash.html#_CPPv410nvs_type_t)
of the current entry. If the iterator is not positioned on
a key-value pair, the returned value is unspecified.

# C++ Style

The code is laid out as follows:

* Two space tab stops
* Indentation is via spaces, not tabs.
* Blocks are intented one tab stop.
* Statement continuations are indented two tab stops.
* Static fields are initialized when declared
* Constructors initialize all fields, even raw storage.
* Destructors de-initialize all fields.
* Class and enum names are in `UpperCamelCase`.
* Variable and function names are in `lower_snake_case`.
* `#define` constants and macros are in `UPPER_SNAKE_CASE`.
* Global variables are in `UpperCamelCase` like their counterparts
  in the Arduino libraries.

# Appendix I: Single Task 500/333 Milliseconds Blink

Here's a single tasking sketch that blinks the builtin LED at 500 milliseconds
and an outboard red LED at 333 milliseconds. Note that the logic is complexi
and difficult to understand.

```c++
#include "Arduino.h"

#define BUILTIN_LED 2
#define RED_LED 13

#define BUILTIN_DELAY 500
#define RED_DELAY 333

uint8_t builtin_level;
uint8_t red_level;

uint32_t builtin_delay;
uint32_t red_delay;

uint32_t current_delay;

uint8_t change_level(uint8_t level) {
  return level == LOW ? HIGH : LOW;
}

void set_leds(void) {
  digitalWrite(BUILTIN_LED, builtin_level);
  digitalWrite(RED_LED, red_level);
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  builtin_level = HIGH;
  red_level = HIGH;
  builtin_delay = BUILTIN_DELAY;
  red_delay = RED_DELAY;
  current_delay = RED_DELAY;
  set_leds();
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(current_delay));
  if (builtin_delay <= current_delay) {
    builtin_level = change_level(builtin_level);
    builtin_delay += BUILTIN_DELAY;
  }
  if (red_delay <= current_delay) {
    red_level = change_level(red_level);
    red_delay += RED_DELAY;
  }

  builtin_delay -= current_delay;
  red_delay -= current_delay;

  current_delay =
      builtin_delay <= red_delay
          ? builtin_delay
          : red_delay;
  set_leds();
}
```

# Appendex II: Dual Task 500/333 Milliseconds Blink

```c++
#include "Arduino.h"

#include "TaskAction.h"
#include "TaskWithAction.h"

#define BUILTIN_LED_PIN 2
#define RED_LED_PIN 13

class BlinkAction : public TaskAction {
  uint8_t pin_no;
  uint32_t delay_ms;

public:
  BlinkAction(
      uint8_t pin_no,
      uint32_t delay_ms) :
        pin_no(pin_no),
        delay_ms(delay_ms) {
    pinMode(pin_no, OUTPUT);
    digitalWrite(pin_no, LOW);
  }

  virtual ~BlinkAction() {}

  virtual void run(void) {
    for (;;) {
      digitalWrite(pin_no, HIGH);
      vTaskDelay(pdMS_TO_TICKS(delay_ms));
      digitalWrite(pin_no, LOW);
      vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
  }
};

static BlinkAction builtin_action(BUILTIN_LED_PIN, 500);
static uint8_t builtin_led_stack[2048];
static TaskWithAction builtin_task(
    "BuiltIn",
    2,
    &builtin_action,
    builtin_led_stack,
    sizeof(builtin_led_stack));

static BlinkAction red_action(RED_LED_PIN, 333);
static uint8_t red_led_stack[2048];
static TaskWithAction red_task(
    "Red",
    2,
    &red_action,
    red_led_stack,
    sizeof(red_led_stack));

void setup() {
  Serial.begin(115200);
  Serial.printf(
      "Dual task blink sketch compiled on %s at %s.\n",
      __DATE__,
      __TIME__);

  builtin_task.start();
  red_task.start();

  Serial.println("Setup completed.");
}

void loop() {
  vTaskDelay(portMAX_DELAY);
}
```

# References

[ESP32 FreeRTOS APIs](https://docs.espressif.com/projects/esp-idf/en/v4.3/esp32/api-reference/system/freertos.html)

[ESP32 FreeRTOS Error Codes Reference](https://docs.espressif.com/projects/esp-idf/en/v4.3/esp32/api-reference/error-codes.html)

[Sloeber IDE](https://eclipse.baeyens.it/index.shtml)

