/// \file   turbinesim.c
/// \author Bernardo Marques
/// \date   2023-05
///
/// \brief  Wind Turbine Simulation - causes voltage to vary according to wind, ogenerated power and consumed power
///
/// source for wind turbine formulas:
/// https://www.omnicalculator.com/ecology/wind-turbine#how-to-calculate-the-power-generated-by-a-wind-turbine
/// and TESUP Magnum 5 user manual:
/// https://drive.google.com/file/d/1lSoAM47hA4uERMrflJ-tpXlLmjjHVzff/view
///

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "include/windsim.h"
#include "include/avg_filter.h"
#include "include/turbinesim.h"

// constants
#define AIR_DENSITY   (119)  // air density in Kg/m³ * 100
#define PI            (314)  // pi * 100

// turbine parameter
#define LOW_WIND     (1000)  // Low/High wind treshold in m/s * 100
#define LOW_WIND_TSR  (312)  // tip Speed Ratio = (Tip Speed of Blade / Wind Speed) * 100
#define HIGH_WIND_TSR (654)  // tip Speed Ratio = (Tip Speed of Blade / Wind Speed) * 100
#define TURBINE_EFF    (20)  // turbine effiecincy in %
#define ROTOR_DIAM    (235)  // turbine rotor diameter
#define MAX_RPM      (1500)  // turbine maximum rpm
#define MIN_WIND      (325)  // minimum wind for the turbine to rotate in m/s * 100
#define MAX_WIND     (5000)  // turbine maximum wind speed 50 m/s * 100
#define RPM_TOLERANCE  (10)  // wind turbine tolerance to go above specs
#define TOLERANCE(RPM)   (((RPM) * RPM_TOLERANCE) / 100)
#define RPM_TO_VOLT(RPM) ((RPM) * 10) // RPM to Voltage conversion - we'll say that Voltage = RPM / 10 (hence V * 100 = RPM * 10)
#define VOLT_TO_RPM(V)   ((V) / 10)   // Voltage to RPM conversion - RPM = Voltage * 10 (hence RPM = V * 100 / 10)

static AVG_Filter_t rpm_filter;         // simulates wind turbine inertia

void turbinesim_init(Turbine_t* t) {
    t->windspeed = 0;
    t->windpower = 0;
    t->genpower = 0;
    t->rpm = 0;
    t->vout = 0;
    avgf_init(&rpm_filter);
}

void turbinesim_run(Turbine_t *t, uint32_t consumedPower) {
    if (t == NULL) return;

    // run wind simulation
    windsim_run();
    // get windspeed
    t->windspeed = windsim_windspeed();
    // calculate wind power
    // power: P = 0.5 * p * pi * R² * V³ where p is air density in Kg/m3, R is blade length in meters and V is wind speed in meters per second
    uint64_t windspeed_cubed = ((uint64_t) t->windspeed * t->windspeed * t->windspeed) / (100 * 100);
    
    // wind mechanical power in W
    t->windpower = (windspeed_cubed * ROTOR_DIAM * ROTOR_DIAM * PI * AIR_DENSITY) / ((uint64_t)8 * 100 * 100 * 100 * 100 * 100);
    
    // Calculate turbine speed
    // low winds (< 3 m/s) do not have enough power to rotate the turbine, speed is zero
    uint16_t rpm = 0;
    if (t->windspeed < MIN_WIND) {
        t->genpower = 0;
    } else {
        // if no power is being consumed calculate turbine speed from TSR
        if (consumedPower == 0 || (t->vout == 0)) {
            if (t->windspeed <= LOW_WIND)
                rpm = (60 * t->windspeed * LOW_WIND_TSR) / (PI * ROTOR_DIAM);
            else
                rpm = (60 * t->windspeed * HIGH_WIND_TSR) / (PI * ROTOR_DIAM);
        } else {
            // power is being drawn from the turbine - calculate voltage and current
            // P = V * I
            // voltage is proportional to speed
            // current is proportional to torque - requiring force from turbine, similar to braking
            // calculate consumed current from consumed power
            uint32_t current_mA = (consumedPower * 100 * 1000) / t->vout;
            // calculate output voltage from power generated with consumed current
            uint16_t vout = (t->genpower * 1000) / current_mA;
            // calculate turbine rpm with calculated voltage
            rpm = VOLT_TO_RPM(vout);
        }
        // generate electrical power from wind = turbine efficiency * wind power
        t->genpower  = (t->windpower * TURBINE_EFF) / 100;
    }

    avgf_addSample(&rpm_filter, rpm);
    t->rpm = avgf_value(&rpm_filter);
    t->vout = RPM_TO_VOLT(t->rpm);
    

    static uint8_t warned = false;

    if (t->rpm > MAX_RPM && (!warned)) {
        printf("\n *** WARNING: Turbine is rotating too fast, risk of mechanical damage to the turbine and excessive input voltage ***\n");
        warned = true;
    } else if(t->rpm < (MAX_RPM - TOLERANCE(MAX_RPM))) {
        warned = false;
    }

    if (t->rpm > (MAX_RPM + TOLERANCE(MAX_RPM)))
    {
        printf("\n *** ERROR: Turbine rotation too fast. Turbine broken by excessive mechanical force and converter destroyed by excessive voltage. ***\n");
        printf("\n \t- try to brake the turbine before it breaks \n");
        exit(-1);
    }
}