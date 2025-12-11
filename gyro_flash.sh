#!/bin/sh
openocd -f openocd.cfg \
  -c "init" \
  -c "reset halt" \
  -c "program build/gyrocube.hex verify" \
  -c "reset run" \
  -c "shutdown"
