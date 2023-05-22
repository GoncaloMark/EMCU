/// \file   batsim.c
/// \author Bernardo Marques
/// \date   2023-05
///
/// \brief  Battery Simulator - simulates a battery charging

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/batsim.h"

#define BAT_C              (910)                 // battery capacitiy in Ah
#define BAT_VOLTAGE        (12)                  // battery nominal voltage in V
#define BAT_CAPACITY       (BAT_C * BAT_VOLTAGE) // battery capacity in Wh

#define BAT_MIN_VOLTAGE    (11 * 100)    // battery voltage when fully discharged in V * 100
#define BAT_MAX_VOLTAGE    (13 * 100)    // battery voltage when fully charged in V * 100
#define BAT_CHARGE_VOLTAGE (15 * 100)    // voltage at wich the battery charges with 0.1C A in V * 100
#define BAT_VOLTAGE_LIMIT  (16 * 100)    // voltage at wich the battery is permanently damaged in V * 100
#define BAT_START_CHARGE   (20)          // battery initial charge in %
#define VOLTAGE_TOLERANCE  (10)          // charge voltage admissible ripple in %
#define TOLERANCE(V)       ((V * VOLTAGE_TOLERANCE) / 100)

static uint16_t soc(uint64_t charge) {
    return charge / (BAT_C * 100);
}

static uint16_t voltage(uint64_t charge) {
    return BAT_MIN_VOLTAGE + ((charge / (BAT_C * 10000)) * (BAT_MAX_VOLTAGE - BAT_MIN_VOLTAGE)) / 100;
}

void batsim_init(Battery_t* bat) {
    bat->charge = ((BAT_START_CHARGE * BAT_C) / 100) * 1000000;
    bat->voltage = voltage(bat->charge);
    bat->soc     = soc(bat->charge);
}

uint32_t batsim_run(Battery_t* bat, uint16_t vcharge, uint32_t available_power) {
    static uint8_t warned = false;

    if ((vcharge > (BAT_CHARGE_VOLTAGE + TOLERANCE(BAT_CHARGE_VOLTAGE))) && (!warned)) {
        printf("\n *** WARNING: Battery charge voltage too high (%4.2f V), risk of causing flamable fumes and damage to the battery ***\n", (float)vcharge / 100);
        warned = true;
    } else if(vcharge < BAT_CHARGE_VOLTAGE) {
        warned = false;
    }

    if (vcharge > BAT_VOLTAGE_LIMIT + TOLERANCE(BAT_VOLTAGE_LIMIT))
    {
        printf("\n *** ERROR: Voltage too high (%4.2f V). Battery exploded, melted, burned, is no longer usable. ***\n", (float)vcharge / 100);
        exit(-1);
    }

    if (vcharge < bat->voltage) // battery only charges with voltage higher than battery voltage
    {
        printf("Consumed Power: 0 W (%f V / 0 A)\n", ((float)vcharge) / 100);
        return 0;
    }

    // calculate charge current in mA
    // Battery current is 0.1 * BAT_C for a voltage difference of BAT_CHARGE_VOLTAGE - BAT_VOLTAGE
    uint32_t current_mA = (((uint32_t)vcharge - bat->voltage) * BAT_C) / ((BAT_CHARGE_VOLTAGE / 100) - BAT_VOLTAGE);

    // current is limited by available power
    uint32_t maxCurrent_mA = (available_power * 100 * 1000) / vcharge;
    if (current_mA > maxCurrent_mA)
      current_mA = maxCurrent_mA;
    // calculate Ah - current * time (in hours)
    uint32_t charged_uAh = (current_mA * BAT_SIM_CALL_RATE_ms) / 3600;     // mA * ms / (s/h) = uAh
    bat->charge += charged_uAh;                                             // add charged current to battery
    // limit charge to maximum
    if (bat->charge > (BAT_C * 1000000))
      bat->charge = (BAT_C * 1000000);
    
    bat->voltage = voltage(bat->charge);
    bat->soc = soc(bat->charge);

    printf("Consumed Power: %d W (%f V / %f A)\n", (vcharge * current_mA) / (1000 * 100) + 1, ((float)vcharge) / 100, ((float) current_mA) / 1000);
    
    // return consumed power in W
    return (vcharge * current_mA) / (1000 * 100) + 1;       
}