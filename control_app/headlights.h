#ifndef HEADLIGHTS_H
#define HEADLIGHTS_H

#include "gpio.h"

class Headlights
{
    Gpio gpioHeadlights;

public:
    Headlights();
    ~Headlights();
    int on();
    int off();
};

#endif // HEADLIGHTS_H
