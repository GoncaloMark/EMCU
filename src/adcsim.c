#include <stdint.h>
#include "include/mem.h"
#include "include/adc.h"
#include "include/windsim.h"

#define VOUT (14)
#define VBAT (12)

void adcsim_run(void){
    uint8_t adcon = read_reg(ADCON0_ADD);
    uint8_t channel = adcon >> 2;
    uint16_t value = 0;
    if(adcon & 2){
        //printf("ADCON: %d\n", adcon);
        switch(channel){
            case ch_vin:
                value = (windsim_getInputVoltage() * 1024) / 150;
                break;
            case ch_vout:
                value = (VOUT * 1024) / 15;
                break;
            case ch_vbat:
                value = (VBAT * 1024) / 15;
                break;
        }

        write_reg(ADRESL_ADD, value);
        write_reg(ADRESH_ADD, value>>8);
        write_reg(ADCON0_ADD, adcon & (~0x2));
        //printf("ADC CHANNEL: %d, VALUE: %d\n", channel, value);
    }
}