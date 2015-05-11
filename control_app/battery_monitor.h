#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

class BatteryMonitor
{
public:
    BatteryMonitor();
    int getADCValue();
    double getVoltage();
    int getChargeLevel();
};

#endif // BATTERY_MONITOR_H
