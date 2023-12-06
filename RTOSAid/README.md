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
when I started to develop ESP32 software. I hope that the
library provides good service and simplifies your projects.

At the time of this writing, late 2023, the software is new,
possibly incomplete, and probably contains defects. I would
be grateful if you would draw my attention to either or for
any feedback you'd care to provide.

In the meantime, enjoy!

## Intended Use

RTOSAid is intended for use in open source ESP32 software projects that
are based on the Arduino API Its intended for use whose complexity makes
them difficult to implement within the Arduino's `setup()`/`loop()`
framework.

## What You Need To Know

RTOSAid is intended for experienced Arduino developers whose projects
have outgrown the Arduino framework.  Users should understand basic
logic design and be comfortable programming C++. Familiarity with
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
or a similar, more powerful IDE.

### Logic Design-Related Prerequisites

Users should understand and be comfortable using

* Breadboards
* LEDs
* Pullup resistors
* Active low logic
* ESP32 GPIO pin functions

Since the documentation does not include diagrams,
users need to be able to assemble simple circuits
from their description.

### C++-Related Prerequisites

Users should understand and be comfortable using

* C++ classes -- constructors, destructors, functions, and fields
* Virtual vs. non-virtual functions
* Virtual destructors
* Pure virtual functions
* Class templates
* Enumerations, both `enum` and `enum class`
* Storage classes: static, automatic, and dynamic

Explanation of C++ and its many features are beyond the scope of this
document. It does contain extensive examples and we hope that you find them
helpful and informative.

### FreeRTOS on the ESP32

Even though RTOSAid minimizes the need, it could be helpful to know a bit
about the following FreeRTOS features.

* Tasks
* Semaphores
* Queues

### Low Level ESP32 Features

#### Useful Background

Even though RTOSAid to eliminate the need, it might be helpful to know a bit
about the following low level ESP32 capabilities.

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

While programs can check signals periodically, doing so
has significant disadvantages.

* The checks occupy the CPU. If the application has to check
  for input continuously, the resulting CPU load can slou the
  application unacceptably.
* It is possible for the application to miss available data.

Interrupts provide a way to detect events like these
with little CPU involvement. To use them,

1. Write an interrupt service routing (ISR) that responds
   to the expected event. ISRs need to complete quickly,
   so they usually notify the application that the event
   occurred. See below for details.
2. Implement application code to process the event.
3. Enable the ISR during application startup.

The library provides examples.

ISR implementations must

* Have no parameters
* Return nothing
* Complete quickly, the quicker the better. ISRs block their
  containing applications.
* **Never ever block**
* Invoke **only** ISR-compatible APIs. Note that most widely used
  library capabilities are **not** usable from ISRs.

Unless otherwise noted, RTOSAid functions **cannot** be used in ISRs. The
names of compatible functions usually end with `from_isr`, as in the
`TaskWithAction`'s (see below) `notify_from_isr()` function.

Pro tips:

