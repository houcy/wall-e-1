#include "motor_test_button.h"

MotorTestRadioButton::MotorTestRadioButton(int motor, int cmd,
    const QString &text, QWidget *parent) : QRadioButton(text, parent)
{
    this->motor = motor;
    this->cmd = cmd;

    connect(this, SIGNAL(toggled(bool)), this, SLOT(slotMotorCmd(bool)));
}

void MotorTestRadioButton::slotMotorCmd(bool checked)
{
    if (!checked)
        return;

    emit setMotorCmd(motor, cmd);
}
