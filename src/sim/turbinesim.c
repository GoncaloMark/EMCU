/// \file   turbinesim.c
/// \author Bernardo Marques
/// \date   2023-05
///
/// \brief  Wind Turbine Simulation - causes voltage to vary according to wind, ogenerated power and consumed power
///
/// source for wind turbine formulas:
/// https://www.omnicalculator.com/ecology/wind-turbine#how-to-calculate-the-power-generated-by-a-wind-turbine
/// and TESUP Magnum 5 user manual:
/// https://drive.google.com/file/d/1lSoAM47hA4uERMrflJ-tpXlLmjjHVzff/view
///

#define AVG_FILTER_DEPTH  (300)

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "include/windsim.h"
#include "include/avg_filter.h"
#include "include/turbinesim.h"

// constants
#define AIR_DENSITY   (119)  // air density in Kg/m³ * 100
#define PI            (314)  // pi * 100

// turbine parameters
#define ROTOR_DIAM    (235)  // turbine rotor diameter
#define MAX_RPM      (1500)  // turbine maximum rpm
#define MAX_WIND     (5000)  // turbine maximum wind speed 50 m/s * 100
#define RPM_TOLERANCE  (10)  // wind turbine tolerance to go above specs
#define TOLERANCE(RPM)   (((RPM) * RPM_TOLERANCE) / 100)


static AVG_Filter_t rpm_filter;         // simulates wind turbine inertia

typedef struct POWER_POINT {
    uint16_t  windspeed;                // windspeed * 100
    uint16_t rpm;                       // rpm
    uint16_t genpower;                  // power in W
    uint16_t voltage;                   // voltage * 100
} PowerPoint_t;

// power curve extracted from: https://www.youtube.com/watch?v=ZSjJapo2hWo
static const PowerPoint_t POWER_CURVE[] = {
  { 3 * 100,   10,   70,  2 * 100},
  { 4 * 100,   42,  301, 12 * 100},
  { 5 * 100,   78,  620, 24 * 100},
  { 6 * 100,  114,  800, 36 * 100},
  { 8 * 100,  181, 1180, 48 * 100},
  { 9 * 100,  300, 1702, 50 * 100},
  {11 * 100,  432, 2350, 52 * 100},
  {12 * 100,  567, 3059, 54 * 100},
  {13 * 100,  652, 3560, 56 * 100},
  {14 * 100,  716, 4195, 58 * 100},
  {16 * 100,  751, 5000, 62 * 100},
  {17 * 100,  760, 5200, 66 * 100},
};

#define POWER_POINT_COUNT (sizeof(POWER_CURVE) / sizeof(PowerPoint_t))

/// @brief Returns turbine power curve data for a given windspeed
/// @param windspeed windspeed in m/s * 100
/// @return generated power, rpm and voltage for the given windspeed
static const PowerPoint_t turbinePower(uint16_t windspeed) {
  PowerPoint_t p = {windspeed, 0, 0, 0 };
  uint8_t i = 0;
  for (i = 0; i < POWER_POINT_COUNT; i++) {
    if (POWER_CURVE[i].windspeed > windspeed)
      break;
  }
  // if windspeed is lower than minimum no power, no rotation and no voltage is generated
  if (i == 0) return p;
  // if windspeed is greater than maximum increase rpm and voltage linearly, generated power is limited to the maximum value
  if (i == POWER_POINT_COUNT) {
    p.genpower =  POWER_CURVE[i-1].genpower;
    p.rpm      = ((uint32_t)POWER_CURVE[i-1].rpm     * windspeed) / POWER_CURVE[i-1].windspeed;
    p.voltage  = ((uint32_t)POWER_CURVE[i-1].voltage * windspeed) / POWER_CURVE[i-1].windspeed;
    return p;
  }
  // calculate rpm, power and voltage for windspeed based on the power curve
  uint16_t speed_diff = windspeed - POWER_CURVE[i-1].windspeed;
  uint16_t curve_diff = POWER_CURVE[i].windspeed - POWER_CURVE[i-1].windspeed;
  p.genpower = POWER_CURVE[i-1].genpower + ((POWER_CURVE[i].genpower - POWER_CURVE[i-1].genpower) * speed_diff) / curve_diff;
  p.rpm      = POWER_CURVE[i-1].rpm      + ((POWER_CURVE[i].rpm      - POWER_CURVE[i-1].rpm)      * speed_diff) / curve_diff;
  p.voltage  = POWER_CURVE[i-1].voltage  + ((POWER_CURVE[i].voltage  - POWER_CURVE[i-1].voltage)  * speed_diff) / curve_diff;
  return p;
}


