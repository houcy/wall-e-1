#ifndef CAR_H
#define CAR_H

class MotorControl;

class Car
{
    MotorControl *mCtrlFrontLeft;
    MotorControl *mCtrlFrontRight;
    MotorControl *mCtrlRearLeft;
    MotorControl *mCtrlRearRight;
    int turnMethod;
    enum CarState : int;
    CarState carState;

public:
    enum CarState : int
    {
        CAR_STATE_STOPPED,
        CAR_STATE_RUN_FREE,
        CAR_STATE_RUN_DIRECT,
        CAR_STATE_RUN_REVERSE,
        CAR_STATE_RUN_LEFT,
        CAR_STATE_RUN_RIGHT
    };
    enum
    {
        MOTOR_TEST_CMD_FREE_RUN,
        MOTOR_TEST_CMD_DIRECT_MOVE,
        MOTOR_TEST_CMD_REVERSE_MOVE,
        MOTOR_TEST_CMD_BLOCK
    };
    enum
    {
        MOTOR_FRONT_RIGHT,
        MOTOR_FRONT_LEFT,
        MOTOR_REAR_RIGHT,
        MOTOR_REAR_LEFT
    };
    enum
    {
        CAR_TURN_METHOD_DIFFERENTIAL,
        CAR_TURN_METHOD_SKID_STEER
    };

    Car();
    ~Car();
    Car(const Car &) = delete;
    Car &operator=(const Car &) = delete;
    void motorTest(int motor, int cmd);
    void runFree();
    void runDirect();
    void runReverse();
    void runLeft();
    void runRight();
    void stop();
    void setTurnMethod(int turnMethod);
    int getTurnMethod() const noexcept;
    CarState getCarState() const noexcept;
};

#endif // CAR_H
