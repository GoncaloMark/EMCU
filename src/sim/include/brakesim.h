/// \file   brakesim.h
/// \author Bernardo Marques
/// \date   2023-05
///
/// \brief  Brake Simulation - consumes power from turbine according to brake value and resistor

#pragma once

#include <stdint.h>

#define BRAKE_RESISTANCE     (8)  // brake resistance in Ohms
#define BRAKE_POWER_RATING (500)  // brake resistor power rating in W


/// \brief Brake Simulation - returns the value for input voltage affected by brake value
/// \param vin    - current value for input voltage
/// \return       - power consumed by the brake resistor
uint32_t brakesim_run(uint16_t vin, uint16_t brake_pwm);