/// \file   io.h
/// \author Gonçalo Marques
/// \date   2023-05

#ifndef __IO_H__
#define __IO_H__

#include <stdint.h>

typedef struct OUT {
    uint16_t brake;
    uint16_t vout;
    uint8_t leds;
} output_t;

typedef struct IN {
    uint16_t vin;
    uint16_t vbat;
    uint16_t vout;
} input_t;

typedef enum LEDS {
    green = 0x20,
    yellow = 0x10,
    red = 0x08
} led_t;

void init_io(void);
void set_outputs(output_t* outputs);

#endif  // __IO_H__