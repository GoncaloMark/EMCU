/// \file   pwm.h
/// \author Gonçalo Marques
/// \date   2023-05

#pragma once
#include <stdint.h>
#include "include/io.h"

#define PWM_RES_BIT (10)
#define PWM_MAX ((1 << PWM_RES_BIT) - 1)
#define TRISC_ADD (0x008E)
#define PWM5CON_ADD (0x0619)
#define PWM6CON_ADD (0x061C)
#define PR2_ADD (0x001E)
#define PWM5DCL_ADD (0x0617)
#define PWM5DCH_ADD (0x0618)
#define PWM6DCL_ADD (0x061A)
#define PWM6DCH_ADD (0x061B)
#define T2CON_ADD (0x001F)
#define PIR1_ADD (0x0011)
#define RC1PPS_ADD (0x0EA1)
#define RC0PPS_ADD (0x0EA0)

/// @brief          - A function to initialize the PWM Registers
/// @param          - void
void pwm_init(void);

/// @brief          - A function to run the PWM module (sets duty cycle based on the brake value and on the desired output voltage) and starts the wave modulation
/// @param inputs   - Input parameters for the PWM module (for example vin)
/// @param outputs  - Output values for the PWM module (for example brake or vout)
void pwm_run(input_t* inputs, output_t* outputs);