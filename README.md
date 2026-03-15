# rca-cdp1802a-emu
Emulator and Cross-Assembler for the RCA CDP1802a chip

### About

This is a clock-cycle accurate hardware emulator and cross-assembler for the legacy RCA CDP1802a 8-bit microprocessor, made from scratch.

Utilizing an independent CPU and system bus design, this emulator is able to do systems of systems tasks (as long as it is multiple RCA 1802 chips communicating with each other since that is the only MPU implemented lol).

This project was mainly made for educational purposes since it forced us to strictly adhere to a datasheet and follow along to its architecture, write an independent and modularized design for having systems of systems, and use tools like flex and bison.

### building - UNIX/Linux

build docker container: `docker build build -t emu`

run docker container: `docker run --rm -it -v $(pwd):/root/env emu /bin/bash`

inside docker container run `make all`

