#include "include/io.h"
#include "include/iosim.h"
#include "include/timer.h"
#include "include/adc.h"
#include "include/adcsim.h"
#include "include/windsim.h"
#include "include/avg_filter.h"
#include <stdio.h>

void idle_run(void)
{
    // do nothing
    // add here functions meant run in the background during idle time
}

int main(void) {
    Timer_t cycleTimer;         // a timer to control the loop cycle interval
    Timer_t logTimer;           // a timer to control log interval
    Timer_t chrono;             // a timer to measure execution time
    AVG_Filter_t Vin;           // average filter for input voltage readings
    AVG_Filter_t execTime;      // average filter for input voltage readings

    output_t out;               // pcb outputs (leds and pwm control for Vout and Brake Resistor)
    input_t  in;                // pcb inputs  (ADC Vin, Vbat, Vout)
    out.leds = green;
    cycleTimer.req_time = 100;  // 100ms control loop 
    t_start(&cycleTimer);       // start cycle timer
    logTimer.req_time   = 1000; // 1s log interval
    t_start(&logTimer);         // start log timer
    avgf_init(&Vin);            // initialize average filter for input voltage ADC measurements
    avgf_init(&execTime);       // initialize average filter for execution time measurements

    while(1){
        // run control code
        t_start(&chrono);
        adc_run();                      // read input voltages from adc
        get_inputs(&in);                // get input values
        avgf_addSample(&Vin, in.vin);   // 
        set_outputs(&out);              // set leds


        // cycle led lights while control for them is not implemented
        if(out.leds == green){
            out.leds = red;
        } else if(out.leds == red){
            out.leds = yellow;
        } else if(out.leds == yellow){
            out.leds = green;
        };

        avgf_addSample(&execTime, (uint16_t)t_elapsed(&chrono));

        // run simulation code
        iosim_run();
        adcsim_run();
        windsim_run();
        // log status every logInterval
        if(t_expired(&logTimer)){
            printf("VIN: %d V, VBAT: %d V, VOUT: %d V, (Execution time average: %dms)\r", avgf_value(&Vin), in.vbat, in.vout, avgf_value(&execTime));
            fflush(NULL);
            t_start(&logTimer);
        };

        while(!t_expired(&cycleTimer)) 
        {
            // wait for cycle time to expire - run idle task
            idle_run();
        };
        t_start(&cycleTimer);

    };
    
    return 0;    
};