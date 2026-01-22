# rca-cdp1802a-emu
Emulator and Cross-Assembler for the RCA CDP1802a chip

### building - UNIX/Linux

build docker container: `docker build build -t emu`

run docker container: `docker run --rm -it -v $(pwd):/root/env emu /bin/bash`

inside docker container run `make all`

