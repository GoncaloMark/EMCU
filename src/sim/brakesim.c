/// \file   brakesim.c
/// \author Bernardo Marques
/// \date   2023-05
///
/// \brief  Brake Simulation - consumes power from turbine according to brake value and resistor

#include <stdint.h>

#define BRAKE_RESISTANCE     (8)  // brake resistance in Ohms
#define BRAKE_POWER_RATING (500)  // brake resistor power rating in W

uint32_t brakesim_run(uint16_t vin)
{
    // TODO: read brake duty cycle from PWM registers
    uint8_t brake_pwm = 0;
    // P = V * I => P = V² / R
    uint32_t power = ((uint32_t)vin * vin * brake_pwm) / (100 * 100 * BRAKE_RESISTANCE);

    return power;
}