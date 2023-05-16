/// \file   adcsim.c
/// \author Gonçalo Marques
/// \date   2023-05

#include <stdint.h>
#include "include/mem.h"
#include "include/adc.h"
#include "include/batsim.h"

void adcsim_run(uint16_t vin, uint16_t vbat) {
    uint8_t adcon = read_reg(ADCON0_ADD);
    uint8_t channel = adcon >> 2;
    uint16_t value = 0;

    // TODO: calculate output voltage from PWM registers and Vin - for now turbine is directly connected to the battery just as the real turbine
    uint16_t vout = vin;

    if(adcon & 2) {
        switch(channel) {
            case ch_vin:
                value = (vin * 1024) / 15000;
                break;
            case ch_vout:
                value = (vout * 1024) / 1500;
                break;
            case ch_vbat:
                value = (vbat * 1024) / 1500;
                break;
        }

        write_reg(ADRESL_ADD, value);
        write_reg(ADRESH_ADD, value>>8);
        write_reg(ADCON0_ADD, adcon & (~0x2));
    }
}