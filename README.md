# rca-cdp1802a-emu
## Emulator and Cross-Assembler for the RCA CDP1802a chip

### About

This is a clock-cycle accurate hardware emulator and cross-assembler for the legacy RCA CDP1802a 8-bit microprocessor, made from scratch. The goal of this project is to not only emulate a very unique and old chip architecture, but also allow the user to have the ability to design and create embedded systems with multiple moving parts and CPUs that communicate with each other.

Features:
 - Cross-assembler for  RCA 1802 assembly
 - Emulator for the RCA 1802 chip
 - Independent system bus, MMIO, and (future) PMIO design
 - Architecture supporting independent, interacting parallel systems
 - YAML parsing for designing and testing custom embedded systems

### Usage

You can always run `-h` on the binary to get help on args. There are also examples in `<root>/demos/` on how it should look like. 

#### Assembling

You can utilize the `-a` flag to assemble a src assembly file, and then use `-o` to specify output file path. By default the output will be `./a.out`.

#### Emulating

There are two ways to emulate, either by just running a binary blob outputted by the assembler (`-e`), or by designing your own embedded system (`-y`, read below for instructions).

The `-e` flag will take a singular binary file as an argument, it will then map out all of memory space (`0x0000-0xffff`), and then put the binary on that memory address space.

The `-y` flag will take a YAML file as an argument, and then based on that file, it will build a system and then run all CPUs at addr 0. See below for how to design a system.

#### Designing a system

The way this YAML parser is designed, you only have three things to worry about:
 - `name` - this is the type descriptor
 - `value` - this is the value
 - `slaves` - all children for this object, make sure each child starts with a ` - `

You will first need one ROOT parent to resemble the system, it doesn't matter what it is called. Its children will be each system or a peripheral that does not need to be part of a devices memory mapping. Supported devices:
 - `cpu`

Each `cpu object`'s value will be the label given to the CPU for the debugger. From there its children are going to be:
 - `sysbus` - the processor's system bus configuration for MMIO
 - `ports` - configure where ports are connected to for PMIO
 - `EF[1-4]` - the processor's EF lines config (commonly used as IRQ lines)

In the `system bus (sysbus)` config, you will need to configure a few things in its children:
 - `decoder_line_count` - this specifies how many of the higher order address pins will be used for a chip select decoder
 - `devices` - the value is the file path used in memory address space, the children for this will be the devices connected via MMIO
   - each `device` is going to need at least two children: `decoder_line` (which decoder line the CS is hooked up to), and `address_lines` (how many low order address lines go into the device)
   - here is a list of all devices supported for MMIO:
     - `memory`
     - `PIT` (in the future) - the intel 8253 programmable interval timer

### Building - UNIX/Linux

build docker container: `docker build build -t emu`

run docker container: `docker run --rm -it -v $(pwd):/root/env emu /bin/bash`

inside docker container run `make all`

