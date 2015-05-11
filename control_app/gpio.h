#ifndef GPIO_H
#define GPIO_H

#include <string>

// AT91SAM9G45-EKES GPIO configuration

#define GPIO_BASE_A 32
#define GPIO_BASE_B 64
#define GPIO_BASE_C 96
#define GPIO_BASE_D 128
#define GPIO_BASE_E 160

// Ultrasonic distance sensor
#define GPIO_PA6 "38" // J11.7
#define GPIO_PA7 "39" // J11.8
// Hall speed driver
#define GPIO_PA8 "40" // J11.9
// Headlights
#define GPIO_PA9 "41" // J11.10
// Battery ADC
#define GPIO_PA20 "52" // J8.13
#define GPIO_PA21 "53" // J8.14
#define TWD0 GPIO_PA20
#define TWCK0 GPIO_PA21
// Motors
#define GPIO_PC17 "113" // J10.7
#define GPIO_PC18 "114" // J10.8
#define GPIO_PC19 "115" // J10.9
#define GPIO_PC20 "116" // J10.10
#define GPIO_PC21 "117" // J10.11
#define GPIO_PC22 "118" // J10.12
#define GPIO_PC23 "119" // J10.13
#define GPIO_PC24 "120" // J10.14
#define GPIO_PC25 "121" // J10.15
#define GPIO_PC26 "122" // J10.16
#define GPIO_PC27 "123" // J10.17
#define GPIO_PC28 "124" // J10.18

class Gpio
{
    std::string num;

public:
    Gpio();
    explicit Gpio(const std::string &num);
    int gExport();
    int gUnexport();
    int setDir(const std::string &dir);
    int setVal(const std::string &val);
    int getVal(std::string &val);
    const std::string &getNum() const;
    void setNum(const std::string &num);
};

#endif // GPIO_H
