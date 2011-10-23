#include "myrand.h"

static number next = 1;

number randnum(void)
{
    next = next * 1103515245 + 12345;
    return next & NUMBER_MAX;
}

void setrandseed(number value)
{
    next = value;
}
