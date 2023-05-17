/// \file   pwm.c
/// \author Gonçalo Marques
/// \date   2023-05

#include "include/pwm.h"
#include "include/mem.h"

#define PWM_RES_BIT (10)
#define PWM_MAX ((1 << PWM_RES_BIT) - 1)

static uint16_t value = 0;
static uint16_t value2 = 0;

void pwm_init(void){
    reg_write(PWMCON5_ADD, 0x0008); // Set Polarity

    reg_write(PWMCON6_ADD, 0x0008); // Set Polarity

    reg_write(PR2_ADD, 0xFF); // Set Period
    reg_write(T2CON_ADD, 0x0004); // Enable Timer 2 

    reg_set(TRISC_ADD, 0x0003); // Enable RC0 (BRAKE) and RC1 (VOUT) pins
    reg_write(RC1PPS_ADD, 0x0002); // Set RC1 Peripheral Pin Select
    reg_write(RC0PPS_ADD, 0x0003); // Set RC0 Peripheral Pin Select

    reg_write(PWM5CON_ADD, 0x0040); // Enable the PWM module
};

void pwm_scan(inputs_t* inputs, outputs_t* outputs){
    uint16_t brake_val = (outputs->brake * PWM_MAX) / (100*100);
    reg_write(PWM6DCH_ADD, ((brake_val >> 2) & 0xFF)); // Set MSB (duty cycle)
    reg_write(PWM6DCL_ADD, (brake_val & 0x3)); // Set LSB (duty cycle)

    uint16_t vout_val = ((outputs->vout + 70) * PWM_MAX) / (inputs->vin);
    reg_write(PWM5DCH_ADD, ((vout_val >> 2) & 0xFF)); // Set MSB (duty cycle)
    reg_write(PWM5DCL_ADD, (vout_val &0x3)); // Set LSB (duty cycle)
};
