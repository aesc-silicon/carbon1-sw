#!/bin/bash

openocd_riscv/src/openocd -c "set HYDROGEN_CPU0_YAML configs/VexRiscv.yaml" -f openocd_riscv/tcl/interface/jlink.cfg -f configs/carbon.cfg
