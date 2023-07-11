/// \file   adcsim.h
/// \author Gonçalo Marques
/// \date   2023-05

#pragma once

#include <stdint.h>

/// \brief Run the ADC simulation - writes analog inputs values to ADC different channels
void adcsim_run();

/// @brief Set a value to a given ADC channel
/// @param channel ADC channel to set value
/// @param signalValues analog input value
/// @param signalMax analog input maximum possible value
/// @param signalMin analog input minimum possible value
void adcsim_setSignal(uint16_t channel, uint16_t signalValue, uint16_t signalMax, uint16_t signalMin);