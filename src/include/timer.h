#pragma once
#include <stdint.h>
#include <time.h>
#include <stdbool.h>

typedef struct Timer {
    clock_t start;
    uint16_t req_time;
} Timer_t;

void    t_start(Timer_t* timer);
bool    t_expired(Timer_t* timer);
clock_t t_elapsed(Timer_t* timer);