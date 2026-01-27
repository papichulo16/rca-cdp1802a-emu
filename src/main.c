#include <stdio.h>
#include <stdlib.h>

#include "hw.h"

int main(int argc, char** argv) {

  if (argc < 1) {
    perror("usage: ./app");

    return -1;
  }

  hw_oscillate_clock();

  return 0;
}
