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

## Intended Use

RTOSAid is intended for use in open source ESP32 software projects that
are based on the Arduino API. It's intended for use cases whose complexity makes
them difficult to implement within the Arduino's `setup()`/`loop()`
framework.

## Intended Audience

RTOSAid is intended for experienced Arduino developers whose projects
have outgrown the Arduino framework.  Users should understand basic
logic design and be comfortable programming C++. Familiarity with
the ESP32 FreeRTOS APIs is helpful, but not required.

Since the library is (meant to be, at least) self-documenting, it
features long class, function, and variable names. We hope that this
doesn't burden our users.

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

Even though RTOSAid eliminates the need, it could be helpful to know a bit
about the following FreeRTOS features.

* Tasks
* Semaphores
* Queues

### Low Level ESP32 Features

Even though RTOSAid eliminates the need, it might be helpful to know a bit
about the following low level ESP32 capabilities.

* GPIO interrupts
* Timers
* EEPROM memory

### Hardware-Related Prerequisites

Users should also have a set of basic electronic tools and be able to
wire simple circuits from their descriptions.

Users should have the following parts on hand.

* Solderless breadboard
* Hookup wire or Dupont wire
* LEDs in assorted colors
* 510 Ohm, 5 percent, 1/4 Watt resistors
* A push button or switch

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

ELike most embedded system, the SP32 imposes the following constraints
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

Documentation does not cover implementation details, such as private fields
and methods. The code is extensively commented where necessary.

# Tasks

The task is the basic execution unit on the ESP32 and the central RTOSAid
class. Almost every other class in the library interacts with the `Task`
class.

Since Arduino style, `setup()`/`loop()`-based sketches, run in
tasks, already use them. The `Task` class merely makes this explicit.

## Overview

To to understand the need for multiple tasks, let's start with a sketch that
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
twice per second. We can split rhw 500 millisecond delays in
two and interleave code to blink the second LED.

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
we want to turn it off and on for 333 milliseconds instead. This is far more
challenging. Because 333, 3^2 * 37  and 500, 2^2 * 5^2
are relatively prime, we cannot use the simple method shown above.
The resulting sketch is too complex to show here so we've placed it in
Appendix I.

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
or explicitly shuts down its containing task. If the function returns, 
FreeRTOS restarts the application. The RTOSAid task shuts itself down if
the task method returns, but it is extremely poor practice to rely on this.

Even though the task function does not return, tasks can temporarily wait
for input, notifications, or other events. While watiing tasks remain in
memory, they do not consume CPU or other system resorces. Instead, FreeRTOS
runs other tasks, which implies that at least one task always be ready to
run. To enure this, FreeRTOS provides an idle task that is always ready
to run, and only runs when every other task is waiting.

FreeRTOS provides an idle task that maintains a watchdog timer. The idle task
runs when all other tasks are quiescent. and runs at
the lowest possible priority. It resets the timer whenever it runs, and if
the watchdog timer expires, FreeRTOS restarts the application. This means
that the idle task **must** run periodically.

FreeRTOS is a cooperative multitasking system, which means that tasks must
periodically cede control. They can do this in several ways:

1. Waiting, i.e. invoking 
   [`vTaskDelay()`](https://docs.espressif.com/projects/esp-idf/en/v5.0/esp32/api-reference/system/freertos.html#_CPPv410vTaskDelayK10TickType_t), 
   [`delay()`](https://www.arduino.cc/reference/en/language/functions/time/delay/),
   or a similar function
2. Being suspended
3. Waiting for a notification
4. Waiting on a queue or timer
5. Waiting on a hardware event such as a GPIO level change, or similar event

Tasks can be in the following states

* Stopped: unitialized, non-existant
* Running: actively running
* Suspended: quiescent, waiting to be resumed
* Waiting: waiting for a notification or an event

In addition, tasks can be waiting on a semaphore or queue, or for a hardware
event.

Waiting tasks do absolutely nothing and impose no load on the CPU.

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

The `TaskWithAction` class manages an ESP32 FreeRTOS task. Application
code can create, start, stop, suspend, resume, and notify tasks. Task logic can
also wait for notification and yield to higher priority tasks.

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

**Parameters:**

* `name` - short, descriptive, `NULL`-terminated task name. This is
  mainly used to facilitate debugging, and should be unique.
* `priority` - task priority, a value between 1 (lowest) to 24 (highest).
  The task scheduler favors higher value priorities
* `action` -  provides the task's runtime logic, the program that the
  task runs
* `stack` - storage that the task uses to support function call and return,
  and to hold automatic variables. This argument is typeless, meaning that
  it can have any time, however it is strongly recommend that the caller
  provide a `uint8_t` (i.e. byte) array.
* `stack_size` - the length of the `stack` arguments in bytes.


:arrow_forward: **Note:** determining the proper stack size is more an art than a science.
Simple tasks that use a few small automatic variables and do not have
deeply nested function calls require small stacks, while tasks that requre
extensive storage or deeply nested function calls require more. 4096 bytes is
a good starting sizes, though the very simplest tasks can get by with
2048.


### Destructor

Stops a task if it is running.

:arrow_forward: **Note:** tasks instances are seldom deleted, so the destructor is implemented
for the sake of completeness. Deleting a running task is extremely poor
practice. Prefer invoking `stop()` first.

### notify

Resumes a task that is waiting for a notification. Does nothing if the
task is suspended or not waiting for a notification. This method is for
use by application code. ISR code **must** invoke `notify_from_ISR()`
instead.

### notify_from_ISR

Resumes a task that is waiting for notification. Does nothing if the
task is suspended or not waiting for a notification. This method is for
use by interupt service routines (ISRs). Application code **must**
invoke `notify()` instead.

### resume

Resumes a suspended task. Does nothing if the task is running. To avoid
unspecified and undesirable behavior, be sure to invoke `start` before
invoking this method.

:arrow_forward: **Note:** suspended tasks do not respond to notifications.

### start

Starts a task. The task must be stopped. Invoking `start()` on a running
task will cause unspecified behavior. The task **must** be started
before any other methods can be invoked.

### stop

Stops and destroys a task. The task can be restarted with `start()`.


:warning: **Warning:** `stop()` will destroy a task in any state. Improper use can leave the system
in an upspecified state.

### suspend

Suspends a running task. Does nothing if the task is already suspended.
The `resume()` method (see above) resumes suspended tasks.



## `TaskAction` Class

Base class whose subclasses provide task logic. To use it:

1. Declare a concrete class that inherits `TaskAction` publicly.
   The concrete class **must** declare a `run()` function which
   must **not** be pure, which means that `= 0` must **not** follow
   its declaration..
2. Implement that logic loop in the class's `run()` function.

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

