/// \file   pwmsim.c
/// \author Bernardo Marques
/// \date   2023-05

#include <stdint.h>
#include "drivers/include/pwm.h"
#include "drivers/include/mem.h"

uint16_t pwmsim_getDuty(uint8_t pwm_channel) {
  uint16_t value = 0;
  switch (pwm_channel) {
    case PWM_CH_BRAKE: value = (((uint16_t)reg_read(PWM6DCH_ADD)) << 2) + (reg_read(PWM6DCL_ADD) & 0x03); break;
    case PWM_CH_VOUT:  value = (((uint16_t)reg_read(PWM5DCH_ADD)) << 2) + (reg_read(PWM5DCL_ADD) & 0x03); break;
    default: break;
  }
  return (((uint32_t)value) * 100 * 100) / PWM_MAX;
}

uint16_t pwmsim_getRatioSignal(uint8_t pwm_channel, uint16_t sourceSignal) {
  uint16_t duty = pwmsim_getDuty(pwm_channel);
  return (((uint32_t)sourceSignal) * duty) / (100 * 100);
}