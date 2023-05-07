#include "include/io.h"
#include "include/mem.h"
#include <stdint.h>
#define LATC_ADD  (0x010E)

static output_t out; 
//static input_t in;

void get_outputs(void){
    out.leds = read_reg(LATC_ADD) & (red | green | yellow);
    //printf("out.leds: %d \n", out.leds);
};

void iosim_run(void){
    static uint8_t previous = 0;
    //printf("previous: %d \n", previous);
    get_outputs();
    if(out.leds != previous){
        //printf("LEDS: %d\n", out.leds);
        previous = out.leds;
    };
};