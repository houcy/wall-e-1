#include "gpio.h"
#include "log.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#define EXPORT_PATH "/sys/class/gpio/export"
#define UNEXPORT_PATH "/sys/class/gpio/unexport"
#define GPIO_PATH "/sys/class/gpio/gpio"
#define GPIO_DIR "/direction"
#define GPIO_VAL "/value"

Gpio::Gpio()
{
}

Gpio::Gpio(const std::string &num)
{
    this->num = num;
}

int Gpio::gExport()
{
#ifdef CONFIG_GPIO
    std::ofstream exportGpioFile(EXPORT_PATH);

    if (exportGpioFile.fail())
    {
        critical("Unable to export GPIO %s", this->num.c_str());
        return -1;
    }

    exportGpioFile << this->num;
    exportGpioFile.close();
#endif

    return 0;
}

int Gpio::gUnexport()
{
#ifdef CONFIG_GPIO
    std::ofstream unexportGpioFile(UNEXPORT_PATH);

    if (unexportGpioFile.fail())
    {
        critical("Unable to unexport GPIO %s", this->num.c_str());
        return -1;
    }

    unexportGpioFile << this->num;
    unexportGpioFile.close();
#endif

    return 0;
}

// dir = "out"/"in"
int Gpio::setDir(const std::string &dir)
{
#ifdef CONFIG_GPIO
    std::string setDirStr = GPIO_PATH + this->num + GPIO_DIR;
    std::ofstream setDirGpioFile(setDirStr.c_str());

    if (setDirGpioFile.fail())
    {
        critical("Unable to set direction of GPIO %s", this->num.c_str());
        return -1;
    }

    setDirGpioFile << dir;
    setDirGpioFile.close();
#else
    (void)dir;
#endif

    return 0;
}

// e.g. val = PIOA base (32) + PA8 (8) = 40
int Gpio::setVal(const std::string &val)
{
#ifdef CONFIG_GPIO
    std::string setValStr = GPIO_PATH + this->num + GPIO_VAL;
    std::ofstream setValGpioFile(setValStr.c_str());

    if (setValGpioFile.fail())
    {
        critical("Unable to set the value of GPIO %s", this->num.c_str());
        return -1;
    }

    setValGpioFile << val;
    setValGpioFile.close();
#else
    (void)val;
#endif

    return 0;
}

int Gpio::getVal(std::string &val)
{
#ifdef CONFIG_GPIO
    std::string getValStr = GPIO_PATH + this->num + GPIO_VAL;
    std::ifstream getValGpioFile(getValStr.c_str());

    if (getValGpioFile.fail())
    {
        critical("Unable to get value of GPIO %s", this->num.c_str());
        return -1;
    }

    getValGpioFile >> val;

    if(val != "0")
        val = "1";
    else
        val = "0";

    getValGpioFile.close();
#else
    (void)val;
#endif

    return 0;
}

const std::string &Gpio::getNum() const
{
    return this->num;
}

void Gpio::setNum(const std::string &num)
{
    this->num = num;
}
