#include "car.h"
#include "motor_control.h"
#include "log.h"

#define GPIO_FRONT_LEFT_ENABLE GPIO_PC28
#define GPIO_FRONT_LEFT_DIRECT GPIO_PC27
#define GPIO_FRONT_LEFT_REVERSE GPIO_PC26

#define GPIO_FRONT_RIGHT_ENABLE GPIO_PC22
#define GPIO_FRONT_RIGHT_DIRECT GPIO_PC21
#define GPIO_FRONT_RIGHT_REVERSE GPIO_PC20

#define GPIO_REAR_LEFT_ENABLE GPIO_PC23
#define GPIO_REAR_LEFT_DIRECT GPIO_PC24
#define GPIO_REAR_LEFT_REVERSE GPIO_PC25

#define GPIO_REAR_RIGHT_ENABLE GPIO_PC17
#define GPIO_REAR_RIGHT_DIRECT GPIO_PC18
#define GPIO_REAR_RIGHT_REVERSE GPIO_PC19

Car::Car()
{
    mCtrlFrontLeft = new MotorControl(GPIO_FRONT_LEFT_DIRECT,
        GPIO_FRONT_LEFT_REVERSE, GPIO_FRONT_LEFT_ENABLE);
    mCtrlFrontRight = new MotorControl(GPIO_FRONT_RIGHT_DIRECT,
        GPIO_FRONT_RIGHT_REVERSE, GPIO_FRONT_RIGHT_ENABLE);
    mCtrlRearLeft = new MotorControl(GPIO_REAR_LEFT_DIRECT,
        GPIO_REAR_LEFT_REVERSE, GPIO_REAR_LEFT_ENABLE);
    mCtrlRearRight = new MotorControl(GPIO_REAR_RIGHT_DIRECT,
        GPIO_REAR_RIGHT_REVERSE, GPIO_REAR_RIGHT_ENABLE);

    turnMethod = CAR_TURN_METHOD_DIFFERENTIAL;
    carState = CAR_STATE_RUN_FREE;
}

Car::~Car()
{
    delete mCtrlRearRight;
    delete mCtrlRearLeft;
    delete mCtrlFrontRight;
    delete mCtrlFrontLeft;
}

void Car::motorTest(int motor, int cmd)
{
    MotorControl *motorControl;

    switch (motor)
    {
    case MOTOR_FRONT_RIGHT:
        motorControl = mCtrlFrontRight;
        break;
    case MOTOR_FRONT_LEFT:
        motorControl = mCtrlFrontLeft;
        break;
    case MOTOR_REAR_RIGHT:
        motorControl = mCtrlRearRight;
        break;
    case MOTOR_REAR_LEFT:
        motorControl = mCtrlRearLeft;
        break;
    default:
        warning("Motor test error - wrong motor number");
        return;
    }

    switch (cmd)
    {
    case MOTOR_TEST_CMD_FREE_RUN:
        motorControl->freeRun();
        break;
    case MOTOR_TEST_CMD_DIRECT_MOVE:
        motorControl->directMove();
        break;
    case MOTOR_TEST_CMD_REVERSE_MOVE:
        motorControl->reverseMove();
        break;
    case MOTOR_TEST_CMD_BLOCK:
        motorControl->block();
        break;
    default:
        warning("Motor test error - wrong motor command");
        return;
    }
}

void Car::runFree()
{
    mCtrlFrontRight->freeRun();
    mCtrlFrontLeft->freeRun();
    mCtrlRearRight->freeRun();
    mCtrlRearLeft->freeRun();
    carState = CAR_STATE_RUN_FREE;
}

void Car::runDirect()
{
    mCtrlFrontRight->directMove();
    mCtrlFrontLeft->directMove();
    mCtrlRearRight->directMove();
    mCtrlRearLeft->directMove();
    carState = CAR_STATE_RUN_DIRECT;
}

void Car::runReverse()
{
    mCtrlFrontRight->reverseMove();
    mCtrlFrontLeft->reverseMove();
    mCtrlRearRight->reverseMove();
    mCtrlRearLeft->reverseMove();
    carState = CAR_STATE_RUN_REVERSE;
}

void Car::runLeft()
{
    switch(turnMethod)
    {
    case CAR_TURN_METHOD_DIFFERENTIAL:
        mCtrlFrontRight->directMove();
        mCtrlFrontLeft->block();
        mCtrlRearRight->directMove();
        mCtrlRearLeft->block();
        break;
    case CAR_TURN_METHOD_SKID_STEER:
        mCtrlFrontRight->directMove();
        mCtrlFrontLeft->reverseMove();
        mCtrlRearRight->directMove();
        mCtrlRearLeft->reverseMove();
        break;
    }
    carState = CAR_STATE_RUN_LEFT;
}

void Car::runRight()
{
    switch(turnMethod)
    {
    case CAR_TURN_METHOD_DIFFERENTIAL:
        mCtrlFrontRight->block();
        mCtrlFrontLeft->directMove();
        mCtrlRearRight->block();
        mCtrlRearLeft->directMove();
        break;
    case CAR_TURN_METHOD_SKID_STEER:
        mCtrlFrontRight->reverseMove();
        mCtrlFrontLeft->directMove();
        mCtrlRearRight->reverseMove();
        mCtrlRearLeft->directMove();
        break;
    }
    carState = CAR_STATE_RUN_RIGHT;
}

void Car::stop()
{
    mCtrlFrontRight->block();
    mCtrlFrontLeft->block();
    mCtrlRearRight->block();
    mCtrlRearLeft->block();
    carState = CAR_STATE_STOPPED;
}

void Car::setTurnMethod(int turnMethod)
{
    switch (turnMethod)
    {
    case CAR_TURN_METHOD_DIFFERENTIAL:
    case CAR_TURN_METHOD_SKID_STEER:
        break;
    default:
        warning("Wrong turn method");
        return;
    }

    this->turnMethod = turnMethod;
}

int Car::getTurnMethod() const noexcept
{
    return turnMethod;
}

Car::CarState Car::getCarState() const noexcept
{
    return carState;
}
