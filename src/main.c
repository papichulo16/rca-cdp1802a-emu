#include <stdio.h>
#include <stdlib.h>

#include "hw.h"

int main(int argc, char** argv) {

  if (argc < 1) {
    perror("usage: ./app");

    return -1;
  }

  cdp1802a_chip_t* chip = malloc(sizeof(cdp1802a_chip_t));

  hw_sys_init(chip);
  hw_sys_run(chip);

  free(chip);

  return 0;
}
