/// \file   timer.h
/// \author Gonçalo Marques
/// \date   2023-05

#pragma once
#include <stdint.h>
#include <time.h>
#include <stdbool.h>

/// @brief Timer - A struct to manage timers, contains the start time and the requested time to wait until completed.
typedef struct Timer {
    clock_t start;
    uint16_t req_time;
} Timer_t;

/// @brief          - Initializes the timer.
/// @param timer    - The timer to initialize
void    t_start(Timer_t* timer);

/// @brief          - Function that checks if the timer has expired/completed by comparing the elapsed time to the requested time.
/// @param timer    - The timer to check
/// @return         - bool: true if the timer has expired
bool    t_expired(Timer_t* timer);

/// @brief          - Function that returns the elapsed time since the timer was started.
/// @param timer    - The timer to check
/// @return         - The elapsed time since the timer was started
clock_t t_elapsed(Timer_t* timer);