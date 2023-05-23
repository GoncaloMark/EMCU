/// \file   turbine_control.h
/// \author Bernardo Marques
/// \date   2023-05

#pragma once

#include <stdint.h>
#include "drivers/include/io.h"

/// @brief Wind Turbine Controller software
/// @param in values on the inputs
/// @param out output parameter with values for the outputs
void turbine_control(input_t in, output_t* out);
