/// \file   adcsim.c
/// \author Gonçalo Marques
/// \date   2023-05

#include <stdint.h>
#include "include/mem.h"
#include "include/adc.h"
#include "include/windsim.h"
#include "include/pwm.h"

#define VBAT (12)

void adcsim_run(uint16_t vin){
    uint8_t adcon = reg_read(ADCON0_ADD);
    uint8_t channel = adcon >> 2;
    uint16_t value = 0;
    uint16_t vout = ((reg_read(PWM5DCH_ADD) << 2) | (reg_read(PWM5DCL_ADD) & 0x03) * vin) / PWM_MAX;
    
    if(adcon & 2){
        //printf("ADCON: %d\n", adcon);
        switch(channel){
            case ch_vin:
                value = (windsim_getInputVoltage() * 1024) / 150;
                break;
            case ch_vout:
                value = (vout * 1023) / (15 * 100);
                break;
            case ch_vbat:
                value = (VBAT * 1023) / 15;
                break;
        }

        reg_write(ADRESL_ADD, value);
        reg_write(ADRESH_ADD, value>>8);
        reg_write(ADCON0_ADD, adcon & (~0x2));
        //printf("ADC CHANNEL: %d, VALUE: %d\n", channel, value);
    }
}