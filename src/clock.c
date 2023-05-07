#include <time.h>
#include <stdint.h>
#include "include/clock.h"

void t_start(Timer_t* timer){
    timer->start = clock();
};

uint8_t t_elapsed(Timer_t* timer){
    clock_t now = clock();
    clock_t elapsed = (now-timer->start)/(CLOCKS_PER_SEC/1000);
    return elapsed > timer->req_time;
};