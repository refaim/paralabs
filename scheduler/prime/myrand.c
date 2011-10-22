#include "myrand.h"

static number next = 1;

number randnum()
{
    next = next * 1103515245 + 12345;
    return next & NUMBER_MAX;
}

void setrandseed(number value)
{
    next = value;
}
