#ifndef VOLUME_SLIDER_H
#define VOLUME_SLIDER_H

#include <QFrame>

class QSlider;
class QLabel;

class VolumeSlider : public QFrame
{
    Q_OBJECT

    QSlider *slider;
    QLabel *volumeLevelLabel;

public:
    explicit VolumeSlider(QWidget *parent = 0);

    void setValue(int volume);

signals:
    void sliderMoved(int);

public slots:
    void volumeSliderMoved(int value);
    void setVolumeLevelText(int value);
};

#endif // VOLUME_SLIDER_H
