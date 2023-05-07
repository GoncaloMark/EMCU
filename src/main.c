#include "include/io.h"
#include "include/iosim.h"
#include "include/clock.h"
#include "include/adc.h"
#include "include/adcsim.h"
#include <stdio.h>

int main(void) {
    init_io();
    adc_init();

    Timer_t T = {
        0,
        100
    };

    Timer_t T2 = {
        0,
        1000
    };

    output_t out;
    input_t in;
    out.leds = green;
    t_start(&T);
    t_start(&T2);

    while(1){
        set_outputs(&out);
        iosim_run();
        adc_run();
        get_inputs(&in);
        adcsim_run();

        if(out.leds == green){
            out.leds = red;
        } else if(out.leds == red){
            out.leds = yellow;
        } else if(out.leds == yellow){
            out.leds = green;
        };

        if(t_elapsed(&T2)){
            printf("VIN: %d, VBAT: %d, VOUT: %d \n", in.vin, in.vbat, in.vout);
            t_start(&T2);
        };
        //get_inputs(&in);
        while(!t_elapsed(&T));
        t_start(&T);
    };
    
    return 0;    
};