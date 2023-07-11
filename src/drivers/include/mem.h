/// \file   mem.h
/// \author Gonçalo Marques
/// \date   2023-05

#pragma once
#include <stdint.h>

/// @brief          - A function to write to memory registers (emulated)
/// @param addr     - An address to write to
/// @param value    - The value to write to the address
void reg_write(uint16_t addr, uint8_t value);

/// @brief          - A function to read from memory registers (emulated)
/// @param addr     - An address to read from
/// @return         - Returns the value read from the address
uint8_t reg_read(uint16_t addr);

void reg_set(uint16_t addr, uint8_t bits);

void reg_clear(uint16_t addr, uint8_t bits);
