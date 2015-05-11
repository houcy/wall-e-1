#ifndef DISTANCE_METER_H
#define DISTANCE_METER_H

#include <QProgressBar>

class DistanceMeter : public QProgressBar
{
    Q_OBJECT

    int contentX, contentY;
    int contentWidth, contentHeight;
    int maxBarWidth;
    int barX;

    void calcWidgetParams();
    void drawBorder(QPainter &painter);
    void drawObstacle(QPainter &painter);
    void drawBar(QPainter &painter);
    void drawNotch(QPainter &painter, int notchX, int notchY1, int notchY2);
    void drawNotchLabel(QPainter &painter, int notchValue, int notchX, int notchY);
    void drawNotches(QPainter &painter);
    void drawUnit(QPainter &painter);

protected:
    void paintEvent(QPaintEvent *) override;

public:
    explicit DistanceMeter(QWidget *parent = 0);
};

#endif // DISTANCE_METER_H
