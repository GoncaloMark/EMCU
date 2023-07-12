/// \file   adc.h
/// \author Gonçalo Marques
/// \date   2023-05

#pragma once
#include <stdint.h>

// ADC constants
#define ADC_BITS     (10)               // ADC resolution
#define ADC_MAX      (1 << ADC_BITS)    // ADC maximum value

#define ADC_CH_VIN   (2)
#define ADC_CH_VBAT  (4)
#define ADC_CH_VOUT  (5)

// channel number for each of the used channels
static const uint8_t adc_channel[] = {
  ADC_CH_VIN,
  ADC_CH_VBAT,
  ADC_CH_VOUT
};

#define TRISA_ADD  (0x008C)
#define ANSELA_ADD (0x018C)
#define ADCON0_ADD (0x009D)
#define ADCON1_ADD (0x009E)
#define ADRESL_ADD (0x009B)
#define ADRESH_ADD (0x009C)

/// @brief Init the ADC registers
void adc_init(void);

/// @brief Run the ADC conversion finite state machine
void adc_run(void);

/// @brief Returns adc value for a given adc channel
/// @param channel the adc channel to get the value from
/// @return the value as calculated by the ADC (from 0 to ADC_MAX)
uint16_t adc_value(uint8_t channel);

/// @brief Calculate signal value for an ADC channel
/// @param adc_channel the adc channel for desired signal
/// @param signalMax the maximum possible value for the analog input
/// @param signalMin the minimum possible value for the analog input
/// @return signal value in the same unit as signalMax and signalMin
uint16_t adc_signalvalue(uint16_t adc_channel, uint16_t signalMax, uint16_t signalMin);
