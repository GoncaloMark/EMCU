/// \file   io.c
/// \author Gonçalo Marques
/// \date   2023-05

#include <stddef.h>
#include "include/io.h"
#include "include/mem.h"
#define LATC_ADD  (0x010E)
#define TRISC_ADD (0x008E)

// io.c

void init_io(void) {
    reg_write(LATC_ADD, 0x0);
    reg_set(TRISC_ADD, 0x7);
}

void set_outputs(output_t* outputs){
    if(outputs == NULL) return;

    uint8_t read_out = reg_read(LATC_ADD);
    read_out &= ~(green | red | yellow );
    read_out |= (outputs->leds & (green | red | yellow));
    reg_write(LATC_ADD, read_out);
    //printf("read_out: %d", read_out);
}
