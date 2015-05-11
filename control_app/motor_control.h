#ifndef MOTOR_H
#define MOTOR_H

#include "gpio.h"

class MotorControl
{
    Gpio gpioDirect;
    Gpio gpioReverse;
    Gpio gpioEnable;

public:
    MotorControl();
    MotorControl(const std::string &gpioDir, const std::string &gpioRev,
        const std::string &gpioEn);
    ~MotorControl();
    MotorControl(const MotorControl &) = delete;
    MotorControl &operator=(const MotorControl &) = delete;
    void directMove();
    void reverseMove();
    void block();
    void freeRun();
};

#endif // MOTOR_H
