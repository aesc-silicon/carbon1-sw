Carbon1 Software
================

This repository contains drivers to build a minimal bare-metal software for the
Carbon1 RISC-V tape-out.

About
#####

Carbon1 is an open-source RISC-V MCU written in SpinalHDL and taped out with Cadence for
IHP's 130nm SG13S semiconductor process. The design uses a VexRiscv RV32IMC CPU with standard
low-speed peripherals (UART, GPIO, I2C, SPI) and system (MTIMER, PLIC) components. Moreover, it has
a 512 Byte on-chip RAM, which provides space for the stack and data segments, and fetches
instructions from an external SPI NOR device through the SPI controller.

The chip was developed with the `Elements SDK`_, an open-source hard-/software co-design tool to
develop, debug and maintain all stacks of an open-source System on Chip (SoC). Additionally, the
project can generate all necessary files for the ASIC flow and allows the reproduction of the chip.

.. _Elements SDK: https://github.com/aesc-silicon/elements-sdk

Known Issues
************

* I2C: Used tri-state IOs with pull-ups instead of open-drain. The potocol looks well with an oscilloscopei, but it is active high and therefore not I2C conform.

Approved Components
*******************

* VexRiscv RV32IMC
* SPI Controller XIP
* On-Chip RAM
* UART TX+RX
* UART IRQ on RX
* PLIC
* GPIO
* MTIMER

Development Board
*****************

X2 / JTAG
---------

X2 has an ARM 20-Pin JTAG interface pinout; therefore, all compatible JTAG debugger can be used.

X4 / UART+I2C
-------------

Use a jumper to connect pin 5 with 7 (UART CTS/RTS) to bypass the flow control when the UART-USB converter only has a TX and RX pin.

+-------------+-----+-----+-------------+
| Description | Pin | Pin | Description |
+-------------+-----+-----+-------------+
| UART TXD    |  1  |  2  | VCC 3.3V    |
+-------------+-----+-----+-------------+
| UART RXD    |  3  |  4  | VCC 3.3V    |
+-------------+-----+-----+-------------+
| UART CTS    |  5  |  6  | I2C SDA     |
+-------------+-----+-----+-------------+
| UART RTS    |  7  |  8  | I2C SCL     |
+-------------+-----+-----+-------------+
| GND         |  9  | 10  | GND         |
+-------------+-----+-----+-------------+

X5 / GPIO
---------

+-------------+-----+-----+-------------+
| Description | Pin | Pin | Description |
+-------------+-----+-----+-------------+
| GPIO4       |  1  |  2  | GPIO0       |
+-------------+-----+-----+-------------+
| GPIO5       |  3  |  4  | GPIO1       |
+-------------+-----+-----+-------------+
| GPIO6       |  5  |  6  | GPIO2       |
+-------------+-----+-----+-------------+
| GND         |  7  |  8  | GPOP3       |
+-------------+-----+-----+-------------+
| GND         |  9  | 10  | GND         |
+-------------+-----+-----+-------------+


X6 / SPI XIP
------------

+--------------+-----+-----+-------------+
| Description  | Pin | Pin | Description |
+--------------+-----+-----+-------------+
| SPI XIP nCS  |  1  |  2  | VCC 3.3V    |
+--------------+-----+-----+-------------+
| SPI XIP CLK  |  3  |  4  | VCC 3.3V    |
+--------------+-----+-----+-------------+
| SPI XIP MOSI |  5  |  6  | TP28        |
+--------------+-----+-----+-------------+
| SPI XIP MISO |  7  |  8  | nRESET      |
+--------------+-----+-----+-------------+
| GND          |  9  | 10  | GND         |
+--------------+-----+-----+-------------+

Installation
############

Download a pre-build risc-v gcc.

.. code-block:: text

    wget https://github.com/stnolting/riscv-gcc-prebuilt/releases/download/rv32i-2.0.0/riscv32-unknown-elf.gcc-10.2.0.rv32i.ilp32.newlib.tar.gz
    mkdir riscv32-unknown-elf
    tar -xvf riscv32-unknown-elf.gcc-10.2.0.rv32i.ilp32.newlib.tar.gz -C riscv32-unknown-elf/
    rm riscv32-unknown-elf.gcc-10.2.0.rv32i.ilp32.newlib.tar.gz

Install OpenOCD with VexRiscv support (when debugging is required).

.. code-block:: test

    git clone git@github.com:aesc-silicon/elements-openocd.git
    cd elements-openocd
    ./bootstrap && ./configure && make -j `nproc`
    cd ../

Compile
#######

A makefile exists in the root directory to build the kernel image.

.. code-block:: text

    make

Structure
*********

``driver/`` and ``include/`` contain driver for all IPs.

``carbon/`` contains the start-up code and main loop.

Flash
#####

Carbon1 has a read-only SPI XIP controller and does not support programming the external memory.

Bus Pirate
**********

Connect a `Bus Pirate`_ with all four SPI XIP pins on pin header X6 and execute the corresponding
make rule.

.. code-block:: text

    make flash_buspirate

``flashrom`` first reads the entire SPI flash content. Unfortunately, the SPI NOR has a size of
64MB and the flash procedure takes 20 minutes.

.. _Bus Pirate: http://dangerousprototypes.com/docs/Bus_Pirate

Sponsores
#########

This project included support from other people whom I would like to thank::

* Steffen Reith (Hochschule RheinMain)
* Matthias Harter (Hochschule RheinMain)
* Andreas Grothe (Hochschule RheinMain)
* Elvira Liandres (Fraunhofer IIS)

This tape-out was funded by Hochschule RheinMain and PHYTEC Messtechnik GmbH sponsored the circuit boards.

License
#######

Copyright (c) 2022 Daniel Schultz. Released under the `license`_.

.. _license: COPYING.MIT
