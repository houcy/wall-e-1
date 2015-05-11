#include "headlights.h"

#define GPIO_HEADLIGHTS_ON GPIO_PA9

Headlights::Headlights()
{
    gpioHeadlights.setNum(GPIO_HEADLIGHTS_ON);
    if (gpioHeadlights.gExport() || gpioHeadlights.setDir("out") ||
        gpioHeadlights.setVal("0"))
    {
        return;
    }
}

Headlights::~Headlights()
{
    gpioHeadlights.setVal("0");
    gpioHeadlights.gUnexport();
}

int Headlights::on()
{
    return gpioHeadlights.setVal("1");
}

int Headlights::off()
{
    return gpioHeadlights.setVal("0");
}
