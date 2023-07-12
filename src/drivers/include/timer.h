/// \file   timer.h
/// \author Gonçalo Marques
/// \date   2023-05

#pragma once
#include <stdint.h>
#include <stdbool.h>

#define CLK_FREQ  (32000000)              // system clock frequency in Hertz
#define CLK_TMR   (CLK_FREQ / 4)          // Timer clock frequency

#define OSCCON1_ADD (0x0919)
#define OSCFRQ_ADD  (0x091F)

#define T0CON0_ADD (0x0017)
#define T0CON1_ADD (0x0018)
#define TMR0L_ADD  (0x0015)
#define TMR0H_ADD  (0x0016)
#define TMR0_MAXVALUE (0xFFFF)


/// @brief Timer - A struct to manage timers, contains the start time and the requested time to wait until completed.
typedef struct Timer {
    uint16_t start;
    uint16_t req_time;  // requested time (in ms): must be <= 65535 milliseconds
} Timer_t;

/// @brief Hardware Timer 0 initialization to count ms (16 bit timer)
void    t_init();

/// @brief          - Initializes the timer.
/// @param timer    - The timer to initialize
void    t_start(Timer_t* timer);

/// @brief          - Function that checks if the timer has expired/completed by comparing the elapsed time to the requested time.
/// @param timer    - The timer to check
/// @return         - bool: true if the timer has expired
bool    t_expired(Timer_t* timer);

/// @brief          - Function that returns the elapsed time since the timer was started.
/// @param timer    - The timer to check
/// @return         - The elapsed time since the timer was started (in ms)
uint16_t t_elapsed(Timer_t* timer);