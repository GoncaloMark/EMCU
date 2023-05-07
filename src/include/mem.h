#pragma once
#include <stdint.h>

void write_reg(uint16_t addr, uint8_t value);
uint8_t read_reg(uint16_t addr);
