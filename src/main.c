#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include "hw.h"
#include "asm.h"

char* binary;
char* outf = "a.out";
char* inf;
char* runf;
bool assemble = false;
bool emulate = false;

void assembler() {

  asm_main(inf);

}

void emulator(){

  cdp1802a_chip_t* chip = malloc(sizeof(cdp1802a_chip_t));

  hw_sys_init(chip);
  hw_sys_run(chip);

  free(chip);

}

void usage() {

  printf("This is a clock-cycle accurate hardware emulator and cross-assembler for the legacy RCA CDP1802a 8-bit microprocessor, made from scratch.\n\n");
  printf("Authors: Luis Abraham, Dominick Morales, Dylin Irons, Giovanni Williams.\n\n");

  printf("Usage: ./%s [options]\n\n", binary);

  printf("Possible arguments:\n\n");
  printf("\t-h\t\t\tPrint help screen\n");
  printf("\t-a [filename]\t\tAssemble file, default output ./a.out\n");
  printf("\t-o [filename]\t\tSpecify assembler output file name\n");
  printf("\t-e [filename]\t\tRun emulator on specified binary file\n");

  putchar('\n');

}

void parse_args(int argc, char** argv) {

  int opt;

  while((opt = getopt(argc, argv, "ha:o:e:")) != -1) { 

    switch(opt) {

      case 'h':
        usage();

        exit(-1);

      case 'a':
        assemble = true;
        inf = optarg;

        break;

      case 'o':
        outf = optarg;

        break;

      case 'e':
        emulate = true;
        runf = optarg;

        break;

      case '?':
        usage();

        exit(-1);

    } 
  } 
}

void run() {

  if (assemble)
    assembler();

  if (emulate)
    emulator();

}

int main(int argc, char** argv) {

  binary = argv[0];

  if (argc < 2) {
    usage();

    exit(-1);
  }

  parse_args(argc, argv);
  run();

  return 0;
}

