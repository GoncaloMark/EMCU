/// \file   iosim.c
/// \author Gonçalo Marques
/// \date   2023-05

#include "include/io.h"
#include "include/mem.h"
#include <stdint.h>
#define LATC_ADD  (0x010E)

static output_t out; 
//static input_t in;

void iosim_run(void){
    static uint8_t previous = 0;
    //printf("previous: %d \n", previous);
    out.leds = read_reg(LATC_ADD) & (red | green | yellow);
    if(out.leds != previous){
        //printf("LEDS: %d\n", out.leds);
        previous = out.leds;
    };
};