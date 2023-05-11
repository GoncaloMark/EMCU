/// \file   windsim.c
/// \author Bernardo Marques
/// \date   2023-05

#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include "include/timer.h"

#define MAX_WIND     (20)   // maximum possible wind speed (in m/s)
#define MAX_VOLTAGE  (150)  // turbine voltage at maximum wind speed (in V)
#define BLADE_LENGTH (150)  // turbine blade length in cm
#define AIR_DENSITY  (119)  // air density in Kg/m³ * 100
#define PI           (314)  // pi * 100

typedef struct WIND {
    uint8_t AVG;        // Average Windspeed in m/s (0 to 20 ms/s)
    uint8_t DEVIATION;  // Wind deviation from avg value (in %)
    uint8_t DURATION;   // Timer that this wind pattern will endure (in minutes)
} WIND_t;

static const WIND_t WIND_PROFILE[] = {
{ 5, 40, 30},        // 5 m/s wind, 40% variation, for 30 minutes
{ 7, 30, 20},        // 7 m/s wind, 10% variation, for 20 minutes
{10, 15, 15},        // 10 m/s wind, 15% variation, for 15 minutes
{ 7, 35,  5},        // 7 m/s wind, 35% variation, for 5 minutes
{12, 50, 20},        // 12 m/s wind, 50% variation, for 20 minutes
{16, 70, 10},        // 16 m/s wind, 70% variation, for 10 minutes
{12, 20, 20},        // 12 m/s wind, 20% variation, for 20 minutes
{ 6, 30, 30},        // 6 m/s wind, 30% variation, for 30 minutes
};

static bool initialized = false;
static uint16_t windspeed = 0;
static uint8_t  sim_idx = -1;
static uint16_t remaining_seconds = 0;
static Timer_t t = { 0, 1000 };         // timer to count 1s

static uint16_t wind_power(uint16_t windspeed)
{
    // wind power: P = 0.5 * Cp * p * pi * R² * V³ where Cp is efficiency, p is air density in Kg/m3, R is blade length in meters and V is wind speed in meters per second
    uint32_t windspeed_cubed = (uint32_t)((((uint64_t)windspeed) * windspeed * windspeed) / (100 * 100));
    uint8_t bladelength_squared = (((uint16_t)BLADE_LENGTH) * BLADE_LENGTH) / 100;
    uint16_t power_cW = ((windspeed_cubed * bladelength_squared) / 100  * ((10 * PI * AIR_DENSITY) / (100 * 100))) / 100;
    return power_cW;
}

static void windsim_init(void)
{
    srand(time(NULL));      // initialize random generator with random seed
}

enum NOISE_TREND
{
    STABLE = 0,
    DOWN   = 1,
    UP     = 2
};


enum NOISE_TREND trend = STABLE;

void changeNoiseTrend(void)
{
    trend = (enum NOISE_TREND) (rand() % 2);
}

static int16_t AddNoise(uint16_t value)
{
    uint16_t noiseRange  = (WIND_PROFILE[sim_idx].DEVIATION * value) / 100;
    uint16_t noiseOffset = 0;

    if (trend == STABLE)
    {
        noiseOffset = noiseRange;
        noiseRange *= 2;
    }
    if (trend == DOWN)
    {
        noiseOffset = noiseRange;
    }

    int16_t noise = (rand() % noiseRange) - noiseOffset;

    return value + noise;
}

void windsim_run(uint16_t consumed_power)
{

    // initialize wind simulation
    if (!initialized)
    {
        windsim_init();
        initialized = true;
    }

    // count seconds
    if (t_expired(&t))
    {
        if (remaining_seconds > 0) remaining_seconds--;
        t_start(&t);
    }

    // change wind profile
    if (remaining_seconds == 0)
    {
        sim_idx++;
        sim_idx %= sizeof(WIND_PROFILE) / sizeof(WIND_t);
        remaining_seconds = WIND_PROFILE[sim_idx].DURATION * 60;
    }

    // change noise trend 4 times during each wind profile
    uint16_t oneQuarter = (WIND_PROFILE[sim_idx].DURATION * 60) / 4;
    if ((remaining_seconds % oneQuarter) == 0)
        changeNoiseTrend();

    windspeed = AddNoise(WIND_PROFILE[sim_idx].AVG * 100);
}

uint16_t windsim_windspeed(void)
{
    return windspeed;
}

uint16_t windsim_windpower(void)
{
    return wind_power(windspeed);
}

