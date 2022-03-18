Carbon1 Software
================

This repository contains driver to build minimal bare-metal software for the
Carbon1 RISC-V tape-out.

Installation
############

Download a pre-build risc-v gcc.

.. code-block:: text

    wget https://github.com/stnolting/riscv-gcc-prebuilt/releases/download/rv32i-2.0.0/riscv32-unknown-elf.gcc-10.2.0.rv32i.ilp32.newlib.tar.gz
    mkdir riscv32-unknown-elf
    tar -xvf riscv32-unknown-elf.gcc-10.2.0.rv32i.ilp32.newlib.tar.gz -C riscv32-unknown-elf/
    rm riscv32-unknown-elf.gcc-10.2.0.rv32i.ilp32.newlib.tar.gz

Compile
#######

A makefile exists in the root directory to build the kernel image.

.. code-block:: text

    make

Structure
#########

``driver/`` and ``include/`` contain driver for all IP.

``carbon/`` contains the start-up code and main loop.

Flash
#####

Carbon1 has a read-only SPI XIP controller and therefore does not support to
programm the external memory.

TBD

License
#######

Copyright (c) 2022 Daniel Schultz. Released under the `license`_.

.. _license: COPYING.MIT
