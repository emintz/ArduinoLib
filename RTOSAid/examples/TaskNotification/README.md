# Inter-Task Notification Example

An example of inter-task notification, which contains

1. A task that receives a notification, then changes a specified GPIO level
   from LOW to HIGH or vice versa. The controlling stack configures the
   receiver to blink the ESP32 development board's built-in LED.
2. A second task that periodically notifies a target task.
3. A sketch that creates and runs the receiving and sending
   tasks.

The example also demonstrates priority use. The receiving
task runs at priority 3, the highest priority in the system.
When it is notified, it preempts its notifier and immediately
changes its target GPIO level. The sending task runs at
priority 2, lower than the receiving task, and higher than
the sketch, which runs at priority 1.

The sketch's `loop()` function has nothing to do. To avoid
unnecessary CPU activity it waits for the longest supported
interval, just over 49 days. When the interval expires, it
simply waits again, so its CPU requirements are negligible.

When the task runs, the built-in LED pin should blinks once
per second.

