#include "hall_speed_sensor.h"
#include "log.h"
#include <fstream>

#define HALL_SPEED_SENSOR_VALUE_PATH "/sys/class/halls/value"

HallSpeedSensor::HallSpeedSensor()
{
}

int HallSpeedSensor::getSpeed()
{
#ifdef CONFIG_HALL_SPEED_SENSOR
    int val;
    std::ifstream speedValFile(HALL_SPEED_SENSOR_VALUE_PATH);

    if (speedValFile.fail())
    {
        critical("Unable to get speed");
        return -1;
    }

    speedValFile >> val;

    speedValFile.close();

    return val;
#endif

    return 0;
}
