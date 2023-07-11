/// \file   pwm.h
/// \author Gonçalo Marques
/// \date   2023-05

#pragma once
#include <stdint.h>

#define PWM_RES_BIT (10)
#define PWM_MAX     ((1 << PWM_RES_BIT) - 1)
#define TRISC_ADD   (0x008E)
#define PWM5CON_ADD (0x0619)
#define PWM6CON_ADD (0x061C)
#define PR2_ADD     (0x001E)
#define PWM5DCL_ADD (0x0617)
#define PWM5DCH_ADD (0x0618)
#define PWM6DCL_ADD (0x061A)
#define PWM6DCH_ADD (0x061B)
#define T2CON_ADD   (0x001F)
#define PIR1_ADD    (0x0011)
#define RC1PPS_ADD  (0x0EA1)
#define RC0PPS_ADD  (0x0EA0)
#define PWMTMRS_ADD (0x061F)

#define PWM_CH_BRAKE (6)
#define PWM_CH_VOUT  (5)

/// @brief Initialize PWM module
void pwm_init(void);

/// @brief Set Duty Cycle on a PWM channel
/// @param pwm_channel the channel to set
/// @param duty the desired duty cycle
void pwm_setDuty(uint8_t pwm_channel, uint16_t duty);

/// @brief Sets a PWM channel to generate an output signal that is a fraction of an input signal
/// @param pwm_channel the channel to set
/// @param sourceSignal source signal value
/// @param ouputSignal output signal value
void pwm_setRatio(uint8_t pwm_channel, uint16_t sourceSignal, uint16_t outputSignal);