/// \file   adc.h
/// \author Gonçalo Marques
/// \date   2023-05

#pragma once
#include <stdint.h>
#include "io.h"
#define TRISA_ADD (0x008C)
#define ANSELA_ADD (0x018C)
#define ADCON0_ADD (0x009D)
#define ADCON1_ADD (0x009E)
#define ADRESL_ADD (0x009B)
#define ADRESH_ADD (0x009C)

/// @brief ADC Channel - An enum type that maps to the corresponding ADC channel
typedef enum adc_channel {
    ch_vin = 2,
    ch_vbat = 4,
    ch_vout = 5
} adc_channel_t;

/// @brief          - From a pointer to inputs, map the values to an inputs object that controls the state machine for the conversions; 
/// @param inputs   - Pointer to the input object initialized on main.
void get_inputs(input_t* inputs);

/// @brief  - Init the ADC registers
/// @param  - void
void adc_init(void);

/// @brief  - Run the ADC conversion finite state machine
/// @param  - void
void adc_run(void);
