#-------------------------------------------------
#
# Project created by QtCreator 2014-07-27T20:21:57
#
#-------------------------------------------------

QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Wall_E_Engine
TEMPLATE = app

SOURCES += main.cpp \
    gpio.cpp \
    motor_control.cpp \
    car.cpp \
    motor_test_button.cpp \
    main_widget.cpp \
    tcp_server.cpp \
    cmd.cpp \
    sliding_stacked_widget.cpp \
    media_stream.cpp \
    horn.cpp \
    ultrasonic_dist_sensor.cpp \
    battery_monitor.cpp \
    hall_speed_sensor.cpp \
    headlights.cpp \
    joystick.cpp

HEADERS += gpio.h \
    motor_control.h \
    car.h \
    motor_test_button.h \
    main_widget.h \
    tcp_server.h \
    cmd.h \
    sliding_stacked_widget.h \
    log.h \
    common.h \
    media_stream.h \
    horn.h \
    ultrasonic_dist_sensor.h \
    battery_monitor.h \
    hall_speed_sensor.h \
    headlights.h \
    joystick.h

config_at91 {
    DEFINES += CONFIG_GPIO CONFIG_ULTRASONIC_DIST_SENSOR \
        CONFIG_BATTERY_VOLTAGE_SENSOR CONFIG_HALL_SPEED_SENSOR \
        CONFIG_AT91
}

debug {
    DEFINES += DEBUG
}

RESOURCES += resources.qrc

QMAKE_CXXFLAGS += -std=c++11 -Wextra -Werror
