/// \file   turbinesim.h
/// \author Bernardo Marques
/// \date   2023-05
///
/// \brief  Wind Turbine Simulation - causes input voltage to vary according to wind, output power and brake
///
/// source for wind turbine formulas:
/// https://www.omnicalculator.com/ecology/wind-turbine#how-to-calculate-the-power-generated-by-a-wind-turbine
/// and TESUP Magnum 5 user manual:
/// https://drive.google.com/file/d/1lSoAM47hA4uERMrflJ-tpXlLmjjHVzff/view
///

#pragma once

#include <stdint.h>


typedef struct Turbine {
    uint16_t windspeed;     // windspeed in m/s * 100
    uint32_t windpower;     // wind power in W
    uint32_t genpower;      // generator power in W
    uint16_t rpm;           // rotations per minute
    uint16_t vout;          // generated voltage in cV (V * 100)
} Turbine_t;

void turbinesim_init(Turbine_t* t);                         // initialize turbine
void turbinesim_run(Turbine_t *t, uint32_t consumedPower);  // run turbine simulation
