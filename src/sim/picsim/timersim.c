/// \file   timersim.c
/// \author Bernardo Marques
/// \date   2023-07

#include <time.h>
#include "drivers/include/mem.h"
#include "drivers/include/timer.h"

void tsim_run() {
  clock_t clk = clock();
  uint16_t ms = clk / ((CLOCKS_PER_SEC/1000));
  reg_write(TMR0L_ADD, ms);
  reg_write(TMR0H_ADD, ms >> 8);
}