#include <stddef.h>
#include "include/avg_filter.h"

void avgf_init(AVG_Filter_t* f)
{
    if (f == NULL) return;
    for (int i = 0; i < AVG_FILTER_DEPTH; i++)
        f->buffer[i] = 0;
    f->sum = 0;
    f->lastValueIndex = 0;
}

void avgf_addSample(AVG_Filter_t* f, uint16_t value)
{
    uint8_t idx = f->lastValueIndex;
    uint16_t lastValue = f->buffer[idx];
    f->buffer[idx] = value;
    f->lastValueIndex++;
    f->lastValueIndex %= AVG_FILTER_DEPTH;
    f->sum += value - lastValue;
}

uint16_t avgf_value(AVG_Filter_t* f)
{
    return f->sum / AVG_FILTER_DEPTH;
}