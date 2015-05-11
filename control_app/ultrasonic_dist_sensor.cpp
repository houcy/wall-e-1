#include "ultrasonic_dist_sensor.h"
#include "log.h"
#include <fstream>

#define DIST_VALUE_PATH "/sys/class/hcsr04/value"

UltrasonicDistSensor::UltrasonicDistSensor()
{
}

int UltrasonicDistSensor::getDistance()
{
#ifdef CONFIG_ULTRASONIC_DIST_SENSOR
    int val;
    std::ifstream distValFile(DIST_VALUE_PATH);

    if (distValFile.fail())
    {
        critical("Unable to get distance to obstacle");
        return -1;
    }

    distValFile >> val;

    distValFile.close();

    return val;
#endif

    return 0;
}
