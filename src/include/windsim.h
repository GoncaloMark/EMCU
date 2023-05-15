/// \file   windsim.h
/// \author Bernardo Marques
/// \date   2023-05
///
/// \brief  Wind Simulation - causes input voltage to vary according to a given wind profile

#pragma once

#include <stdint.h>

/// @brief Run Wind Simulation with the hardcoded wind profile
/// @return wind speed in cm/s (m/s * 100)
uint16_t windsim_run(void);

/// @brief Returns current windspeed in cm/s (m/s * 100)
/// @return wind speed
uint16_t windsim_windspeed(void);

/// @brief Returns available wind power in cW (W * 100)
/// @return wind power
uint16_t windsim_windpower(void);