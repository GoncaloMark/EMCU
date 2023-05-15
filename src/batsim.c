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


static uint64_t charge;                  // battery charge in uAh

uint16_t batsim_charge() {
    return charge / (BAT_C * 100);
}

uint16_t batsim_voltage() {
    return BAT_MIN_VOLTAGE + ((charge / (BAT_C * 10000)) * (BAT_MAX_VOLTAGE - BAT_MIN_VOLTAGE)) / 100;
}

void batsim_init() {
    charge = ((BAT_START_CHARGE * BAT_C) / 100) * 1000000;
}

uint16_t batsim_run(uint16_t vcharge) {
    static uint8_t warned = false;

    if ((vcharge > (BAT_CHARGE_VOLTAGE + TOLERANCE(BAT_CHARGE_VOLTAGE))) && (!warned)) {
        printf("\n *** WARNING: Battery charge voltage too high, risk of causing flamable fumes and damage to the battery ***\n");
        warned = true;
    } else if(vcharge < BAT_CHARGE_VOLTAGE) {
        warned = false;
    }

    if (vcharge > BAT_VOLTAGE_LIMIT + TOLERANCE(BAT_VOLTAGE_LIMIT))
    {
        printf("\n *** ERROR: Voltage too high. Battery exploded, melted, burned, is no longer usable. ***\n");
        exit(-1);
    }

    // calculate charge current in mA
    // vcharge (V) * 0.1 * BAT_C (A) / BAT_CHARGE_VOLTAGE (V) would be current in Amps
    uint16_t current_mA = (((uint32_t)vcharge) * BAT_C) / (BAT_CHARGE_VOLTAGE / 100);
    // calculate Ah - current * time (in hours)
    uint32_t charged_uAh = (current_mA * BAT_SIM_CALL_RATE_ms) / 3600;     // mA * ms / (s/h) = uAh
    charge += charged_uAh;                                                 // add charged current to battery
    // limit charge to maximum
    if (charge > (BAT_C * 1000000))
      charge = (BAT_C * 1000000);
    // return consumed power in W
    return (BAT_VOLTAGE * current_mA) / 1000;       
}