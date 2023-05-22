/// \file   batsim.h
/// \author Bernardo Marques
/// \date   2023-05
///
/// \brief  Battery Simulator - simulates a battery charging

#pragma once

#include <stdint.h>

#define BAT_SIM_CALL_RATE_ms (100)      // rate that bat_sim is called

typedef struct BATTERY {
    uint64_t charge;        // battery charge in uAh
    uint16_t voltage;       // battery voltage in V * 100
    uint16_t soc;           // state of charge in % * 100
} Battery_t;

/// \brief  initializes the battery
/// \param bat  the battery to be initialized
void batsim_init(Battery_t* bat);

/// \brief Battery Simulation - charges the battery and returns consumed power (in Watts) 
/// \param bat     the battery to be charged
/// \param vcharge battery charge voltage
/// \param available_power maximum power available on the charge voltage source
/// \return consumed power
uint32_t batsim_run(Battery_t* bat, uint16_t vcharge, uint32_t avilable_power);


