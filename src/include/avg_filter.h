/// \file   avg_filter.h
/// \author Bernardo Marques
/// \date   2023-05
///
/// \brief  Average Filter - calculates the average of the last AVG_FILTER_DEPTH samples
#pragma once

#include <stdint.h>


/// \brief Average Filter - A filter to calculate the average of the last AVG_FILTER_DEPTH samples
typedef struct AVG_F {
    uint16_t buffer[AVG_FILTER_DEPTH];
    uint32_t sum;
    uint16_t  lastValueIndex;
} AVG_Filter_t;


/// \brief   - initializes an average filter as zero
/// \param f - the filter to be initialized
static void avgf_init(AVG_Filter_t* f);

/// \brief       - adds a sample to an average filter
/// \param f     - filter to add the sample to
/// \param value - value of the sample to be added to the filter 
static void avgf_addSample(AVG_Filter_t* f, uint16_t value);

/// \brief   - returns the average value
/// \param f - the avg filter
/// \return  - the average value
static uint16_t avgf_value(AVG_Filter_t* f);

static void avgf_init(AVG_Filter_t* f)
{
    if (f == NULL) return;
    for (int i = 0; i < AVG_FILTER_DEPTH; i++)
        f->buffer[i] = 0;
    f->sum = 0;
    f->lastValueIndex = 0;
}

static void avgf_addSample(AVG_Filter_t* f, uint16_t value)
{
    uint16_t idx = f->lastValueIndex;
    uint16_t lastValue = f->buffer[idx];
    f->buffer[idx] = value;
    f->lastValueIndex++;
    f->lastValueIndex %= AVG_FILTER_DEPTH;
    f->sum += value - lastValue;
}

static uint16_t avgf_value(AVG_Filter_t* f)
{
    return f->sum / AVG_FILTER_DEPTH;
}