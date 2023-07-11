/// \file   adc.c
/// \author Gonçalo Marques
/// \date   2023-05

#include <stdint.h>
#include "include/adc.h"
#include "include/mem.h"

#define ADC_CHANNELS (sizeof(adc_channel))

typedef enum states {
    set_channel,
    acq_time, 
    start_conv,
    conv_time,
    result
} states_t;

static states_t state = set_channel;
static uint8_t current_signal = 0;

static uint16_t value[ADC_CHANNELS] = {}; 

void adc_init(void){
    reg_set(TRISA_ADD, 0x0034);
    reg_write(ANSELA_ADD, 0x0034);
    reg_write(ADCON1_ADD, (6 << 4)); //Clock source FOSC/64, 32MHz clock, 2 us conversion time
    reg_write(ADCON0_ADD, 0x0001 | (ADC_CH_VIN << 2)); //turn on ADON and select channel 2.
};

void adc_run(void){
    uint8_t conv;
    uint8_t adcon = reg_read(ADCON0_ADD);

    switch(state) {
        case set_channel:
            reg_write(ADCON0_ADD, (adcon & 0x3) | (adc_channel[current_signal] << 2));  // set channel
            state = acq_time;
            break;
        case acq_time:
            //do nothing wait for 1ms - settling time
            state = start_conv;
            break;
        case start_conv:
            reg_write(ADCON0_ADD, (adcon | 2));   // start conversion
            state = conv_time;
            break;
        case conv_time:
            //wait 1ms - aquisition time
            state = result;
            break;
        case result:
            conv = (reg_read(ADCON0_ADD) & 2);    // check if conversion finished
            if(conv == 0){
                value[current_signal] = reg_read(ADRESL_ADD) + (((uint16_t)reg_read(ADRESH_ADD))<<8); // save value
                current_signal++;                   // increment signal
                current_signal %= ADC_CHANNELS;     // limit signal number to used channels
                state = set_channel;
            };
    };
};

uint16_t adc_value(uint8_t channel) {
  uint8_t i = 0;
  for(i = 0; i < ADC_CHANNELS; i++) {
    if (adc_channel[i] == channel)
      break;
  }
  if (i < ADC_CHANNELS) return value[i];
  return 0;
}

uint16_t adc_signalvalue(uint16_t adc_channel, uint16_t signalMax, uint16_t signalMin) {
  return (adc_value(adc_channel) * (signalMax - signalMin)) / ADC_MAX + signalMin;
}