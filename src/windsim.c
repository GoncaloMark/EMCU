#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include "include/clock.h"

typedef struct WIND {
    uint8_t AVG;        // Average Windspeed in m/s (0 to 20 ms/s)
    uint8_t DEVIATION;  // Wind deviation from avg value (in %)
    uint8_t DURATION;   // Timer that this wind pattern will endure (in minutes)
} WIND_t;

static const WIND_t WIND_SIMULATION[] = {
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
static uint16_t vin = 0;
static uint8_t  sim_idx = -1;
static uint16_t remaining_seconds = 0;
static Timer_t t = { 0, 1000 };         // timer to count 1s

static void windsim_init(void)
{
    srand(time(NULL));      // initialize random generator with random seed
}

enum WIND_TREND
{
    STABLE = 0,
    DOWN   = 1,
    UP     = 2
};


enum WIND_TREND trend = STABLE;

void changeNoiseTrend(void)
{
    trend = (enum WIND_TREND) (rand() % 2);
}

static int16_t AddNoise(uint16_t value)
{
    uint16_t noiseRange  = (WIND_SIMULATION[sim_idx].DEVIATION * value) / 100;
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

void windsim_run(void)
{

    // initialize wind simulation
    if (!initialized)
    {
        windsim_init();
        initialized = true;
    }

    // count seconds
    if (t_elapsed(&t))
    {
        if (remaining_seconds > 0) remaining_seconds--;
        t_start(&t);
    }

    // change noise trend 4 times during each wind profile
    uint16_t oneQuarter = (WIND_SIMULATION[sim_idx].DURATION * 60) / 4;
    if (remaining_seconds % oneQuarter == 0)
        changeNoiseTrend();

    // change wind profile
    if (remaining_seconds == 0)
    {
        sim_idx++;
        sim_idx %= sizeof(WIND_SIMULATION) / sizeof(WIND_t);
        remaining_seconds = WIND_SIMULATION[sim_idx].DURATION * 60;
    }
    uint16_t value = (WIND_SIMULATION[sim_idx].AVG * 150) / 20;

    vin = AddNoise(value);
}

uint16_t windsim_getInputVoltage(void)
{
    return vin;
}

