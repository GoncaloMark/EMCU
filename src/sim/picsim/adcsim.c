/// \file   adcsim.c
/// \author Gonçalo Marques
/// \date   2023-05

#include <stdint.h>
#include "drivers/include/mem.h"
#include "drivers/include/adc.h"
#include "sim/picsim/include/adcsim.h"

#define ADC_CHANNELS (11)

static uint16_t values[ADC_CHANNELS];

void adcsim_run() {
  uint8_t adcon = reg_read(ADCON0_ADD);
  uint8_t channel = adcon >> 2;
  uint16_t value = 0;

  // set value for current channel if ADC is enabled and a conversion was requested
  if ((adcon & 0x03) == 0x03) {
    if ((channel < ADC_CHANNELS)) value = values[channel];
    reg_write(ADRESL_ADD, value);
    reg_write(ADRESH_ADD, value>>8);
    reg_write(ADCON0_ADD, adcon & (~0x2));
  }
}

static uint16_t adcsim_value(uint16_t signalValue, uint16_t signalMax, uint16_t signalMin) {
  return ((signalValue - signalMin) * ADC_MAX) / (signalMax - signalMin);
}

void adcsim_setSignal(uint16_t channel, uint16_t signalValue, uint16_t signalMax, uint16_t signalMin) {
  if (channel < ADC_CHANNELS)
    values[channel] = adcsim_value(signalValue, signalMax, signalMin);
}