#pragma once

#include <stdint.h>


#define AVG_FILTER_DEPTH (10)

/// @brief Average Filter - A filter that returns the average of the previous 10 samples that were added
typedef struct AVG_F {
    uint16_t buffer[AVG_FILTER_DEPTH];
    uint32_t sum;
    uint8_t  lastValueIndex;
} AVG_Filter_t;


/// @brief   - initializes an average filter as zero
/// @param f - the filter to be initialized
void avgf_init(AVG_Filter_t* f);

/// @brief       - Adds a sample to an average filter
/// @param f     - filter to add the sample to
/// @param value - value of the sample to be added to the filter 
void avgf_addSample(AVG_Filter_t* f, uint16_t value);

/// @brief   - returns the current value calculated by the average filter
/// @param f - the avg filter
/// @return  - the average value
uint16_t avgf_value(AVG_Filter_t* f);