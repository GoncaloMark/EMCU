/// \file   iosim.h
/// \author Gonçalo Marques
/// \date   2023-05

#include "include/adc.h"
#include "include/io.h"
#include "include/mem.h"
#include <stdint.h>

// Analog input constants
#define VIN_MAX  (150)      // maximum voltage in Vin  - 150V
#define VBAT_MAX  (15)      // maximum voltage in VBat -  15V
#define VOUT_MAX  (15)      // maximum voltage in VOut -  15V

// ADC constants
#define ADC_BITS  (10)
#define ADC_MAX   (1 << ADC_BITS)

typedef enum states {
    set_channel,
    acq_time, 
    start_conv,
    conv_time,
    result
} states_t;

static states_t state = set_channel;
static uint8_t current_channel = ch_vin;

static input_t inputs;

void read_voltage(uint16_t adc_value){
    switch(current_channel){
        case ch_vin:
            inputs.vin = (adc_value * VIN_MAX * 100) / ADC_MAX;
            break;
        case ch_vbat:
            inputs.vbat = (adc_value * VBAT_MAX * 100) / ADC_MAX;
            break;
        case ch_vout:
            inputs.vout = (adc_value * VOUT_MAX * 100) / ADC_MAX;
            break;
    };   
};

void get_inputs(input_t* in){
    in->vin = inputs.vin;
    in->vbat = inputs.vbat;
    in->vout = inputs.vout;
};

void change_channel(void){
    switch(current_channel){
        case ch_vin:
            current_channel = ch_vbat;
            break;
        case ch_vbat:
            current_channel = ch_vout;
            break;
        case ch_vout:
            current_channel = ch_vin;
            break;
    };
};

void adc_init(void){
    write_reg(TRISA_ADD, 0x0034);
    write_reg(ANSELA_ADD, 0x0034);
    write_reg(ADCON1_ADD, (5 << 4)); //Clock source FOSC/16 4MHz clock 4 ys
    write_reg(ADCON0_ADD, 0x0001 | (ch_vin << 2)); //turn on ADON and select channel 2.
};

void adc_run(void){
    uint8_t conv;
    uint8_t adcon = read_reg(ADCON0_ADD);
    static states_t old_state = set_channel;
    switch(state) {
        case set_channel:
            write_reg(ADCON0_ADD, (adcon & 0x3) | (current_channel << 2));
            state = acq_time;
            break;
        case acq_time:
            //do nothing wait for 1ms
            state = start_conv;
            break;
        case start_conv:
            write_reg(ADCON0_ADD, (adcon | 2));
            state = conv_time;
            break;
        case conv_time:
            //wait 1ms
            state = result;
            break;
        case result:
            conv = (read_reg(ADCON0_ADD) & 2);
            if(conv == 0){
                read_voltage((read_reg(ADRESL_ADD) + (((uint16_t)read_reg(ADRESH_ADD))<<8)));
                change_channel();
                state = set_channel;
            };
            
    };

    if(state != old_state){
        //printf("ADC STATE CHANGE: old - %d new - %d\n", old_state, state);
        old_state = state;
    };
};