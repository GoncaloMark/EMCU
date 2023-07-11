/// \file   mem.c
/// \author Gonçalo Marques
/// \date   2023-05

#include <stdint.h>

static uint8_t mem[4096] = {};

void reg_write(uint16_t addr, uint8_t value){
    addr &= 0x0FFF;
    mem[addr] = value;
    //printf("WRITE - addr: %d value: %d \n", addr, value);
};

uint8_t reg_read(uint16_t addr){
    addr &= 0x0FFF;
    //printf("READ - addr: %d value: %d \n", addr, mem[addr]);
    return mem[addr];
};

void reg_set(uint16_t addr, uint8_t bits){
    uint8_t reg_val = reg_read(addr);
    reg_val |= bits;
    reg_write(addr, reg_val);
}

void reg_clear(uint16_t addr, uint8_t bits){
    uint8_t reg_val = reg_read(addr);
    reg_val &= ~bits;
    reg_write(addr, reg_val);
}

