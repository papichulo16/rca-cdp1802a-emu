#include <stdio.h>
#include <unistd.h>

#include "hw.h"

cdp1802a_chip_t chip;

void toggle_clock() {

  
}

void hw_oscillate_clock() {

  while(1) {

    toggle_clock();

    sleep(1);
  }

}

