#ifndef BATTERY_WIDGET_H
#define BATTERY_WIDGET_H

#include <QProgressBar>
#include <QPixmap>
#include <QColor>
#include <map>
#include <memory>

class BatteryWidget : public QProgressBar
{
    Q_OBJECT

    int warnLevel;
    QPixmap backgroundPixmap;
    int backgroundImageWidth;
    int backgroundImageHeight;
    int maxChargeLevelRectWidth;
    int chargeLevelRectHeight;
    std::map< qreal, QColor > normalGradMap, warnGradMap;
    QFont font;
    std::unique_ptr< QFontMetrics > fontMetric;

protected:
    void paintEvent(QPaintEvent *) override;

public:
    explicit BatteryWidget(QWidget *parent = 0);

    void setWarnLevel(int level);

public slots:
    void setValue(int value);
};

#endif // BATTERY_WIDGET_H
