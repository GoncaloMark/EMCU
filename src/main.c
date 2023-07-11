#define AVG_FILTER_DEPTH (10)

#include <stdio.h>
#include "drivers/include/io.h"
#include "drivers/include/timer.h"
#include "drivers/include/adc.h"
#include "drivers/include/pwm.h"
#include "include/turbine_control.h"
#include "sim/picsim/include/iosim.h"
#include "sim/picsim/include/adcsim.h"
#include "sim/picsim/include/pwmsim.h"
#include "sim/picsim/include/timersim.h"
#include "sim/include/batsim.h"
#include "sim/include/brakesim.h"
#include "sim/include/turbinesim.h"

#define VIN_MAX (150 * 100)   // maximum value for input voltage in V * 100
#define VOUT_MAX (15 * 100)   // maximum value for output voltage in V * 100
#define VBAT_MAX (15 * 100)   // maximum value for battery voltage in V * 100

static Turbine_t t;
static Battery_t b;
static output_t out;               // pcb outputs (leds and pwm control for Vout and Brake Resistor)

void idle_run(void)
{
    // run adc and adc_sim faster than control cycle because it takes several cycles to update all voltage values
    // needs to be changed to a faster loop cycle intead of running in idle
    adc_run();
    adcsim_run();
    tsim_run();
    // do nothing
    // add here functions meant run in the background during idle time
}

int main(void) {
    Timer_t cycleTimer;         // a timer to control the loop cycle interval
    Timer_t logTimer;           // a timer to control log interval
    uint32_t acc_power = 0;     // total power charged to the battery in mWh

    input_t  in;                // pcb inputs  (ADC - Vin, Vbat, Vout)
    out.leds = green;
    t_init();
    cycleTimer.req_time = 100;  // 100ms control loop 
    t_start(&cycleTimer);       // start cycle timer
    logTimer.req_time   = 200;  // 200ms log interval
    t_start(&logTimer);         // start log timer
    turbinesim_init(&t);        // initialize turbine simulation
    batsim_init(&b);            // initialize battery simulation
    pwm_init();                 // initialize pwm registers
    adc_init();                 // initialize ADC
    
    while(1){
        // run adc to convert input voltages
        adc_run();
        adcsim_run();
        tsim_run();

        // run control and simulation
        if (t_expired(&cycleTimer)) {
          // get values measured by the ADC to inputs
          in.vin  = adc_signalvalue(ADC_CH_VIN,  VIN_MAX,  0);
          in.vbat = adc_signalvalue(ADC_CH_VBAT, VBAT_MAX, 0);
          in.vout = adc_signalvalue(ADC_CH_VOUT, VOUT_MAX, 0);

          // run wind turbine control software
          turbine_control(in, &out);

          // set digital outputs
          set_outputs(&out);
          // set pwm outputs
          pwm_setDuty(PWM_CH_BRAKE, out.brake);
          pwm_setRatio(PWM_CH_VOUT, in.vin, out.vout);

          // run simulation code
          // io simulation
          iosim_run();
          // run adc simulation
          uint16_t vout = pwmsim_getRatioSignal(PWM_CH_VOUT, in.vin);
          adcsim_setSignal(ADC_CH_VIN,  t.vout,    VIN_MAX,  0);
          adcsim_setSignal(ADC_CH_VBAT, b.voltage, VBAT_MAX, 0);
          adcsim_setSignal(ADC_CH_VOUT, vout,       VOUT_MAX, 0);
          adcsim_run();
          tsim_run();

          // run battery simulation
          uint32_t active_power = batsim_run(&b, in.vout, t.genpower);  // simulated battery charges with generated output voltage (out.vout or in.vout they should be the same)
          acc_power += (active_power * BAT_SIM_CALL_RATE_ms) / 3600;
          // run brake simulation
          uint32_t wasted_power = brakesim_run(in.vin, pwmsim_getDuty(PWM_CH_BRAKE));

          // run wind turbine simulation
          turbinesim_run(&t, active_power + wasted_power);
          // log status every logInterval
          if(t_expired(&logTimer)){
              printf(" WS: %4.2f m/s, WP: %4.2f kW, GP: %3.2f kW, RPM: %4d, VIN: %5.2fV, VBAT: %4.2fV, VOUT: %4.2fV, Active Power: %4d W, Stored Power: %4d Wh, Bat: %4.2f%%  \r", ((float)t.windspeed) / 100, ((float)t.windpower) / 1000, ((float)t.genpower) / 1000, t.rpm, ((float)in.vin) / 100, ((float)in.vbat) / 100, ((float)in.vout) / 100, active_power, acc_power / 1000, (float)b.soc / 100);
              fflush(NULL);
              t_start(&logTimer);
          };
          t_start(&cycleTimer);
        }
    };
    
    return 0;    
};