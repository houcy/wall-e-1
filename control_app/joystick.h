#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <QObject>

class QTimer;

class Joystick : public QObject
{
    Q_OBJECT

    struct JsEvent
    {
        std::uint32_t time;  /* event timestamp in milliseconds */
        std::int16_t value;  /* value */
        std::uint8_t type;   /* event type */
        std::uint8_t number; /* axis/button number */
    };

    int jsFile;
    JsEvent jsEvent;
    QTimer *readDataTimer;
public:
    enum JsEventType
    {
        JS_EVENT_TYPE_BUTTON = 0x01,
        JS_EVENT_TYPE_AXIS = 0x02,
        JS_EVENT_TYPE_INIT = 0x80
    };
    enum JsEventAxisButtonNumber
    {
        JS_EVENT_AXIS_X_WE = 0x00,
        JS_EVENT_AXIS_X_NS = 0x01,
    };

    explicit Joystick(QObject *parent = 0);
    ~Joystick();

signals:
    void joystickEvent(std::uint8_t type, std::uint8_t num,
        std::int16_t value);

public slots:
    void slotReadData();
};

#endif // JOYSTICK_H
