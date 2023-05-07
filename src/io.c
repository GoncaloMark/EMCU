/// \file   io.h
/// \author Gonçalo Marques
/// \date   2023-04

#include "include/io.h"
#include "include/mem.h"
#define LATC_ADD  (0x010E)
#define TRISC_ADD (0x008E)

// io.c

void init_io(void) {
    write_reg(LATC_ADD, 0x0);
    write_reg(TRISC_ADD, 0x7);
}

void set_outputs(output_t* outputs){
    if(outputs == NULL) return;

    uint8_t read_out = read_reg(LATC_ADD);
    read_out &= ~(green | red | yellow );
    read_out |= (outputs->leds & (green | red | yellow));
    write_reg(LATC_ADD, read_out);
    //printf("read_out: %d", read_out);
}
