/// \file   turbine_control.c
/// \author Bernardo Marques
/// \date   2023-05

#include <stdint.h>
#include "drivers/include/io.h"

// TODO: write the software that controls the turbine
// turbine should be left freewheeling (no power being drained) up to a given voltage
// shall start production whenever the voltage goes higher than the predefined voltage
// shall search for the Maximum Power Point (maximum power going to batteries without slowing down the turbine):
//  - increase power to batteries (Vout) if turbine is accelerating (Vin increasing)
//  - reduce power to batteries if turbine is decelerating
//  - brake if turbine is accelerating above set voltage even with full power to the batteries
//    in such occasion wind turbine speed (and voltage) shall be reduced so that charging the battery is 
//    able to brake the turbine (due to higher pwm duty cycle because Vin is lower) and keep it under control
// if not possible to keep the turbine under control (too much wind or battery fully charged) the turbine shall be
// brought to a ful stop for safety (constant braking may burn the braking resistor)
void turbine_control(input_t in, output_t* out) {
  // this is a simple control loop just for testing while actual control is not implemented
  if (in.vin > 12) {
    out->leds |= green;
  } else {
    out->leds &= ~green;
  }

  if(in.vin > 3600) {
    out->vout = 1300;
    out->leds |= yellow;
  } else {
    out->vout = 0;
    out->leds &= ~yellow;
  }
}