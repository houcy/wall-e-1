#include "joystick.h"
#include "log.h"
#include <QTimer>
// Linux headers
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

/* Standard C++ API does not alow to read char device file in unblocking mode,
 * so used Linux API
 */

#define JOYSTICK_FILE "/dev/input/js0"

#define READ_DATA_INTERVAL 50

Joystick::Joystick(QObject *parent) : QObject(parent)
{
    if ((jsFile = open(JOYSTICK_FILE, O_RDONLY | O_NONBLOCK,
        S_IRUSR | S_IRGRP | S_IROTH)) == -1)
    {
        critical("Failed to open joystick file %d: %s", errno,
            strerror(errno));
        return;
    }

    readDataTimer = new QTimer(this);
    connect(readDataTimer, SIGNAL(timeout()), this,
        SLOT(slotReadData()));
    readDataTimer->setInterval(READ_DATA_INTERVAL);
    readDataTimer->start();
}

Joystick::~Joystick()
{
    if (jsFile != -1)
    {
        if (close(jsFile))
            critical("Failed to close joystick file");
    }
}

void Joystick::slotReadData()
{
    int n;

    while((n = read(jsFile, &jsEvent, sizeof(jsEvent))) == sizeof(jsEvent))
    {
        switch (jsEvent.type)
        {
        case JS_EVENT_TYPE_AXIS:
            switch (jsEvent.number)
            {
            case JS_EVENT_AXIS_X_NS:
            case JS_EVENT_AXIS_X_WE:
                emit joystickEvent(jsEvent.type, jsEvent.number, jsEvent.value);
            default:
	        break;
            }
            break;
        case JS_EVENT_TYPE_BUTTON:
        case JS_EVENT_TYPE_INIT:
        default:
            break;
        }
    }

    if (!n || (n > 0 && n != sizeof(jsEvent)) || (n == -1 && errno != EAGAIN))
    {
        critical("Failed to read joystick file");
        return;
    }
}