/// @brief Returns turbine power curve data for a given rpm
/// @param rpm turbine rotations per minute
/// @return    power and voltage proportional to the given rpm
static const PowerPoint_t rpmPower(uint16_t rpm) {
  PowerPoint_t p = {0, rpm, 0, 0 };
  uint8_t i = 0;
  for (i = 0; i < POWER_POINT_COUNT; i++) {
    if (POWER_CURVE[i].rpm > rpm)
      break;
  }
  // if rpm is lower than minimum no power, no rotation and no power or voltage is generated
  if (i == 0) return p;
  // if rpm is greater than maximum increase voltage linearly, generated power is limited to the maximum value
  if (i == POWER_POINT_COUNT) {
    p.genpower =  POWER_CURVE[i-1].genpower;
    p.voltage  = (POWER_CURVE[i-1].voltage * rpm) / POWER_CURVE[i-1].rpm;
    return p;
  }

  // calculate power and voltage for windspeed based on the power curve
  uint16_t rpm_diff = rpm - POWER_CURVE[i-1].rpm;
  uint16_t curve_diff = POWER_CURVE[i].rpm - POWER_CURVE[i-1].rpm;
  p.genpower = POWER_CURVE[i-1].genpower + ((POWER_CURVE[i].genpower - POWER_CURVE[i-1].genpower) * rpm_diff) / curve_diff;
  p.voltage  = POWER_CURVE[i-1].voltage  + ((POWER_CURVE[i].voltage  - POWER_CURVE[i-1].voltage)  * rpm_diff) / curve_diff;
  return p;
}

void turbinesim_init(Turbine_t* t) {
  t->windspeed = 0;
  t->windpower = 0;
  t->genpower = 0;
  t->rpm = 0;
  t->vout = 0;
  avgf_init(&rpm_filter);
}

void turbinesim_run(Turbine_t *t, uint32_t consumedPower) {
  if (t == NULL) return;

  // run wind simulation
  windsim_run();
  // get windspeed
  t->windspeed = windsim_windspeed();
  // calculate wind power
  // power: P = 0.5 * p * pi * R² * V³ where p is air density in Kg/m3, R is blade length in meters and V is wind speed in meters per second
  uint64_t windspeed_cubed = ((uint64_t) t->windspeed * t->windspeed * t->windspeed) / (100 * 100);
  
  // wind mechanical power in W
  t->windpower = (windspeed_cubed * ROTOR_DIAM * ROTOR_DIAM * PI * AIR_DENSITY) / ((uint64_t)8 * 100 * 100 * 100 * 100 * 100);

  // Calculate turbine speed, power and voltage derived from wind
  PowerPoint_t p = turbinePower(t->windspeed);

  // brake wind turbine speed according to power drawn
  if (t->genpower > 0) {
    if (consumedPower > t->genpower) consumedPower = t->genpower;                           // not possible to consume more than is generated
    uint16_t accell = (((uint32_t)t->genpower - consumedPower) * 100 * 100) / t->genpower;  // power percentage left for accelerating (in % * 100)
    uint16_t brake  = (100 * 100) - accell;                                                 // power percentage spent braking (in % *100)
    uint16_t rpm = t->rpm - (t->rpm * brake) / (100 * 100);                                 // turbine rotation is reduced by consumed power
    p.rpm = rpm + (p.rpm * accell) / (100 * 100);                                           // turbine accelerates while no power is being consumed
  }
  
  avgf_addSample(&rpm_filter, p.rpm);                                                       // simulate turbine inertia with an average filter on rpm
  t->rpm = avgf_value(&rpm_filter);
  // calculate power and voltage for the resulting rpm
  p = rpmPower(t->rpm);
  t->genpower = p.genpower;
  t->vout = p.voltage;
  

  static uint8_t warned = false;

  if (t->rpm > MAX_RPM && (!warned)) {
      printf("\n *** WARNING: Turbine is rotating too fast (%d rpm), risk of mechanical damage to the turbine and excessive input voltage ***\n", t->rpm);
      warned = true;
  } else if(t->rpm < (MAX_RPM - TOLERANCE(MAX_RPM))) {
      warned = false;
  }

  if (t->rpm > (MAX_RPM + TOLERANCE(MAX_RPM)))
  {
      printf("\n *** ERROR: Turbine rotation too fast (%d rpm). Turbine broken by excessive mechanical force and converter destroyed by excessive voltage. ***\n", t->rpm);
      printf("\n \t- try to brake the turbine before it breaks \n");
      exit(-1);
  }
}
