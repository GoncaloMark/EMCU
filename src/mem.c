/// \file   iosim.h
/// \author Gonçalo Marques
/// \date   2023-05

#include <stdint.h>

static uint8_t mem[4096] = {};

void write_reg(uint16_t addr, uint8_t value){
    addr &= 0x0FFF;
    mem[addr] = value;
    //printf("WRITE - addr: %d value: %d \n", addr, value);
};

uint8_t read_reg(uint16_t addr){
    addr &= 0x0FFF;
    //printf("READ - addr: %d value: %d \n", addr, mem[addr]);
    return mem[addr];
};
