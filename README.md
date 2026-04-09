# rca-cdp1802a-emu
## Emulator and Cross-Assembler for the RCA CDP1802a chip

### About

This is a clock-cycle accurate hardware emulator and cross-assembler for the legacy RCA CDP1802a 8-bit microprocessor, made from scratch. The goal of this project is to not only emulate a very unique and old chip architecture, but also allow the user to have the ability to design and create embedded systems with multiple moving parts and CPUs that communicate with each other.

### building - UNIX/Linux

build docker container: `docker build build -t emu`

run docker container: `docker run --rm -it -v $(pwd):/root/env emu /bin/bash`

inside docker container run `make all`

