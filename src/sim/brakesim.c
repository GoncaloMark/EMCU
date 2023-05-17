/// \file   brakesim.c
/// \author Bernardo Marques
/// \date   2023-05
///
/// \brief  Brake Simulation - consumes power from turbine according to brake value and resistor

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define BRAKE_RESISTANCE     (8)  // brake resistance in Ohms
#define BRAKE_POWER_RATING (500)  // brake resistor power rating in W
#define TOLERANCE (BRAKE_POWER_RATING * 10 / 100)   // resistor power tolerance

static bool resistor_ok = true;

uint32_t brakesim_run(uint16_t vin) {
    static bool warned = false;

    if (!resistor_ok)
        return 0;
    // TODO: read brake duty cycle from PWM registers
    uint8_t brake_pwm = 0;
    // P = V * I => P = V² / R
    uint32_t power = ((uint32_t)vin * vin * brake_pwm) / (100 * 100 * BRAKE_RESISTANCE);

    if (power > BRAKE_POWER_RATING && !warned) {
        warned = true;
        printf("\n*** WARNING: Brake power too high (%d W), risk of burning brake resistor ***\n", power);
    } else if (power < (BRAKE_POWER_RATING - TOLERANCE)) {
        warned = false;
    }

    if (power > BRAKE_POWER_RATING + TOLERANCE) {
        printf("\n*** ERROR: Brake resistor burned due to high power (%d W). Brake resistor no longer working. ***\n", power);
        resistor_ok = false;
    }


    return power;
}