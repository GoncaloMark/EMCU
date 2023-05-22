/// \file   avg_filter.h
/// \author Bernardo Marques
/// \date   2023-05
///
/// \brief  Average Filter - calculates the average of the last AVG_FILTER_DEPTH samples
#pragma once

#include <stdint.h>


#define AVG_FILTER_DEPTH (100)

/// \brief Average Filter - A filter to calculate the average of the last AVG_FILTER_DEPTH samples
typedef struct AVG_F {
    uint16_t buffer[AVG_FILTER_DEPTH];
    uint32_t sum;
    uint8_t  lastValueIndex;
} AVG_Filter_t;


/// \brief   - initializes an average filter as zero
/// \param f - the filter to be initialized
void avgf_init(AVG_Filter_t* f);

/// \brief       - adds a sample to an average filter
/// \param f     - filter to add the sample to
/// \param value - value of the sample to be added to the filter 
void avgf_addSample(AVG_Filter_t* f, uint16_t value);

/// \brief   - returns the average value
/// \param f - the avg filter
/// \return  - the average value
uint16_t avgf_value(AVG_Filter_t* f);