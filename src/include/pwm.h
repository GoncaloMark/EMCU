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
#define PR2_ADD (0x001E)
#define PWM5DCL_ADD (0x0617)
#define PWM5DCH_ADD (0x0618)
#define T2CON_ADD (0x001F)
#define PIR1_ADD (0x0011)
#define RC1PPS_ADD (0x0EA1)

void pwm_init(void);
void pwm_scan(inputs_t* inputs, outputs_t* outputs);