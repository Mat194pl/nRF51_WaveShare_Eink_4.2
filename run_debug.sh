#!/bin/bash

~/gcc-arm-none-eabi-7-2018-q2-update/bin/arm-none-eabi-gdb ./_build/nrf51_test.out --tui -ex 'target remote localhost:2331'

