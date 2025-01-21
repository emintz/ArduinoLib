# Blink Task Action Use Example

`BlinkTaskAction.ino` shows how to use the `BlinkAction` class.

## Overview

The sketch works as follows.

1. The static `BlinkAction` `action` variable isw configured to blink five times, 50 milliseconds
   on, 100 milliseconds off, with a 500 millisecond wait between groups of five blinks.
2. The static `TaskWithActionH` `task` variable is configured to 
3. `setup()` starts `action`.
4. `loop()` has nothing to do, so it just waits.

## Using the Sketch

See the instructions for the Blinky example.
