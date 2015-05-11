#include <QPainter>
#include <QPaintEvent>
#include <cmath>
#include "joystick_button.h"

#define IMAGES_PREFIX ":/images/images/joystick/"

JoystickButton::JoystickButton(QWidget *parent) : QLabel(parent)
{
    buttonPressed = buttonHovered = BUTTON_NONE;
    setMouseTracking(true);

    buttonPixmap[BUTTON_PIXMAP_BACKGROUND] =
        QPixmap(IMAGES_PREFIX "background.svg");
    buttonPixmap[BUTTON_PIXMAP_CENTER_NORMAL] =
        QPixmap(IMAGES_PREFIX "center.svg");
    buttonPixmap[BUTTON_PIXMAP_CENTER_HOVER] =
        QPixmap(IMAGES_PREFIX "center_hover.svg");
    buttonPixmap[BUTTON_PIXMAP_CENTER_PRESSED] =
        QPixmap(IMAGES_PREFIX "center_pressed.svg");
    buttonPixmap[BUTTON_PIXMAP_LEFT_NORMAL] =
        QPixmap(IMAGES_PREFIX "left.svg");
    buttonPixmap[BUTTON_PIXMAP_LEFT_HOVER] =
        QPixmap(IMAGES_PREFIX "left_hover.svg");
    buttonPixmap[BUTTON_PIXMAP_LEFT_PRESSED] =
        QPixmap(IMAGES_PREFIX "left_pressed.svg");
    buttonPixmap[BUTTON_PIXMAP_RIGHT_NORMAL] =
        QPixmap(IMAGES_PREFIX "right.svg");
    buttonPixmap[BUTTON_PIXMAP_RIGHT_HOVER] =
        QPixmap(IMAGES_PREFIX "right_hover.svg");
    buttonPixmap[BUTTON_PIXMAP_RIGHT_PRESSED] =
        QPixmap(IMAGES_PREFIX "right_pressed.svg");
    buttonPixmap[BUTTON_PIXMAP_UP_NORMAL] =
        QPixmap(IMAGES_PREFIX "up.svg");
    buttonPixmap[BUTTON_PIXMAP_UP_HOVER] =
        QPixmap(IMAGES_PREFIX "up_hover.svg");
    buttonPixmap[BUTTON_PIXMAP_UP_PRESSED] =
        QPixmap(IMAGES_PREFIX "up_pressed.svg");
    buttonPixmap[BUTTON_PIXMAP_DOWN_NORMAL] =
        QPixmap(IMAGES_PREFIX "down.svg");
    buttonPixmap[BUTTON_PIXMAP_DOWN_HOVER] =
        QPixmap(IMAGES_PREFIX "down_hover.svg");
    buttonPixmap[BUTTON_PIXMAP_DOWN_PRESSED] =
        QPixmap(IMAGES_PREFIX "down_pressed.svg");

    padding = 15;
    backgroundCenterOffsetXY = buttonPixmap[BUTTON_PIXMAP_BACKGROUND].
        width() / 2;
    intRadius = buttonPixmap[BUTTON_PIXMAP_CENTER_NORMAL].width() / 2;
    extRadius = (buttonPixmap[BUTTON_PIXMAP_BACKGROUND].width() - padding) / 2;

    for (int buttonIter = 0; buttonIter < buttonNumer; buttonIter++)
        isButtonSetPressed[buttonIter] = false;

    setFixedSize(buttonPixmap[BUTTON_PIXMAP_BACKGROUND].size());
}

JoystickButton::Button JoystickButton::getButton(int x, int y)
{
    int radius = sqrt(pow(x - backgroundCenterOffsetXY, 2) +
        pow(y - backgroundCenterOffsetXY, 2));

    if (radius <= intRadius)
        return BUTTON_CENTER;

    if (radius > intRadius && radius <= extRadius)
    {
        double angle = atan2(x - backgroundCenterOffsetXY,
            y - backgroundCenterOffsetXY);

        if (angle >= M_PI / 4 && angle < 3 * M_PI / 4)
            return BUTTON_RIGHT;

        if ((angle >= 3 * M_PI / 4 && angle <= M_PI) ||
            (angle >= -M_PI && angle < -3 * M_PI / 4))
        {
            return BUTTON_UP;
        }

        if (angle >= -3 * M_PI / 4 && angle < -M_PI / 4)
            return BUTTON_LEFT;

        return BUTTON_DOWN;
    }

    return BUTTON_NONE;
}

