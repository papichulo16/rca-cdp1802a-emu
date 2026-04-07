# rca-cdp1802a-emu
## Emulator and Cross-Assembler for the RCA CDP1802a chip

### Authors: Luis Abraham, Dominick Morales, Dylin Irons, Giovanni Williams

### About

This is a clock-cycle accurate hardware emulator and cross-assembler for the legacy RCA CDP1802a 8-bit microprocessor, made from scratch. The goal of this project is to not only emulate a very unique and old chip architecture, but also allow the user to have the ability to design and create embedded systems with multiple moving parts and CPUs that communicate with each other.

Utilizing an independent CPU and system bus design, this emulator is able to do systems of systems tasks (as long as it is multiple RCA 1802 chips communicating with each other since that is the only MPU implemented lol).

This project was mainly made for educational purposes since it forced us to strictly adhere to a datasheet and follow along to its architecture, write an independent and modularized design for having systems of systems, and use tools like flex and bison.

### building - UNIX/Linux

build docker container: `docker build build -t emu`

run docker container: `docker run --rm -it -v $(pwd):/root/env emu /bin/bash`

inside docker container run `make all`

