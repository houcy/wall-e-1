#include "volume_slider.h"
#include <QGridLayout>
#include <QLabel>
#include <QSlider>

#define VOLUME_SLIDER_STYLE "\
    QSlider::groove:horizontal\
    {\
        border: 1px solid #bbb;\
        background: white;\
        height: 10px;\
        border-radius: 4px;\
    }\
    QSlider::sub-page:horizontal\
    {\
        background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
            stop: 0 #416b08, stop: 1 #cde800);\
        background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,\
            stop: 0 #cde800, stop: 1 #416b08);\
        border: 1px solid #777;\
        height: 10px;\
        border-radius: 4px;\
    }\
    QSlider::add-page:horizontal\
    {\
        background: #fff;\
        border: 1px solid #777;\
        height: 10px;\
        border-radius: 4px;\
    }\
    QSlider::handle:horizontal\
    {\
        image: url(:/images/images/volume_slider/circle.svg);\
        margin-top: -5px;\
        margin-bottom: -5px;\
        margin-right: -1px;\
        margin-left: -1px;\
    }\
    QSlider::handle:horizontal:hover\
    {\
        image: url(:/images/images/volume_slider/circle_hover.svg);\
    }\
    QSlider::handle:horizontal:pressed\
    {\
        image: url(:/images/images/volume_slider/circle_pressed.svg);\
    }\
    "

#define VOLUME_LEVEL_LABEL_STYLE "\
    QLabel\
    {\
        font-weight: bold;\
        color : #555;\
        font-size: 14px;\
        padding-bottom: 2px;\
    }\
    "

#define VOLUME_SLIDER_ICON ":/images/images/volume_slider/speaker.svg"

VolumeSlider::VolumeSlider(QWidget *parent) : QFrame(parent)
{
    QPixmap speakerPixmap(VOLUME_SLIDER_ICON);
    QLabel *speakerLabel = new QLabel;
    speakerLabel->setPixmap(speakerPixmap);

    slider = new QSlider(Qt::Horizontal);
    slider->setRange(0, 100);
    slider->setSingleStep(1);
    slider->setMinimumHeight(25);
    slider->setStyleSheet(VOLUME_SLIDER_STYLE);
    connect(slider, SIGNAL(sliderMoved(int)), this,
        SLOT(volumeSliderMoved(int)));

    volumeLevelLabel = new QLabel("0 %");
    volumeLevelLabel->setFixedWidth(60);
    volumeLevelLabel->setStyleSheet(VOLUME_LEVEL_LABEL_STYLE);
    connect(slider, SIGNAL(valueChanged(int)), this,
        SLOT(setVolumeLevelText(int)));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(speakerLabel, 0, 0);
    mainLayout->addWidget(slider, 0, 1);
    mainLayout->addWidget(volumeLevelLabel, 0, 2);

    setLayout(mainLayout);
}

void VolumeSlider::setVolumeLevelText(int value)
{
    volumeLevelLabel->setText(QString::number(value) + " %");
}

void VolumeSlider::setValue(int volume)
{
    slider->setValue(volume);
}

void VolumeSlider::volumeSliderMoved(int value)
{
    emit sliderMoved(value);
}

