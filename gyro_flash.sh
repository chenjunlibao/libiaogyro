#!/bin/sh
openocd -f openocd.cfg -c "init; reset halt; program build/gyrocube.hex verify reset; shutdown"
