/// \file   brakesim.h
/// \author Bernardo Marques
/// \date   2023-05
///
/// \brief  Brake Simulation - reduces input voltage proportionally to brake value

#pragma once

#include <stdint.h>

/// \brief Brake Simulation - returns the value for input voltage affected by brake value
/// \param vin    - current value for input voltage
/// \param brake  - brake value in %  (0 to 100) of time (PWM controlled)
/// \return the value for input voltage with brake applied
uint16_t brakesim(uint16_t vin, uint8_t brake) {
    return ((100 - brake) * vin) / 100;
}