#include <stdio.h>
#include <unistd.h>

#include "hw.h"
#include "cpu.h"

cdp1802a_chip_t chip;

void toggle_clock() {

  cpu_handle_state(&chip);
  
}

void hw_oscillate_clock() {

  while(1) {

    toggle_clock();

    sleep(1);
  }

}