* Implement as much interrupt processing in application code.
* Do no I/O in ISRs. `Serial`, for example, is right out, as are
  other streams.
  [SPI](https://www.arduino.cc/reference/en/language/functions/communication/spi/),
  [Wire](https://www.arduino.cc/reference/en/language/functions/communication/wire/),
  WiFi, and other external communication are also out of bounds, as are
  the Advanced I/O Arduino functions.
* **Do not block, ever.** `vTaskDelay()`, `delay()`, and `delayMicroseconds()`
  and the like are incompatible with interrupt handling. Don't even think about
  invoking them.
  
The ESP32 chip contains several memory types, including static RAM (SRAM).
SRAM is the fastest kind, but even modest amounts consume a lot of chip
area. Unless otherwise noted, ISRs **must** reside in SRAM.

The `IRAM_ATTR` attribute, defined in `Arduino.h`, forces a function into
SRAM. It applies to free standing functions and to functions in classes.
Here's an example of a free-standing function.

```
#include "Arduino.h"

// ...

void IRAM_ATTR isr_handler(void) {
  // ...
}
```

ISRs in classes must be `static` in the class definition.

```
#include "Arduino.h"

class ShortLongRedGreenEventHandler {

  // Fields and other stuff

  // Interrupt service routines can be private, if desired.
  static void IRAM_ATTR take_interrupt(void);

  // ...
}
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

* 1 ESP32 development (a.k.a. breakout) board. Mine has a generic ESP32 S2.
* 1 solderless breadboard
* Hookup wire or Dupont cabkes. 24 guage solid wire works well in solderless
  breadboards.
* LEDs in assorted colors, 1 each red, yellow, green, blue, and white
* 5 510 Ohm , 5 percent (green, brown, black, gold), 1/4 Watt resistors
* 1 push button switch or equivalent

#### Wiring

The test system is wired. Note that LEDs are wired by connecting

1. A GPIO pin to one lead of a 510 Ohm resistor
2. The other resistor lead to the long (positive) LED lead
3. The short (negative) LED pin to ground

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
If your board's built in LED is wired differently, you will have to change the
sketch accordingly. If your board does not have a builtin LED, you will
need to add one and change the example sketches as needed.

## RTOSAid Features

RTOSAid supports the following FreeRTOS features.

* Tasks, the basic unit of execution under FreeRTOS. 
* Mutual Exclusion Semaphores, a.k.a. "Mutexes", used to prevent tasks from
  interfering with each other.
* FIFO queues, which carry messages between tasks
* High resolution timers, which run specified logic after a specified
  delay
* GPIO change detectors, which run specified logic when GPIO inputs change

# Design Philosophy

Like most embedded system, the SP32 imposes the following constraints
on its software.

* Resources: the ESP32 runs slower and has far less memory than
  regular computers.
* Operation: production ESP32 software must run indefinitely without
  uogrades or user intervention.
* Criticality: embedded software failures are far more likely to
  threaten life and property than failing application software.
* Resonsiveness: most embedded systems must respond within strict
  deadlines, it can be vital for embedded software.
* Expected service life: embedded software can remain in service for
  decades without support or upgrade.

To comply with the foregoing requirements, RTOSAid

* Minimizes heap use, allocating storage statically instead of via
 `new` or `malloc`. Users must provide storage to some classes.
* Short functions: logic is short, simple, and fits on a single
  printed page.
* Does not recurse to minimize the chance of stack overflow
* Does one thing well: RTOSAid classes have few, if any options. For example,
  the RTOSAid messasge queue is FIFO even though the FreeRTOS also supports
  LIFO. If LIFO or deques are needed, we will implement them in separate
  classes..
* Type safety: APIs are designed to minimize the risk of type errors,
  to ensure that functions only receive values of expected types. 

In the author's experience, the presence of "do everything" classes
limits a library's usefulness. Designers cannot anticipate their
users' future needs, so over time, large, multifunction classes lose
their usefulness. Hence, each class in the RTOSAid library performs
one and only one a basic function. The classes are designed for
composition, so users can mix and match them as use cases require.

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
class. FreeRTOS is _multitasking_ meaning that many tasks can run
simultaneously. Tasks have priorities, and RTOS runs the highest
priority available task.

Since Arduino style, `setup()`/`loop()`-based sketches, run in
tasks, your existing sketches already use them. 

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

Now let's blink two LEDs, the builtin as above, and another at
twice per second. We can split the 500 millisecond delays into
two 250 millisecond delays and interleave the code tthat blinks
the second LED.

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

Suppose that instead of turning the second LED on and off for 250 milliseconds,
we want to turn it off and on for 333 milliseconds instead, which is far more
challenging. Unlike 500 and 250, whose greatest common factor is 250,
333, 3^2 * 37  and 500, 2^2 * 5^2
are relatively prime so we cannot use the simple method shown above.
The resulting sketch is too complex to show here so we've placed it in
[Appendix I](#appendix-i-single-task-500333-milliseconds-blink).

Implementation would be simple if we could run two independent loops
like this.

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

Tasks let us do just that provide we're willing to write some simple
boilerplate.

## Details

Task code resides in a function whose logic either runs in an endless loop
or, if it needs to stop, explicitly shuts itself now. If the function returns, 
FreeRTOS restarts the application. The RTOSAid task shuts itself down if
the task method returns, but it is extremely poor practice to rely on this.

Even though the task function does not return, tasks can wait
for input, notifications, or other events. Waiting tasks do not consume CPU
or other system resorces. Instead, FreeRTOS runs other tasks. To ensure
that it always has a task to run, FreeRTOS provides idle task that it
runs when no other task is available.

The idle task maintains a watchdog timer that it resets whenever
it runs.If the watchdog timer expires, FreeRTOS assumes that the
system is locked and reboots. To prevent this,  the idle task
**must** run periodically.

FreeRTOS is a preemptive multitasking system, which means that it
preempts a task when a higher priority task is ready to run. Since
the background task cannot preempt an application task,
they must volunteer for preemption from time to time so
that the idle task resets the watchdog timer. A task becomes
not ready when it

1. Pauses by invoking 
   [`vTaskDelay()`](https://docs.espressif.com/projects/esp-idf/en/v5.0/esp32/api-reference/system/freertos.html#_CPPv410vTaskDelayK10TickType_t), 
   [`delay()`](https://www.arduino.cc/reference/en/language/functions/time/delay/),
   or a similar function
2. Suspends
3. Waits for a notification from another task
4. Waits for a message to arrive
5. Waots to acquire a semaphore
6. Waits for a timer to expire
7. Waits on a hardware event such as a GPIO level change to occur

A task can be in the following states

* Ready: ready to run
* Running: actively running
* Suspended: quiescent, waiting to be resumed
* Waiting: waiting for a notification or an event

A nonrunning task does absolutely nothing and imposes no load on the CPU.

:warning: **Warning:** when a task is waiting, be sure to wake it properly.
For example, application code must not attempt to resume a task that is
waiting for a timer to expire.

## Creating a Task

To create a task, users must provide

1. A stack, storage that the task uses to manage function invocations and to
   hold automatic variables.
2. Task logic, code that the tasks runs. As shown below, the task logic
   resides in a `TaskAction` subclass.
3. A `TaskWithAction` instance, which starts and manages the task.
3. A priority, an unsigned integer between 0 and 24

:warning: **Warning**: the system idle task runs at priority 0. User tasks
should run at priority 1 or higher.

The `BlinkAction` class shown below provides the required logic.

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

Given the `BlinkAction`, we can blink as many LEDs as we please. Here's the
builtin LED configuration.


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
is more than enough.

We also create a task to blink the red LED.

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
[`notify_from_ISR()`](#notify_from_isr)
instead.

### notify_from_ISR

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
The `resume()` method (see above) resumes suspended tasks.

:arrow_forward: **Note**: suspending a suspended task has no effect. No matter
how many times the user invols `suspend()` on a suspended task, the next call
to [`resume()`](#resume) will reactivate it.

:arrow_forward: **Note:** suspended tasks do not respond to notifications.

## `TaskAction` Class

Base class whose subclasses provide task logic. To use it:

1. Declare a concrete class that inherits `TaskAction` publicly.
   The concrete class **must** declare a `run()` function which
   must **not** be pure, which means that `= 0` must **not** follow
   its declaration.
2. Implement that logic loop in the class's `run()` function. Implementation
   is mandatory.

`TaskAction` has no public functions. All of its functions are `protected`
that are reserved for use within inheriting classes.

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

### run

A place holder for the task logic. `TaskAction` does **not** implement
this function. Classes that inherit `TaskAction` provide its logic.
This is a hard requirement if, as is usually the case, an application needs
to create instances of the inheriting class.

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

# Pull Queues

Applications use pull queues, which transmit messages in first in, first out
(FIFO) order, to send data between tasks. Typically, a receiving task removes
messages from the queue and responds to them, while any number of sending
tasks add messages to the queue.

## Overview

Queues provide reliable transport between tasks. Their advantages include:

1. Asynchronous send and receive: queues automatically interleave sending
   and receiving, meaning that applications don't have to serialize
   queue mutations.
2. Overlap: sending and receiving take place simultaneously (nominally, at
   least) without requiring application-level logic.
3. Scalability: developers can add queue capacity by enlarging the queue's
   buffer, which is extremely simple to do.
4. Load leveling: queues absorb load spikesd by holding messages for later
   processing. This simplifies background processing, where the queue holds
   messages for processing by a low priority task.
5. Fan in: any number of tasks can add messages to a queue, simplifying
   the implementation of client/server architectures
6. Multi-task and multi-thread safety: multiple tasks can send and receive
   queue entries simultaneously without colliding, which makes queues the
   preferred inter-task communication mechanism.

The only cost is storage, the memory required to store messages for later
processing, and the possibility of queue overflow should the queue run out
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

Users access queues via the `PullQueueT` class, though the `BasePullQueue`
class contributes some of its public API. 

The `PullQueueT` class is templated by the message type, which users must
provide in a queue declaration. In principal, `PullQueueT` defines a unique
class for each message type.

:warning: specify a message as plain, C-style `struct`, a structure (as opposed
to a class) without functions, including constructors and destructor, as in


```c++
struct LedCommand {
  uint16_t step_time;
  uint16_t inter_cycle_time;
  uint16_t repititions;
```

Attempting to send a `class` is risky and likely to fail.
## Creating a Queue

To create a queue, users must provide

1. Its message type
2. Storage for enqueued messages
3. Storage capacity, the maximum number of messages that the queue can hold

Users supply an array of `T` to hold enqueued messages. The resulting
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
| `queue_lendgh`  | Number of elements in the `queue_storage` array           |

The constructor declares a queue. The created queue is not running. Applications
must start it to make it usable.

### Destructor

The destructor stops the queue if it is running, and has no effect otherwise.

### peek_message (single parameter)

```c++
peek_message(
    T *message);
```

Receive a message from the queue, waiting forever for a message to arrive
without removing the message from the queue. The message remains available
for reading.

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
arrive, it remains available for reading. If a message arrives, the function
returns immediately.

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
message arrives, the function returns immediately.


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

# Mutual Exclusion Semaphore (Mutex)

Mutual Exclusion Semaphores, a.k.a. Mutexes, prevent multiple
tasks from colliding. When used properly, A Mutex ensures that logic
runs only in a single task by forcing other tasks to wait until the current
task finishes.

## Overview

A mutex can be in one of two states: unlocked and locked. Any task can
attempt to lock the mutex, with two possible results:

If the mutex is currently unlocked, the attempt succeeds. The requesting
task holds the lock and keeps running. When the task no longer needs to
hold the lock, it must surrender it, unlocking the mutex.

If, on the other hand, a task attempts to lock a mutex that is already
locked, it blocks until the lock becomes available. It then acquires the lock
as described immediately above, surrendering it, as always, when it no longer
needs the lock.

:arrow_forward: **Note**: tasks should unlock semaphores as quickly as possible,
holding the mutex lock for the shortest possible time. Holding locks too long
can degrade performance and could trip the watchdog timer and reboot the
system.

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

:warning: **Warning**: a task holding a lock **MUST** release it
promptly. Holding a lock forever can hang the system, triggering the
watchdog timer and rebooting the system.

:warning: **Warning**: release locked mutexes promptly. Holding
a lock too long can hang the system, triggering the watchdog timer
and causing a reboot.

:warning: **Warning**: a task **MUST** lock a mutex **at most once**. The `Mutex`
class does **not** support recursive locks. Attempting to relock a
`Mutex` will cause unspecified and undesirable behavior.

The API is designed to assure that locks are released eventually. Users
are responsible for ensuring prompt release.

## `Mutex` Class

The `Mutex` class maintain a list of tasks that are waiting for its lock,
and doles out its lock to one task at a time.

:arrow_forward: **Note**: applications **MUST** lock a `Mutex` with a `MutexLock`. 
`Mutex` does not provide an application-visible locking API.

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

### Example of Use

Here's how to use a `MutexLock` in a class function. Assume that
the class contains a `Mutex` member named `mutex`.

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
should always invoke this after constructing a `MutexLock`.

# Function Classes

A function class is a class that acts as a stand in for a function.

## Overview

Typically, a function class has a single pure virtual function where
subclasses place the function's logic. We have already seen an example
in the `TaskAction` described previously.

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
in microseconds.

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
library's `Serial` component. It installs the FreeRTOS low
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
sensitve to noise, the `GpioDebouncer` does filters out switch
spurrious makes and breaks.

## Background

Mechanical switches, including our beloved push buttons, are subject to
[contact bounce](https://en.wikipedia.org/wiki/Switch#Contact_bounce)
(a.k.a. "chatter"). Instead of generating clean makes (openings) and
breaks (closures), the switch generates sequences of makes and breaks
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
activity.


```
 High (Open)   --> -------+  +---+      +--------+
                          |  |   |      |        |
                          |  |   |      |        |
 Low (Closed) -->         +--+   +------+        +----------------------- ...
                          |<=........|
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
[`VoidFunction`](#voidfunction-class), and
[`GpioChangeDetector`](#gpiochangedetector-class)
and a few lines of custom logic to detect switch activity,
remove the ensuing chatter, and apply a user-provided
[`VoidFunction`](#voidfunction-class). The few lines of
custom logic is reside in 
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
Since `GpioDebounceFunction` runs as an ISR, it notifies the task by
invoking its `notify_from_isr()` function.

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
timer's delay runs to completion, and the timer applies the user-provided
[`VoidFunction`](#voidfunction-class).


## `GpioDebouncer` Class

`GpioDebouncer` packages the foregoing components in an easily
used class. In additiion to solving a knotty problem, the class
also shows how to aggregate RTOSAid library components into
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

:arrow_forward: **Note**: as stated above, violent acceleration can
cause a `GpioDebouncer` to detect a non-existant change, e.g a change
from closed to closed. If your project is subject to extreme
mechanical shock, be sure th check that the switch changed.

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

[ESP32 FRtos Error Codes Reference](https://docs.espressif.com/projects/esp-idf/en/v4.3/esp32/api-reference/error-codes.html)

[Sloeber IDE](https://eclipse.baeyens.it/index.shtml)

