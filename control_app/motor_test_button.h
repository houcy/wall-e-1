#ifndef MOTORTESTRADIOBUTTON_H
#define MOTORTESTRADIOBUTTON_H

#include <QRadioButton>

class MotorTestRadioButton : public QRadioButton
{
    Q_OBJECT

    int motor;
    int cmd;

public:
    explicit MotorTestRadioButton(int motor, int cmd, const QString &text,
        QWidget *parent = 0);

signals:
    void setMotorCmd(int motor, int cmd);

public slots:
    void slotMotorCmd(bool checked);
};

#endif // MOTORTESTRADIOBUTTON_H
