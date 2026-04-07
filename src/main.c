#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include "hw.h"
#include "dbg.h"
#include "instr_tree.h"
#include "asm.h"
#include "utils.h"

char* binary;
char* outf = "a.out";
char* inf;
char* conf;
int yaml = 0;
bool assemble = false;
bool emulate = false;

void assembler() {

  asm_main(inf, outf);

}

void emulator(){

  printf("Starting emulator...\n");
  hw_sys_t* sys = malloc(sizeof(hw_sys_t));

  hw_sys_init(conf, sys, yaml);
  dbg_cmds_init();
  instr_tree_init();

  printf("System config initialized, running emulator...\n\n");
  hw_sys_run(sys);

  printf("Exiting...\n\n");
  hw_sys_cleanup(sys);
  dbg_cmds_clean();
  free(sys);

}

void usage() {

  printf("This is a clock-cycle accurate hardware emulator and cross-assembler for the legacy RCA CDP1802a 8-bit microprocessor, made from scratch.\n\nUtilizing an independent CPU and system bus design, this emulator is able to do systems of systems tasks (as long as it is multiple RCA 1802 chips communicating with each other since that is the only MPU implemented lol).\n\nThis project was mainly made for educational purposes since it forced us to strictly adhere to a datasheet and follow along to its architecture, write an independent and modularized design for having systems of systems, and use tools like flex and bison.\n\n");
  printf("Authors: Luis Abraham, Dominick Morales, Dylin Irons, Giovanni Williams.\n\n");

  printf("Usage: %s [options]\n\n", binary);

  printf("Possible arguments:\n\n");
  printf("\t-h\t\t\tPrint help screen\n");
  printf("\t-a [filename]\t\tAssemble file, default output ./a.out\n");
  printf("\t-o [filename]\t\tSpecify assembler output file name\n");
  printf("\t-e [filename]\t\tRun emulator on specified binary file\n");
  printf("\t-y [filename]\t\tRun emulator on configured YAML system\n");

  putchar('\n');

}

void parse_args(int argc, char** argv) {

  int opt;

  while((opt = getopt(argc, argv, "ha:o:e:y:")) != -1) { 

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

      case 'y':
        yaml = 1;

      case 'e':
        emulate = true;
        conf = optarg;

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

