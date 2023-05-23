/// \file   pwm.c
/// \author Gonçalo Marques
/// \date   2023-05

#include "include/pwm.h"
#include "include/mem.h"

void pwm_init(void){
  reg_write(PWM5CON_ADD, 0x0008); // Set Polarity

  reg_write(PWM6CON_ADD, 0x08); // Set Polarity

  reg_write(PR2_ADD,    0xFF);  // Set Period
  reg_write(T2CON_ADD,  0x04);  // Enable Timer 2 

  reg_set(TRISC_ADD,    0x03);  // Enable RC0 (BRAKE) and RC1 (VOUT) pins
  reg_write(RC1PPS_ADD, 0x02);  // Set RC1 Peripheral Pin Select
  reg_write(RC0PPS_ADD, 0x03);  // Set RC0 Peripheral Pin Select

  reg_write(PWM5CON_ADD, 0x80); // Enable the PWM5 module
  reg_write(PWM6CON_ADD, 0x80); // Enable the PWM6 module
  reg_write(PWMTMRS_ADD, 1 << 2 | 1); // select TMR2 as source for both PWM signals
};

static void pwm_setPWM5(uint16_t pwm_value) {
  reg_write(PWM5DCH_ADD, (pwm_value >> 2) & 0xFF); // Set MSB (duty cycle)
  reg_write(PWM5DCL_ADD, pwm_value & 0x3);         // Set LSB (duty cycle)
}

static void pwm_setPWM6(uint16_t pwm_value) {
  reg_write(PWM6DCH_ADD, (pwm_value >> 2) & 0xFF); // Set MSB (duty cycle)
  reg_write(PWM6DCL_ADD, pwm_value & 0x3);         // Set LSB (duty cycle)
}

void pwm_setDuty(uint8_t pwm_channel, uint16_t duty) {
  uint16_t value = (duty * PWM_MAX) / (100*100);
  switch (pwm_channel) {
    case PWM_CH_BRAKE: pwm_setPWM6(value); break;
    case PWM_CH_VOUT:  pwm_setPWM5(value); break;
    default: return;
  }
}

void pwm_setRatio(uint8_t pwm_channel, uint16_t sourceSignal, uint16_t outputSignal) {
  // limit output to source value
  if (outputSignal > sourceSignal)
    outputSignal = sourceSignal;

  // calculate duty cycle
  uint16_t duty = 0;
  if (sourceSignal > 0)
    duty = (((uint32_t)outputSignal) * 100 * 100) / sourceSignal;
  pwm_setDuty(pwm_channel, duty);
}