void JoystickButton::setButtonNormal(Button button, QPixmap **currPixmap)
{
    switch (button)
    {
    case BUTTON_CENTER:
        currPixmap[BUTTON_CENTER] = &buttonPixmap[BUTTON_PIXMAP_CENTER_NORMAL];
        break;
    case BUTTON_LEFT:
        currPixmap[BUTTON_LEFT] = &buttonPixmap[BUTTON_PIXMAP_LEFT_NORMAL];
        break;
    case BUTTON_RIGHT:
        currPixmap[BUTTON_RIGHT] = &buttonPixmap[BUTTON_PIXMAP_RIGHT_NORMAL];
        break;
    case BUTTON_UP:
        currPixmap[BUTTON_UP] = &buttonPixmap[BUTTON_PIXMAP_UP_NORMAL];
        break;
    case BUTTON_DOWN:
        currPixmap[BUTTON_DOWN] = &buttonPixmap[BUTTON_PIXMAP_DOWN_NORMAL];
        break;
    default:
        break;
    }
}

void JoystickButton::setButtonPressed(Button button, QPixmap **currPixmap)
{
    switch (button)
    {
    case BUTTON_CENTER:
        currPixmap[BUTTON_CENTER] =
            &buttonPixmap[BUTTON_PIXMAP_CENTER_PRESSED];
        break;
    case BUTTON_LEFT:
        currPixmap[BUTTON_LEFT] = &buttonPixmap[BUTTON_PIXMAP_LEFT_PRESSED];
        break;
    case BUTTON_RIGHT:
        currPixmap[BUTTON_RIGHT] = &buttonPixmap[BUTTON_PIXMAP_RIGHT_PRESSED];
        break;
    case BUTTON_UP:
        currPixmap[BUTTON_UP] = &buttonPixmap[BUTTON_PIXMAP_UP_PRESSED];
        break;
    case BUTTON_DOWN:
        currPixmap[BUTTON_DOWN] = &buttonPixmap[BUTTON_PIXMAP_DOWN_PRESSED];
        break;
    default:
        break;
    }
}

void JoystickButton::setButtonHover(Button button, QPixmap **currPixmap)
{
    switch (button)
    {
    case BUTTON_CENTER:
        currPixmap[BUTTON_CENTER] = &buttonPixmap[BUTTON_PIXMAP_CENTER_HOVER];
        break;
    case BUTTON_LEFT:
        currPixmap[BUTTON_LEFT] = &buttonPixmap[BUTTON_PIXMAP_LEFT_HOVER];
        break;
    case BUTTON_RIGHT:
        currPixmap[BUTTON_RIGHT] = &buttonPixmap[BUTTON_PIXMAP_RIGHT_HOVER];
        break;
    case BUTTON_UP:
        currPixmap[BUTTON_UP] = &buttonPixmap[BUTTON_PIXMAP_UP_HOVER];
        break;
    case BUTTON_DOWN:
        currPixmap[BUTTON_DOWN] = &buttonPixmap[BUTTON_PIXMAP_DOWN_HOVER];
        break;
    default:
        break;
    }
}

void JoystickButton::drawButtons(QPainter *painter, QPixmap **currPixmap)
{
    painter->drawPixmap(0, 0, buttonPixmap[BUTTON_PIXMAP_BACKGROUND]);
    painter->drawPixmap(backgroundCenterOffsetXY - intRadius,
        backgroundCenterOffsetXY - intRadius, *currPixmap[BUTTON_CENTER]);
    painter->drawPixmap(padding, backgroundCenterOffsetXY -
        currPixmap[BUTTON_LEFT]->height() / 2, *currPixmap[BUTTON_LEFT]);
    painter->drawPixmap(backgroundCenterOffsetXY -
        currPixmap[BUTTON_UP]->width() / 2, padding, *currPixmap[BUTTON_UP]);
    painter->drawPixmap(buttonPixmap[BUTTON_PIXMAP_BACKGROUND].width() -
        padding - currPixmap[BUTTON_RIGHT]->width(), backgroundCenterOffsetXY -
        currPixmap[BUTTON_RIGHT]->height() / 2, *currPixmap[BUTTON_RIGHT]);
    painter->drawPixmap(backgroundCenterOffsetXY -
        currPixmap[BUTTON_DOWN]->width() / 2,
        buttonPixmap[BUTTON_PIXMAP_BACKGROUND].height() - padding -
        currPixmap[BUTTON_DOWN]->height(), *currPixmap[BUTTON_DOWN]);
}

