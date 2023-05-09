/// \file   mem.h
/// \author Gonçalo Marques
/// \date   2023-05

#pragma once
#include <stdint.h>

/// @brief          - A function to write to memory registers (emulated)
/// @param addr     - An address to write to
/// @param value    - The value to write to the address
void write_reg(uint16_t addr, uint8_t value);

/// @brief          - A function to read from memory registers (emulated)
/// @param addr     - An address to read from
/// @return         - Returns the value read from the address
uint8_t read_reg(uint16_t addr);
