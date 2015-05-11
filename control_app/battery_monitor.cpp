#include "battery_monitor.h"
#include "log.h"
#include <fstream>

#define VOLTAGE_VALUE_PATH "/sys/bus/i2c/devices/0-0048/in2_input"
#define VOLTAGE_REF 4.65
#define MAX_VALUE 256
#define VOLTAGE_DIVIDER_COEFFICIENT 2.05
#define VALUE_COEFFICIENT 10
#define MAX_VOLTAGE 7.6
#define MIN_VOLTAGE 6.0

BatteryMonitor::BatteryMonitor()
{
}

int BatteryMonitor::getADCValue()
{
#ifdef CONFIG_BATTERY_VOLTAGE_SENSOR
    int val;
    std::ifstream voltageValFile(VOLTAGE_VALUE_PATH);

    if (voltageValFile.fail())
    {
        critical("Unable to get battery voltage");
        return -1;
    }

    voltageValFile >> val;

    voltageValFile.close();

    return val;
#endif

    return 0;
}

double BatteryMonitor::getVoltage()
{
    int adcValue = getADCValue();

    if (adcValue == -1)
        return -1;

    return adcValue * VOLTAGE_REF * VOLTAGE_DIVIDER_COEFFICIENT /
        MAX_VALUE / VALUE_COEFFICIENT;
}

int BatteryMonitor::getChargeLevel()
{
    double voltage = getVoltage();

    if (voltage < 0.0)
        return -1;

    if (voltage > MAX_VOLTAGE)
        return 100;

    if (voltage < MIN_VOLTAGE)
        return 0;

    return (voltage - MIN_VOLTAGE) * 100 / (MAX_VOLTAGE - MIN_VOLTAGE);
}
