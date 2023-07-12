/// \file   timer.c
/// \author Gonçalo Marques
/// \date   2023-05

#include <stdint.h>
#include <stdbool.h>
#include "include/mem.h"
#include "include/timer.h"

static uint16_t clock() {
  volatile uint16_t clk = reg_read(TMR0L_ADD);
  return (clk + (((uint16_t)reg_read(TMR0H_ADD)) << 8));
}

void t_init() {
      // enable TMR0 as 16 bit timer counting ms (with some neglegible error)
    reg_set(T0CON0_ADD, 0x90);    // T0EN = 1, T016BIT = 1
    reg_write(T0CON1_ADD, 0x4D);  // T0CS = Fosc / 4 - Prescaler 1 : 8192
    reg_write(OSCCON1_ADD, 0x60); // NOSC = HFINTOSC 1MHz, NDIV = 1
    reg_write(OSCFRQ_ADD, 0x05);  // System Clock Frequency - 32 MHz
}

void t_start(Timer_t* timer){
    timer->start = clock();
};

uint16_t t_elapsed(Timer_t* timer){
    uint16_t now = clock();

    if (now >= timer->start)
      return (now - timer->start);
    return (TMR0_MAXVALUE - timer->start + now);
};

bool t_expired(Timer_t* timer)
{
    return t_elapsed(timer) > timer->req_time;
}