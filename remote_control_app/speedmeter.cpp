#include "speedmeter.h"
#include <QPainter>
#include <math.h>

#define MARGIN 10

#define BORDER_GRAD_0 "#cccccc"
#define BORDER_GRAD_1 "#555555"
#define BORDER_WIDTH 10

#define BACKGROUND_GRAD_0 "#eeeeee"
#define BACKGROUND_GRAD_1 "#aaaaaa"

#define NOTCH_COLOR "#ffffff"
#define NOTCH_WIDTH 1
#define NOTCH_LENGTH 10
#define NOTCHES_NUMBER 7

#define TEXT_SIZE 10
#define TEXT_COLOR "#000000"
#define TEXT_PEN_WIDTH 1

#define ANGLE_RANGE 240
#define ANGLE_MIN (90 + (360 - ANGLE_RANGE) / 2)
#define ANGLE_MAX (ANGLE_MIN + ANGLE_RANGE)
#define ANGLE_STEP (ANGLE_RANGE / NOTCHES_NUMBER)
#define ANGLE_FIX 5

#define ARROW_PEN_COLOR "#000000"
#define ARROW_BRUSH_COLOR "#a7a7a7"
#define ARROW_WIDTH 1
#define ARROW_BOTTOM_LENGTH 10
#define ARROW_CIRCLE_COLOR "#77ff00"
#define ARROW_CIRCLE_RADIUS 2

#define ARC_WIDHT 10
#define ARC_ANGLE_MIN (-(90 - (360 - ANGLE_RANGE) / 2))
#define ARC_GRAD_1 "#55ff00"
#define ARC_GRAD_0_5 "#60ff00"
#define ARC_GRAD_0_9 "#70ff00"
#define ARC_GRAD_0 "#ff7000"

#define MIN_SPEED 0
#define MAX_SPEED 70
#define SPEED_DIV 1

#define SPEED_UNIT "cm/s"
#define SPEED_UNIT_TEXT_SIZE 10
#define SPEED_UNIT_TEXT_COLOR "#000000"
#define SPEED_UNIT_TEXT_PEN_WIDTH 1

Speedmeter::Speedmeter(QWidget *parent) : QDial(parent)
{
    setRange(MIN_SPEED, MAX_SPEED);
    setWrapping(false);
    setDisabled(true);
}

void Speedmeter::calcWidgetParams()
{
    widgetRect = QRect(MARGIN, MARGIN, size().width() - 2 * MARGIN,
        size().height() - 2 * MARGIN);
    centerX = widgetRect.topLeft().x() + widgetRect.size().width() / 2;
    centerY = widgetRect.topLeft().y() + widgetRect.size().height() / 2;
    notchRadiusExtX = widgetRect.width() / 2 - BORDER_WIDTH / 2 - ARC_WIDHT;
    notchRadiusExtY = widgetRect.height() / 2 - BORDER_WIDTH / 2  - ARC_WIDHT;
    notchRadiusIntX = notchRadiusExtX - NOTCH_LENGTH;
    notchRadiusIntY = notchRadiusExtY - NOTCH_LENGTH;
}

void Speedmeter::drawBorder(QPainter &painter)
{
    QLinearGradient borderGradient(widgetRect.topLeft(),
        widgetRect.bottomLeft());
    borderGradient.setColorAt(0, BORDER_GRAD_0);
    borderGradient.setColorAt(1, BORDER_GRAD_1);
    painter.setPen(QPen(QBrush(borderGradient), BORDER_WIDTH));

    QLinearGradient backgroundGradient(widgetRect.topLeft(),
        widgetRect.bottomLeft());
    backgroundGradient.setColorAt(0, BACKGROUND_GRAD_0);
    backgroundGradient.setColorAt(1, BACKGROUND_GRAD_1);
    painter.setBrush(QBrush(backgroundGradient));

    painter.drawEllipse(widgetRect);
}

void Speedmeter::drawNotch(QPainter &painter, int centerX, int centerY,
    int angle, int notchRadiusIntX, int notchRadiusIntY, int notchRadiusExtX,
    int notchRadiusExtY)
{
    painter.setPen(QPen(QBrush(NOTCH_COLOR), NOTCH_WIDTH));

    double notchAngleRad = angle * M_PI / 180.0;
    double notchAngleCos = cos(notchAngleRad);
    double notchAngleSin = sin(notchAngleRad);
    int notchX1 = centerX + notchRadiusExtX * notchAngleCos;
    int notchY1 = centerY + notchRadiusExtY * notchAngleSin;
    int notchX2 = centerX + notchRadiusIntX * notchAngleCos;
    int notchY2 = centerY + notchRadiusIntY * notchAngleSin;
    painter.drawLine(notchX1, notchY1, notchX2, notchY2);
}

void Speedmeter::drawNotchLabel(QPainter &painter, int centerX, int centerY,
    int angle, int notchRadiusIntX, int notchRadiusIntY, int notchValue)
{
    painter.setPen(QPen(QBrush(TEXT_COLOR), TEXT_PEN_WIDTH));

    QString notchValueString(QString::number(notchValue));

    QFont font;
    font.setBold(true);
    font.setPointSize(TEXT_SIZE);
    QFontMetrics fm(font);
    int textWidht = fm.width(notchValueString);
    int textHeight = fm.height();
    int maxTextOffset = sqrt(pow(textWidht / 2, 2) + pow(textHeight / 2, 2));
    int textRadiusX = notchRadiusIntX - maxTextOffset;
    int textRadiusY = notchRadiusIntY - maxTextOffset;

    double notchAngleRad = angle * M_PI / 180.0;
    double notchAngleCos = cos(notchAngleRad);
    double notchAngleSin = sin(notchAngleRad);
    int textX = centerX + textRadiusX * notchAngleCos - textWidht / 2;
    int textY = centerY + textRadiusY * notchAngleSin - textHeight / 2;

    painter.drawText(QRect(textX, textY, textWidht, textHeight),
        notchValueString);
}

