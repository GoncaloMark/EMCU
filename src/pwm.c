/// \file   pwm.c
/// \author Gonçalo Marques
/// \date   2023-05

#include "include/pwm.h"
#include "include/mem.h"

static unsigned float value = 0;

void pwm_init(void){
    write_reg(PWMCON5_ADD, 0x0008); // Set Polarity
    write_reg(PR2_ADD, 0xFF); // Set Period
    write_reg(PWM5DCH_ADD, ((value >> 2) & 0xFF)); // Set MSB (duty cycle)
    write_reg(PWM5DCL_ADD, (value & 0x3)); // Set LSB (duty cycle)

    write_reg(PIR1_ADD, 0x13); // Clear TMRF2 flag
    write_reg(T2CON_ADD, 0x0001); // Timer prescale value, it affects the frequency of the PWM signal
    write_reg(T2CON_ADD, 0x0004); // Enable Timer 2 

    while(!(read_reg(PIR1_ADD) & 0x2)); // Wait for TIMER2IF Overflow Interrupt to be set

    write_reg(TRISC_ADD, 0x0003); // Enable RC0 and RC1 pins
    write_reg(RC1PPS_ADD, 0x0002); // Set RC1 Peripheral Pin Select

    write_reg(PWM5CON_ADD, 0x0040); // Enable the PWM module
};

void pwm_set_duty(uint8_t duty){
    unsigned float duty_p = duty / 100;
    value = duty_p * 1023;
};