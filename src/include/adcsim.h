/// \file   adcsim.h
/// \author Gonçalo Marques
/// \date   2023-05

#pragma once

/// @brief  - Run the ADC simulation reading from the different channels
/// @param  - void
void adcsim_run(uint16_t vin, uint16_t vout, uint16_t vbat);