void Speedmeter::drawNotches(QPainter &painter)
{
    int notchValue = minimum();
    int notchValueStep = (maximum() - minimum()) / NOTCHES_NUMBER;

    for (int angleIter = ANGLE_MIN; angleIter <= ANGLE_MAX;
         angleIter += ANGLE_STEP)
    {
        drawNotch(painter, centerX, centerY, angleIter, notchRadiusIntX,
            notchRadiusIntY, notchRadiusExtX, notchRadiusExtY);

        drawNotchLabel(painter, centerX, centerY, angleIter, notchRadiusIntX,
            notchRadiusIntY, notchValue);

        notchValue += notchValueStep / SPEED_DIV;
    }
}

void Speedmeter::drawColorArc(QPainter &painter)
{
    QConicalGradient arcGrad(QPointF(centerX, centerY), ARC_ANGLE_MIN -
        ANGLE_FIX);
    arcGrad.setColorAt(1, ARC_GRAD_1);
    arcGrad.setColorAt(0.5, ARC_GRAD_0_5);
    arcGrad.setColorAt(0.9, ARC_GRAD_0_9);
    arcGrad.setColorAt(0, ARC_GRAD_0);
    painter.setPen(QPen(QBrush(arcGrad), ARC_WIDHT));

    QRect arcRect(centerX - notchRadiusExtX - ARC_WIDHT / 2, centerY -
        notchRadiusExtY - ARC_WIDHT / 2, 2 * notchRadiusExtX + ARC_WIDHT,
        2 * notchRadiusExtY + ARC_WIDHT);
    int arcStartAngle = ARC_ANGLE_MIN * 16;
    int arcSpanAngle = ANGLE_RANGE * 16;
    painter.drawArc(arcRect, arcStartAngle, arcSpanAngle);
}

void Speedmeter::drawArrow(QPainter &painter)
{
    painter.setPen(QPen(QBrush(ARROW_PEN_COLOR), ARROW_WIDTH));
    painter.setBrush(QBrush(ARROW_BRUSH_COLOR));

    double arrowAngle = ANGLE_MIN + value() * (double)(ANGLE_MAX - ANGLE_MIN) /
        (maximum() - minimum());

    double arrowAngleRad0 = arrowAngle * M_PI / 180.0;
    int arrowX0 = centerX + notchRadiusExtX * cos(arrowAngleRad0);
    int arrowY0 = centerY + notchRadiusExtY * sin(arrowAngleRad0);

    double arrowAngleRad1 = (arrowAngle + 90) * M_PI / 180.0;
    int arrowX1 = centerX + ARROW_BOTTOM_LENGTH / 2 * cos(arrowAngleRad1);
    int arrowY1 = centerY + ARROW_BOTTOM_LENGTH / 2 * sin(arrowAngleRad1);

    double arrowAngleRad2 = (arrowAngle + 180) * M_PI / 180.0;
    int arrowX2 = centerX + ARROW_BOTTOM_LENGTH / 2 * cos(arrowAngleRad2);
    int arrowY2 = centerY + ARROW_BOTTOM_LENGTH / 2 * sin(arrowAngleRad2);

    double arrowAngleRad3 = (arrowAngle + 270) * M_PI / 180.0;
    int arrowX3 = centerX + ARROW_BOTTOM_LENGTH / 2 * cos(arrowAngleRad3);
    int arrowY3 = centerY + ARROW_BOTTOM_LENGTH / 2 * sin(arrowAngleRad3);

    QPolygon poly;
    poly << QPoint(arrowX0, arrowY0) << QPoint(arrowX1, arrowY1) <<
        QPoint(arrowX1, arrowY1) << QPoint(arrowX2, arrowY2) <<
        QPoint(arrowX3, arrowY3) << QPoint(arrowX0, arrowY0);
    painter.drawPolygon(poly);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(ARROW_CIRCLE_COLOR));

    painter.drawEllipse(centerX - ARROW_CIRCLE_RADIUS / 2,
        centerY - ARROW_CIRCLE_RADIUS / 2, ARROW_CIRCLE_RADIUS,
        ARROW_CIRCLE_RADIUS);
}

void Speedmeter::drawUnit(QPainter &painter)
{
    painter.setPen(QPen(QBrush(SPEED_UNIT_TEXT_COLOR),
        SPEED_UNIT_TEXT_PEN_WIDTH));

    QFont font;
    font.setBold(true);
    font.setPointSize(SPEED_UNIT_TEXT_SIZE);
    QFontMetrics fm(font);
    int textWidht = fm.width(SPEED_UNIT);
    int textHeight = fm.height();

    int textX = centerX - textWidht / 2;
    int textY = centerY + notchRadiusExtY / 2 - textHeight / 2;

    painter.drawText(QRect(textX, textY, textWidht, textHeight),
        SPEED_UNIT);
}

void Speedmeter::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    calcWidgetParams();

    drawBorder(painter);

    drawNotches(painter);

    drawColorArc(painter);

    drawArrow(painter);

    drawUnit(painter);
}
