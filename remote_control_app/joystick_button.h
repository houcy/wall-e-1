#ifndef JOYSTICK_BUTTON_H
#define JOYSTICK_BUTTON_H

#include <QLabel>
#include <QPixmap>

class QPaintEvent;
class QMouseEvent;
class QPaint;

class JoystickButton : public QLabel
{
    Q_OBJECT

    enum Button : unsigned int
    {
        BUTTON_CENTER = 0, BUTTON_LEFT = 1, BUTTON_RIGHT = 2, BUTTON_UP = 3,
        BUTTON_DOWN = 4, BUTTON_NONE = 5
    };
    enum ButtonPixmap : unsigned int
    {
        BUTTON_PIXMAP_BACKGROUND = 0, BUTTON_PIXMAP_CENTER_NORMAL = 1,
        BUTTON_PIXMAP_CENTER_HOVER = 2, BUTTON_PIXMAP_CENTER_PRESSED = 3,
        BUTTON_PIXMAP_LEFT_NORMAL = 4, BUTTON_PIXMAP_LEFT_HOVER = 5,
        BUTTON_PIXMAP_LEFT_PRESSED = 6, BUTTON_PIXMAP_RIGHT_NORMAL = 7,
        BUTTON_PIXMAP_RIGHT_HOVER = 8, BUTTON_PIXMAP_RIGHT_PRESSED = 9,
        BUTTON_PIXMAP_UP_NORMAL = 10, BUTTON_PIXMAP_UP_HOVER = 11,
        BUTTON_PIXMAP_UP_PRESSED = 12, BUTTON_PIXMAP_DOWN_NORMAL = 13,
        BUTTON_PIXMAP_DOWN_HOVER = 14, BUTTON_PIXMAP_DOWN_PRESSED = 15,
        BUTTON_PIXMAP_LAST = 16
    };

    static const int buttonNumer = 5;

    QPixmap buttonPixmap[BUTTON_PIXMAP_LAST];
    int padding;
    int backgroundCenterOffsetXY;
    int intRadius;
    int extRadius;
    Button buttonPressed;
    Button buttonHovered;
    bool isButtonSetPressed[buttonNumer];

    Button getButton(int x, int y);
    void drawButtons(QPainter *painter, QPixmap **currPixmap);
    void setButtonNormal(Button button, QPixmap **currPixmap);
    void setButtonPressed(Button button, QPixmap **currPixmap);
    void setButtonHover(Button button, QPixmap **currPixmap);
    void sendButtonPressedSignal(Button button);
    void sendButtonReleasedSignal(Button button);
    void setPressed(bool pressed, Button button);

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;

public:
    explicit JoystickButton(QWidget *parent = 0);

    void setCenterButtonPressed(bool pressed);
    void setUpButtonPressed(bool pressed);
    void setDownButtonPressed(bool pressed);
    void setRightButtonPressed(bool pressed);
    void setLeftButtonPressed(bool pressed);

signals:
    void centerButtonPressed();
    void centerButtonReleased();
    void upButtonPressed();
    void upButtonReleased();
    void downButtonPressed();
    void downButtonReleased();
    void rightButtonPressed();
    void rightButtonReleased();
    void leftButtonPressed();
    void leftButtonReleased();
};

#endif // JOYSTICK_BUTTON_H
