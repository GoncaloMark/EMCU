#pragma once
#include <stdint.h>
#include "io.h"
#define TRISA_ADD (0x008C)
#define ANSELA_ADD (0x018C)
#define ADCON0_ADD (0x009D)
#define ADCON1_ADD (0x009E)
#define ADRESL_ADD (0x009B)
#define ADRESH_ADD (0x009C)

typedef enum adc_channel {
    ch_vin = 2,
    ch_vbat = 4,
    ch_vout = 5
} adc_channel_t;

void get_inputs(input_t* inputs);
void adc_init(void);
void adc_run(void);
