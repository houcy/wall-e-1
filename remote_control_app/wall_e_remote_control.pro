#-------------------------------------------------
#
# Project created by QtCreator 2014-07-20T16:58:29
#
#-------------------------------------------------

QT += core gui network widgets

TARGET = Wall_E_Control
TEMPLATE = app

SOURCES += main.cpp \
    player.cpp \
    cmd.cpp \
    central_widget.cpp \
    main_window.cpp \
    tcp_client.cpp \
    options_dialog.cpp \
    joystick_button.cpp \
    volume_slider.cpp \
    animated_button.cpp \
    battery_widget.cpp \
    speedmeter.cpp \
    distance_meter.cpp \
    section.cpp

HEADERS += \
    player.h \
    cmd.h \
    central_widget.h \
    main_window.h \
    tcp_client.h \
    common.h \
    options_dialog.h \
    joystick_button.h \
    volume_slider.h \
    animated_button.h \
    battery_widget.h \
    speedmeter.h \
    distance_meter.h \
    section.h

unix {
    LIBS += -lvlc
}

QMAKE_CXXFLAGS += -std=c++11 -Wextra -Werror -DQT_COMPILING_QSTRING_COMPAT_CPP

win32 {
    INCLUDEPATH += "C:\Program Files\VideoLAN\VLC\sdk\include"
    LIBS += -L"C:\Program Files\VideoLAN\VLC\sdk\lib" -llibvlc
}

RESOURCES += resources.qrc
