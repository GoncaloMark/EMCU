/// \file   timer.c
/// \author Gonçalo Marques
/// \date   2023-05

#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include "include/timer.h"

void t_start(Timer_t* timer){
    timer->start = clock();
};

clock_t t_elapsed(Timer_t* timer){
    clock_t now = clock();
    return (now-timer->start)/(CLOCKS_PER_SEC/1000);
};

bool t_expired(Timer_t* timer)
{
    return t_elapsed(timer) > timer->req_time;
}