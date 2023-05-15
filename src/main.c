#include "include/io.h"
#include "include/iosim.h"
#include "include/timer.h"
#include "include/adc.h"
#include "include/adcsim.h"
#include "include/windsim.h"
#include "include/batsim.h"
#include "include/turbinesim.h"
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
    AVG_Filter_t execTime;      // average filter for executio time measurements
    uint16_t active_power;      // power used to charge the battery in this cycle
    uint32_t acc_power = 0;     // total power charged to the battery in cWh (Wh * 100)
    Turbine_t t;
    

    output_t out;               // pcb outputs (leds and pwm control for Vout and Brake Resistor)
    input_t  in;                // pcb inputs  (ADC - Vin, Vbat, Vout)
    out.leds = green;
    cycleTimer.req_time = 100;  // 100ms control loop 
    t_start(&cycleTimer);       // start cycle timer
    logTimer.req_time   = 200;  // 200ms log interval
    t_start(&logTimer);         // start log timer
    avgf_init(&Vin);            // initialize average filter for input voltage ADC measurements
    avgf_init(&execTime);       // initialize average filter for execution time measurements
    batsim_init();              // initialize battery simulation

    while(1){
        // run control code
        t_start(&chrono);
        adc_run();                           // run adc to convert input voltages
        get_inputs(&in);                     // get input values
        avgf_addSample(&Vin, in.vin);
        set_outputs(&out);                   // set leds
        active_power = batsim_run(in.vout);  // simulated battery charges with generated output voltage
        acc_power += (active_power * BAT_SIM_CALL_RATE_ms) / (10 * 3600);

        // cycle led lights while control for them is not implemented
        if(out.leds == green){
            out.leds = red;
        } else if(out.leds == red){
            out.leds = yellow;
        } else if(out.leds == yellow){
            out.leds = green;
        };

        // do something cpu intensive only so that execution time is greater than 0
        for (int i = 0; i < 1000; i++)
            for(int k = 0; k < 1000; k++)
            {
                if (avgf_value(&Vin) > 70000) out.leds = yellow;
            }

        avgf_addSample(&execTime, (uint16_t)t_elapsed(&chrono));

        // run simulation code
        iosim_run();
        adcsim_run(t.vout, 1400, 1200);
        turbinesim_run(&t, 0);
        // log status every logInterval
        if(t_expired(&logTimer)){
            printf(" WS: %4.2f m/s, WP: %4.2f kW, GP: %3.2f kW, RPM: %d, VIN: %5.2fV, VBAT: %4.2fV, VOUT: %4.2fV, Active Power: %4.2f W, Stored Power: %4.2f Wh (exec time avg: %d ms) \r", ((float)windsim_windspeed()) / 100, ((float)t.windpower) / 1000, ((float)t.genpower) / 1000, t.rpm, ((float)t.vout) / 100, ((float)in.vbat) / 100, ((float)in.vout) / 100, ((float)active_power) / 100, ((float)acc_power) / 100, avgf_value(&execTime));
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