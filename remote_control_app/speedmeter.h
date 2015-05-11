#ifndef SPEEDMETER_H
#define SPEEDMETER_H

#include <QDial>

class Speedmeter : public QDial
{
    Q_OBJECT

    QRect widgetRect;
    int centerX, centerY;
    int notchRadiusExtX, notchRadiusExtY;
    int notchRadiusIntX, notchRadiusIntY;

    void calcWidgetParams();
    void drawBorder(QPainter &painter);
    void drawNotch(QPainter &painter, int centerX, int centerY, int angle,
        int notchRadiusIntX, int notchRadiusIntY, int notchRadiusExtX,
        int notchRadiusExtY);
    void drawNotchLabel(QPainter &painter, int centerX, int centerY, int angle,
        int notchRadiusIntX, int notchRadiusIntY, int notchValue);
    void drawNotches(QPainter &painter);
    void drawColorArc(QPainter &painter);
    void drawArrow(QPainter &painter);
    void drawUnit(QPainter &painter);

protected:
    void paintEvent(QPaintEvent *) override;

public:
    explicit Speedmeter(QWidget *parent = 0);
};

#endif // SPEEDMETER_H
