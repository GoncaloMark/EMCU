#include "include/io.h"
#include "include/iosim.h"
#include "include/clock.h"
#include "include/adc.h"
#include "include/adcsim.h"
#include "include/windsim.h"
#include "include/avg_filter.h"
#include <stdio.h>

int main(void) {
    Timer_t T = { 0,   100 };
    Timer_t T2 = { 0, 1000 };
    AVG_Filter_t Vin;

    output_t out;
    input_t in;
    out.leds = green;
    t_start(&T);
    t_start(&T2);
    avgf_init(&Vin);

    while(1){
        set_outputs(&out);
        iosim_run();
        adc_run();
        get_inputs(&in);
        adcsim_run();
        windsim_run();
        avgf_addSample(&Vin, in.vin);

        if(out.leds == green){
            out.leds = red;
        } else if(out.leds == red){
            out.leds = yellow;
        } else if(out.leds == yellow){
            out.leds = green;
        };

        if(t_elapsed(&T2)){
            printf("VIN: %d, VBAT: %d, VOUT: %d \n", avgf_value(&Vin), in.vbat, in.vout);
            t_start(&T2);
        };
        //get_inputs(&in);
        while(!t_elapsed(&T));
        t_start(&T);
    };
    
    return 0;    
};