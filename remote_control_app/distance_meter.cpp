#include "distance_meter.h"
#include <QPainter>

#define RECT_PEN_COLOR "#bbbbbb"
#define RECT_PEN_WIDTH 3

#define PADDING 2
#define RIGHT_PADDING 10

#define GRAD_0 "#ff7000"
#define GRAD_0_5 "#60ff00"
#define GRAD_0_9 "#70ff00"
#define GRAD_1 "#55ff00"

#define OBSTACLE_RECT_WIDTH 10
#define OBSTACLE_COLOR "#777777"

#define NOTCH_HEIGHT 5
#define NOTCH_WIDTH 2
#define NOTCH_COLOR "#777777"

#define NOTCH_COUNT 4

#define TEXT_COLOR "#777777"
#define TEXT_PEN_WIDTH 1
#define TEXT_SIZE 10

#define UNIT_TEXT "cm"

#define MIN_VALUE 0
#define MAX_VALUE 4000
#define VALUE_DIV 10

DistanceMeter::DistanceMeter(QWidget *parent) : QProgressBar(parent)
{
    setMinimum(MIN_VALUE);
    setMaximum(MAX_VALUE);
}

void DistanceMeter::calcWidgetParams()
{
    const QRect &widgetRect = rect();

    contentX = widgetRect.x() + PADDING + RECT_PEN_WIDTH / 2;
    contentY = widgetRect.y() + PADDING + RECT_PEN_WIDTH / 2;
    contentHeight = widgetRect.height() - 2 * PADDING - RECT_PEN_WIDTH / 2;
    contentWidth = widgetRect.width() - 2 * PADDING - RECT_PEN_WIDTH / 2;
    maxBarWidth = contentWidth - OBSTACLE_RECT_WIDTH - RIGHT_PADDING;
    barX = contentX + OBSTACLE_RECT_WIDTH;
}

void DistanceMeter::drawBorder(QPainter &painter)
{
    painter.setPen(QPen(QBrush(RECT_PEN_COLOR), RECT_PEN_WIDTH));

    painter.drawRect(rect());
}

void DistanceMeter::drawObstacle(QPainter &painter)
{
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(OBSTACLE_COLOR));

    painter.drawRect(contentX, contentY, OBSTACLE_RECT_WIDTH, contentHeight);
}

void DistanceMeter::drawBar(QPainter &painter)
{
    painter.setPen(Qt::NoPen);

    QLinearGradient rectGrad(barX, contentY, barX + maxBarWidth, contentY);
        rectGrad.setColorAt(0, GRAD_0);
        rectGrad.setColorAt(0.5, GRAD_0_5);
        rectGrad.setColorAt(0.9, GRAD_0_9);
        rectGrad.setColorAt(1, GRAD_1);
    painter.setBrush(QBrush(rectGrad));

    int barWidth = value() * maxBarWidth / (maximum() - minimum());
    painter.drawRect(barX, contentY, barWidth, contentHeight);
}

void DistanceMeter::drawNotch(QPainter &painter, int notchX, int notchY1,
    int notchY2)
{
    painter.setPen(QPen(QBrush(NOTCH_COLOR), NOTCH_WIDTH));

    painter.drawLine(notchX, notchY1, notchX, notchY2);
}

void DistanceMeter::drawNotchLabel(QPainter &painter, int notchValue,
    int notchX, int notchY)
{
    QString notchValueString(QString::number(notchValue / VALUE_DIV));

    QFont font;
    font.setBold(true);
    font.setPointSize(TEXT_SIZE);
    QFontMetrics fm(font);
    int textWidht = fm.width(notchValueString);
    int textHeight = fm.height();
    int textX = notchX - textWidht / 2;
    int textY = notchY - textHeight;

    painter.drawText(QRect(textX, textY, textWidht, textHeight),
        notchValueString);
}

void DistanceMeter::drawNotches(QPainter &painter)
{
    int notchStep = maxBarWidth / NOTCH_COUNT;
    int valueStep = (maximum() - minimum()) / NOTCH_COUNT;

    for (int notchIter = 1; notchIter <= NOTCH_COUNT; notchIter++)
    {
        int notchX = barX + notchIter * notchStep;
        int notchY1 = contentY + contentHeight - NOTCH_WIDTH / 2;
        int notchY2 = notchY1 - NOTCH_HEIGHT;

        drawNotch(painter, notchX, notchY1, notchY2);

        drawNotchLabel(painter, notchIter * valueStep, notchX, notchY2);
    }
}

void DistanceMeter::drawUnit(QPainter &painter)
{
    QFont font;
    font.setBold(true);
    font.setPointSize(TEXT_SIZE);
    QFontMetrics fm(font);
    int textWidht = fm.width(UNIT_TEXT);
    int textHeight = fm.height();
    int textX = barX + maxBarWidth - textWidht / 2;

    painter.drawText(QRect(textX, contentY, textWidht, textHeight), UNIT_TEXT);
}

void DistanceMeter::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    calcWidgetParams();

    drawBorder(painter);

    drawObstacle(painter);

    drawBar(painter);

    drawNotches(painter);

    drawUnit(painter);
}
