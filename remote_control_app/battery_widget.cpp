#include "battery_widget.h"
#include <QPainter>

#define MIN_VALUE 0
#define MAX_VALUE 100
#define DEFAULT_WARN_LEVEL 10

#define IMAGES_PATH ":/images/images/"
#define BACKGROUND_IMAGE_PATH IMAGES_PATH "battery_widget/battery.svg"

#define IMAGE_OFFSET_X 0
#define IMAGE_OFFSET_Y 0
#define IMAGE_MARGIN 7
#define IMAGE_RIGHT_PADDING 6

#define GRAD_X1 0
#define GRAD_X2 0.3
#define GRAD_X3 1
#define GRAD_X1_NORM_COLOR "#006600"
#define GRAD_X2_NORM_COLOR "#00ff00"
#define GRAD_X3_NORM_COLOR "#005500"
#define GRAD_X1_WARN_COLOR "#660000"
#define GRAD_X2_WARN_COLOR "#ff0000"
#define GRAD_X3_WARN_COLOR "#550000"

#define RECT_RADIUS 5.0

#define TEXT_SIZE 14
#define TEXT_COLOR "#003300"

BatteryWidget::BatteryWidget(QWidget *parent) : QProgressBar(parent),
    backgroundPixmap(QPixmap(BACKGROUND_IMAGE_PATH)),
    normalGradMap({ { GRAD_X1, GRAD_X1_NORM_COLOR },
        { GRAD_X2, GRAD_X2_NORM_COLOR }, { GRAD_X3, GRAD_X3_NORM_COLOR } }),
    warnGradMap({ { GRAD_X1, GRAD_X1_WARN_COLOR },
        { GRAD_X2, GRAD_X2_WARN_COLOR }, { GRAD_X3, GRAD_X3_WARN_COLOR } })
{
    setRange(MIN_VALUE, MAX_VALUE);
    setValue(MIN_VALUE);
    warnLevel = DEFAULT_WARN_LEVEL;

    setMinimumSize(backgroundPixmap.size());

    backgroundImageWidth = backgroundPixmap.width();
    backgroundImageHeight = backgroundPixmap.height();
    maxChargeLevelRectWidth = backgroundImageWidth - 2 * IMAGE_MARGIN -
        IMAGE_RIGHT_PADDING;
    chargeLevelRectHeight = backgroundImageHeight - 2 * IMAGE_MARGIN;

    font.setBold(true);
    font.setItalic(true);
    font.setPointSize(TEXT_SIZE);
    fontMetric = std::move(std::unique_ptr< QFontMetrics >
        (new QFontMetrics(font)));
}

void BatteryWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int chargeLevel = value();
    QString chargeLevelText = QString::number(chargeLevel) + " %";

    painter.drawPixmap(IMAGE_OFFSET_X, IMAGE_OFFSET_Y, backgroundImageWidth,
        backgroundImageHeight, backgroundPixmap);

    painter.setRenderHint(QPainter::Antialiasing);
    QRect chargeLevelRect(IMAGE_MARGIN, IMAGE_MARGIN,
        chargeLevel * maxChargeLevelRectWidth / 100, chargeLevelRectHeight);

    QLinearGradient gradient(chargeLevelRect.topLeft().x(),
        chargeLevelRect.topLeft().y(), chargeLevelRect.bottomLeft().x(),
        chargeLevelRect.bottomLeft().y());
    std::map < qreal, QColor > *gradMap = chargeLevel > warnLevel ?
        &normalGradMap : &warnGradMap;
    gradient.setColorAt(GRAD_X1, (*gradMap)[GRAD_X1]);
    gradient.setColorAt(GRAD_X2, (*gradMap)[GRAD_X2]);
    gradient.setColorAt(GRAD_X3, (*gradMap)[GRAD_X3]);

    QBrush brush(gradient);
    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(chargeLevelRect, RECT_RADIUS, RECT_RADIUS);

    painter.setFont(font);
    int textWidth = fontMetric->width(chargeLevelText);
    int textHeight = fontMetric->height();

    QRect textRect(IMAGE_OFFSET_X + backgroundImageWidth / 2 - textWidth / 2,
        IMAGE_OFFSET_Y + backgroundImageHeight / 2 - textHeight / 2,
        textWidth, textHeight);

    painter.setPen(TEXT_COLOR);
    painter.drawText(textRect, chargeLevelText);
}

void BatteryWidget::setValue(int value)
{
    QProgressBar::setValue(value);
    update();
}

void BatteryWidget::setWarnLevel(int level)
{
    if (level < MIN_VALUE || level > MAX_VALUE)
        return;

    warnLevel = level;
    update();
}
