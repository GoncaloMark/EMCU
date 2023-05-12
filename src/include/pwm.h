/// \file   pwm.h
/// \author Gonçalo Marques
/// \date   2023-05

#pragma once
#include <stdint.h>

#define TRISC_ADD (0x008E)
#define PWM5CON_ADD (0x0619)
#define PR2_ADD (0x001E)
#define PWM5DCL_ADD (0x0617)
#define PWM5DCH_ADD (0x0618)
#define T2CON_ADD (0x001F)
#define PIR1_ADD (0x0011)
#define RC1PPS_ADD (0x0EA1)

void pwm_init(void);
void pwm_set_duty(uint8_t duty);