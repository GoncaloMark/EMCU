#pragma once
#include <stdint.h>
#include <time.h>

typedef struct Timer {
    clock_t start;
    uint16_t req_time;
} Timer_t;

void t_start(Timer_t* timer);
uint8_t t_elapsed(Timer_t* timer);