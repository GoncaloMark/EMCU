#include <stdio.h>
#include "include/io.h"
#include "include/timer.h"
#include "include/adc.h"
#include "include/avg_filter.h"
#include "sim/include/iosim.h"
#include "sim/include/adcsim.h"
#include "sim/include/batsim.h"
#include "sim/include/brakesim.h"
#include "sim/include/turbinesim.h"

static Turbine_t t;
static Battery_t b;
static output_t out;               // pcb outputs (leds and pwm control for Vout and Brake Resistor)

void idle_run(void)
{
    // run adc and adc_sim faster than control cycle because it takes several cycles to update all voltage values
    // needs to be changed to a faster loop cycle intead of running in idle
    adc_run();
    adcsim_run(t.vout, b.voltage);
    // do nothing
    // add here functions meant run in the background during idle time
}

int main(void) {
    Timer_t cycleTimer;         // a timer to control the loop cycle interval
    Timer_t logTimer;           // a timer to control log interval
    Timer_t chrono;             // a timer to measure execution time
    AVG_Filter_t Vin;           // average filter for input voltage readings
    AVG_Filter_t execTime;      // average filter for executio time measurements
    uint32_t acc_power = 0;     // total power charged to the battery in mWh

    input_t  in;                // pcb inputs  (ADC - Vin, Vbat, Vout)
    out.leds = green;
    cycleTimer.req_time = 100;  // 100ms control loop 
    t_start(&cycleTimer);       // start cycle timer
    logTimer.req_time   = 200;  // 200ms log interval
    t_start(&logTimer);         // start log timer
    avgf_init(&Vin);            // initialize average filter for input voltage ADC measurements
    avgf_init(&execTime);       // initialize average filter for execution time measurements
    turbinesim_init(&t);        // initialize turbine simulation
    batsim_init(&b);            // initialize battery simulation

    while(1){
        // run control code
        t_start(&chrono);
        adc_run();                           // run adc to convert input voltages
        get_inputs(&in);                     // get input values
        avgf_addSample(&Vin, in.vin);
        set_outputs(&out);                   // set leds

        // cycle led lights while control for them is not implemented
        if(out.leds == green){
            out.leds = red;
        } else if(out.leds == red){
            out.leds = yellow;
        } else if(out.leds == yellow){
            out.leds = green;
        }

        avgf_addSample(&execTime, (uint16_t)t_elapsed(&chrono));

        // run simulation code
        // io simulation
        iosim_run();
        // run adc simulation
        adcsim_run(t.vout, b.voltage);         // Vin = Turbine.Vout, Vbat = Battery.Voltage

        // run battery simulation
        uint32_t active_power = batsim_run(&b, in.vout);  // simulated battery charges with generated output voltage (out.vout or in.vout they should be the same)
        acc_power += (active_power * BAT_SIM_CALL_RATE_ms) / 3600;
        // run brake simulation
        uint32_t wasted_power = brakesim_run(in.vin);

        // run wind turbine simulation
        turbinesim_run(&t, active_power + wasted_power);
        // log status every logInterval
        if(t_expired(&logTimer)){
            printf(" WS: %4.2f m/s, WP: %4.2f kW, GP: %3.2f kW, RPM: %4d, VIN: %5.2fV, VBAT: %4.2fV, VOUT: %4.2fV, Active Power: %4d W, Stored Power: %4d Wh, Bat: %4.2f%%  \r", ((float)t.windspeed) / 100, ((float)t.windpower) / 1000, ((float)t.genpower) / 1000, t.rpm, ((float)t.vout) / 100, ((float)in.vbat) / 100, ((float)in.vout) / 100, active_power, acc_power / 1000, (float)b.soc / 100);
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