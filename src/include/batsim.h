/// \file   batsim.h
/// \author Bernardo Marques
/// \date   2023-05
///
/// \brief  Battery Simulator - simulates a battery charging

#pragma once

#include <stdint.h>

#define BAT_SIM_CALL_RATE_ms (100)      // rate that bat_sim is called

/// @brief  initializes the battery
void batsim_init(void);

/// @brief Battery Simulation - charges the battery and returns consumed power (in Watts) 
/// @param vcharge battery charge voltage
/// @return consumed power
uint16_t batsim_run(uint16_t vcharge);

/// @brief returns battery voltage in centivolt (V* 100) - meaning 9876 > 98.76V
/// @return battery voltage
uint16_t batsim_voltage();

/// @brief returns battery charge level in % (0 ~ 100) * 100 - meaning: 1234 => 12.34%
/// @return charge level
uint16_t batsim_charge();