void JoystickButton::paintEvent(QPaintEvent *)
{
    QPixmap *currentButtonPixmap[buttonNumer];
    QPainter painter(this);

    for (unsigned int buttonIter = 0; buttonIter < buttonNumer; buttonIter++)
    {
        if (isButtonSetPressed[buttonIter] || buttonIter == buttonPressed)
        {
            setButtonPressed(static_cast< Button >(buttonIter),
                currentButtonPixmap);
        }
        else if (buttonIter == buttonHovered)
        {
            setButtonHover(static_cast< Button >(buttonIter),
                currentButtonPixmap);
        }
        else
        {
            setButtonNormal(static_cast< Button >(buttonIter),
                currentButtonPixmap);
        }
    }
    drawButtons(&painter, currentButtonPixmap);
}

void JoystickButton::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton &&
        (buttonPressed = getButton(e->x(), e->y())) != BUTTON_NONE)
    {
        sendButtonPressedSignal(buttonPressed);
        update();
    }

    QLabel::mousePressEvent(e);
}

void JoystickButton::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        if ((buttonPressed = getButton(e->x(), e->y())) != BUTTON_NONE)
        {
            sendButtonReleasedSignal(buttonPressed);
            buttonHovered = buttonPressed;
            buttonPressed = BUTTON_NONE;
        }
        update();
    }

    QLabel::mouseReleaseEvent(e);
}

void JoystickButton::mouseMoveEvent(QMouseEvent *e)
{
    buttonHovered = getButton(e->x(), e->y());
    update();

    QLabel::mouseMoveEvent(e);
}

void JoystickButton::setPressed(bool pressed, Button button)
{
    if (button >= buttonNumer)
        return;

    isButtonSetPressed[button] = pressed;

    update();
}

void JoystickButton::setCenterButtonPressed(bool pressed)
{
    setPressed(pressed, BUTTON_CENTER);
}

void JoystickButton::setUpButtonPressed(bool pressed)
{
    setPressed(pressed, BUTTON_UP);
}

void JoystickButton::setDownButtonPressed(bool pressed)
{
    setPressed(pressed, BUTTON_DOWN);
}

void JoystickButton::setRightButtonPressed(bool pressed)
{
    setPressed(pressed, BUTTON_RIGHT);
}

void JoystickButton::setLeftButtonPressed(bool pressed)
{
    setPressed(pressed, BUTTON_LEFT);
}

void JoystickButton::sendButtonPressedSignal(Button button)
{
    switch (button)
    {
    case BUTTON_CENTER:
        emit centerButtonPressed();
        break;
    case BUTTON_LEFT:
        emit leftButtonPressed();
        break;
    case BUTTON_RIGHT:
        emit rightButtonPressed();
        break;
    case BUTTON_UP:
        emit upButtonPressed();
        break;
    case BUTTON_DOWN:
        emit downButtonPressed();
        break;
    default:
        break;
    }
}

void JoystickButton::sendButtonReleasedSignal(Button button)
{
    switch (button)
    {
    case BUTTON_CENTER:
        emit centerButtonReleased();
        break;
    case BUTTON_LEFT:
        emit leftButtonReleased();
        break;
    case BUTTON_RIGHT:
        emit rightButtonReleased();
        break;
    case BUTTON_UP:
        emit upButtonReleased();
        break;
    case BUTTON_DOWN:
        emit downButtonReleased();
        break;
    default:
        break;
    }
}
