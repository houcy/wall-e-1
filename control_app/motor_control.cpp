#include "motor_control.h"

MotorControl::MotorControl()
{
}

MotorControl::MotorControl(const std::string &gpioDir,
    const std::string &gpioRev, const std::string &gpioEn)
{
    gpioDirect.setNum(gpioDir);
    if (gpioDirect.gExport() || gpioDirect.setDir("out") ||
        gpioDirect.setVal("0"))
    {
        return;
    }

    gpioReverse.setNum(gpioRev);
    if (gpioReverse.gExport() || gpioReverse.setDir("out") ||
        gpioReverse.setVal("0"))
    {
        return;
    }

    gpioEnable.setNum(gpioEn);
    if (gpioEnable.gExport() || gpioEnable.setDir("out") ||
        gpioEnable.setVal("0"))
    {
        return;
    }
}

MotorControl::~MotorControl()
{
    gpioEnable.setVal("0");
    gpioEnable.gUnexport();

    gpioReverse.setVal("0");
    gpioReverse.gUnexport();

    gpioDirect.setVal("0");
    gpioDirect.gUnexport();
}

void MotorControl::directMove()
{
    if (gpioEnable.setVal("1") || gpioDirect.setVal("1") ||
        gpioReverse.setVal("0"))
    {
        return;
    }
}

void MotorControl::reverseMove()
{
    if (gpioEnable.setVal("1") || gpioDirect.setVal("0") ||
        gpioReverse.setVal("1"))
    {
        return;
    }
}

void MotorControl::block()
{
    if (gpioEnable.setVal("1") || gpioDirect.setVal("0") ||
        gpioReverse.setVal("0"))
    {
        return;
    }
}

void MotorControl::freeRun()
{
    if (gpioEnable.setVal("0") || gpioDirect.setVal("0") ||
        gpioReverse.setVal("0"))
    {
        return;
    }
}
