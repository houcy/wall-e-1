#include "main_widget.h"
#include "tcp_server.h"
#include "gpio.h"
#include "car.h"
#include "motor_test_button.h"
#include "cmd.h"
#include "sliding_stacked_widget.h"
#include "log.h"
#include "media_stream.h"
#include "horn.h"
#include "ultrasonic_dist_sensor.h"
#include "battery_monitor.h"
#include "hall_speed_sensor.h"
#include "headlights.h"
#include <QRadioButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QTimer>
#include <QPushButton>
#include <QSettings>
#include <QCheckBox>

#define SERVER_PORT 5000

#define VIDEO_STREAM_PORT 5001
#define AUDIO_STREAM_PORT 5002

#define CAR_START_DELAY 200

#define CMD_TIMEOUT 400

#define DYSPLAY_HORIZONTAL_RESOUTION 480
#define DYSPLAY_VERTICAL_RESOLUTION 272

#define NAVIGATION_BUTTON_WIDTH 40

#define CAR_TEST_COMMAND_RADIO_BOX_HEIGHT 15

#define SETTINGS_KEY_TURN_METHOD "turn_method"

#define DIST_UPDATE_INTERVAL 200
#define BATTERY_MONITOR_UPDATE_INTERVAL 5000
#define SPEED_SENSOR_UPDATE_INTERVAL 200

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    setupWindow();
    setupCmd();
    setupServer();
    setupCar();
    setupMediaStream();
    setupHorn();
    setupDistSensor();
    setupBatteryMonitor();
    setupSpeedSensor();
    setupHeadlights();

    setLayout(createMainLayout());

    readSettings();
}

MainWidget::~MainWidget()
{
}

void MainWidget::setupWindow()
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    setFixedSize(DYSPLAY_HORIZONTAL_RESOUTION, DYSPLAY_VERTICAL_RESOLUTION);
}

void MainWidget::setupCmd()
{
    cmd = std::unique_ptr< Cmd > (new Cmd);

    cmdTimeoutTimer = new QTimer(this);
    connect(cmdTimeoutTimer, SIGNAL(timeout()), this,
        SLOT(slotCmdTimeoutHandler()));
}

void MainWidget::setupServer()
{
    server = new TcpServer(this);
    if (!server->listen(QHostAddress::Any, SERVER_PORT))
    {
        critical("Unable to start the server: %s",
             server->errorString().toLatin1().constData());
        server->close();
    }
    connect(server, SIGNAL(signalDataReceived(const std::string &)),
        this, SLOT(slotDataReceived(const std::string &)));
    connect(server, SIGNAL(connectionStateChanged()), this,
        SLOT(slotUpdateConnection()));
}

void MainWidget::setupCar()
{
    car = std::unique_ptr< Car >(new Car());
}

void MainWidget::setupMediaStream()
{
    mediaStream = new MediaStream(this);
}

void MainWidget::setupHorn()
{
    horn = new Horn(this);
}

void MainWidget::setupDistSensor()
{
    distSensor =
        std::unique_ptr< UltrasonicDistSensor >(new UltrasonicDistSensor);
    distSensorTimer = new QTimer(this);
    connect(distSensorTimer, SIGNAL(timeout()), this,
        SLOT(slotUpdateDistance()));
    distSensorTimer->setInterval(DIST_UPDATE_INTERVAL);
    distSensorTimer->start();
}

void MainWidget::setupBatteryMonitor()
{
    batteryMonitor =
        std::unique_ptr< BatteryMonitor >(new BatteryMonitor);
    batteryMonitorTimer = new QTimer(this);
    connect(batteryMonitorTimer, SIGNAL(timeout()), this,
        SLOT(slotUpdateBatteryMonitor()));
    batteryMonitorTimer->setInterval(BATTERY_MONITOR_UPDATE_INTERVAL);
    batteryMonitorTimer->start();
}

void MainWidget::setupSpeedSensor()
{
    hallSpeedSensor =
        std::unique_ptr< HallSpeedSensor >(new HallSpeedSensor);
    hallSpeedSensorTimer = new QTimer(this);
    connect(hallSpeedSensorTimer, SIGNAL(timeout()), this,
        SLOT(slotUpdateSpeed()));
    hallSpeedSensorTimer->setInterval(SPEED_SENSOR_UPDATE_INTERVAL);
    hallSpeedSensorTimer->start();
}

void MainWidget::setupHeadlights()
{
    headlights = std::unique_ptr< Headlights >(new Headlights);
}

SlidingStackedWidget *MainWidget::createSlideWidget()
{
    slideWidget = new SlidingStackedWidget(this);
    slideWidget->setWrap(true);
    slideWidget->addWidget(createStatusPage());
    slideWidget->addWidget(createMotorTestPage());
    slideWidget->addWidget(createCarTestPage());

    return slideWidget;
}

QVBoxLayout *MainWidget::createNavigationButton(const char *imagePath,
    SlidingStackedWidget *sw, const char *slot)
{
    QPushButton *navButton = new QPushButton();
    connect(navButton, SIGNAL(pressed()), sw, slot);

    navButton->setMaximumWidth(NAVIGATION_BUTTON_WIDTH);
    navButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    navButton->setStyleSheet("border: 0px");
    navButton->setIcon(QIcon(imagePath));
    navButton->setIconSize(QSize(40, 40));
    navButton->setFocusPolicy(Qt::NoFocus);

    QVBoxLayout *navButtonVBoxLayout = new QVBoxLayout;
    navButtonVBoxLayout->addWidget(navButton);

    return navButtonVBoxLayout;

}

QHBoxLayout *MainWidget::createMainLayout()
{
    SlidingStackedWidget *slideWidget = createSlideWidget();

    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    hBoxLayout->addLayout(createNavigationButton(":/images/images/back.ico",
        slideWidget, SLOT(slideInPrev())));
    hBoxLayout->addWidget(slideWidget);
    hBoxLayout->addLayout(createNavigationButton(":/images/images/forward.ico",
       slideWidget, SLOT(slideInNext())));

    return hBoxLayout;
}

void MainWidget::slotDataReceived(const std::string &data)
{
    char command;
    std::string commandData;

    cmd->parse(data);

    while (!cmd->get(command, commandData))
        executeCommand(command, commandData);

    sendResponseToClient();
}

void MainWidget::executeCommand(char cmd, const std::string &cmdData)
{
    int data;

    try
    {
        data = std::stoi(cmdData);
    }
    catch (std::invalid_argument)
    {
        debug("Failed to get command's data");
        return;
    }
    catch (std::out_of_range)
    {
        debug("Failed to get command's data");
        return;
    }

    switch (cmd)
    {
    case Cmd::CMD_NONE:
        break;
    case Cmd::CMD_CAR_MOVE:
        switch (data)
        {
        case Cmd::CMD_DATA_CAR_MOVE_RUN_FREE:
            car->runFree();
            break;
        case Cmd::CMD_DATA_CAR_MOVE_RUN_DIRECT:
            car->runDirect();
            break;
        case Cmd::CMD_DATA_CAR_MOVE_RUN_REVERSE:
            car->runReverse();
            break;
        case Cmd::CMD_DATA_CAR_MOVE_TURN_LEFT:
            car->runLeft();
            break;
        case Cmd::CMD_DATA_CAR_MOVE_TURN_RIGHT:
            car->runRight();
            break;
        case Cmd::CMD_DATA_CAR_MOVE_STOP:
            car->stop();
            break;
        default:
            critical("Wrong car move command");
            return;
        }
        break;
    case Cmd::CMD_CAR_TURN_METHOD:
        switch (data)
        {
        case Cmd::CMD_DATA_TURN_METHOD_DIRERENTIAL:
            car->setTurnMethod(Car::CAR_TURN_METHOD_DIFFERENTIAL);
            break;
        case Cmd::CMD_DATA_TURN_METHOD_SKID_STEER:
            car->setTurnMethod(Car::CAR_TURN_METHOD_SKID_STEER);
            break;
        default:
            critical("Wrong car turn method");
            return;
        }
        saveSettings();
        break;
    case Cmd::CMD_HORN_SIGNAL:
        switch (data)
        {
        case Cmd::CMD_DATA_HORN_SIGNAL_START:
            horn->signalStart();
            break;
        case Cmd::CMD_DATA_HORN_SIGNAL_STOP:
            horn->signalStop();
            break;
        default:
            critical("Wrong horn signal action");
            return;
        }
        break;
    case Cmd::CMD_HEADLIGHTS:
        switch (data)
        {
        case Cmd::CMD_DATA_HEADLIGHTS_ON:
            headlights->on();
            break;
        case Cmd::CMD_DATA_HEADLIGHTS_OFF:
            headlights->off();
            break;
        default:
            critical("Wrong headlights signal action");
	    return;
        }
        break;
    default:
        critical("Wrong car command");
        return;
    }

    cmdTimeoutTimer->start(CMD_TIMEOUT);
}

void MainWidget::slotCmdTimeoutHandler()
{
    server->disconnectClient();
}

void MainWidget::slotMotorTestCmd(int motor, int cmd)
{
    car->motorTest(motor, cmd);
}

QGroupBox* MainWidget::motorTestCreate(int motor, const QString &groupText)
{
    MotorTestRadioButton *motorTestFreeRunButton =
        new MotorTestRadioButton(motor, Car::MOTOR_TEST_CMD_FREE_RUN,
        tr("Free running"));
    MotorTestRadioButton *motorTestDirectButton =
        new MotorTestRadioButton(motor, Car::MOTOR_TEST_CMD_DIRECT_MOVE,
        tr("Direct move"));
    MotorTestRadioButton *motorTestReverseButton =
        new MotorTestRadioButton(motor, Car::MOTOR_TEST_CMD_REVERSE_MOVE,
        tr("Reverse move"));
    MotorTestRadioButton *motorTestBlockButton =
        new MotorTestRadioButton(motor, Car::MOTOR_TEST_CMD_BLOCK,
        tr("Block"));

    connect(motorTestFreeRunButton, SIGNAL(setMotorCmd(int, int)),
            this, SLOT(slotMotorTestCmd(int, int)));
    connect(motorTestDirectButton, SIGNAL(setMotorCmd(int, int)),
            this, SLOT(slotMotorTestCmd(int, int)));
    connect(motorTestReverseButton, SIGNAL(setMotorCmd(int, int)),
            this, SLOT(slotMotorTestCmd(int, int)));
    connect(motorTestBlockButton, SIGNAL(setMotorCmd(int, int)),
            this, SLOT(slotMotorTestCmd(int, int)));

    QVBoxLayout *motorTestVBoxLayout = new QVBoxLayout;
    motorTestVBoxLayout->addWidget(motorTestFreeRunButton);
    motorTestVBoxLayout->addWidget(motorTestDirectButton);
    motorTestVBoxLayout->addWidget(motorTestReverseButton);
    motorTestVBoxLayout->addWidget(motorTestBlockButton);

    motorTestVBoxLayout->setContentsMargins(0, 0, 0, 0);

    QGroupBox *motorTestGroupBox = new QGroupBox(groupText);
    motorTestGroupBox->setLayout(motorTestVBoxLayout);
    motorTestGroupBox->setCheckable(true);
    motorTestGroupBox->setChecked(false);

    return motorTestGroupBox;
}

QWidget* MainWidget::carTestCreate()
{
    QLabel *turnMethodLabel = new QLabel(tr("Drive method:"));

    carTurnMethod = new QComboBox;
    carTurnMethod->addItem("Differential",
        QVariant(Car::CAR_TURN_METHOD_DIFFERENTIAL));
    carTurnMethod->addItem("Skid Steer",
        QVariant(Car::CAR_TURN_METHOD_SKID_STEER));
    carTurnMethod->setMaximumWidth(100);

    connect(carTurnMethod, SIGNAL(currentIndexChanged(int)), this,
            SLOT(slotSetTurnMethod(int)));

    QVBoxLayout *turnMethodLayout = new QVBoxLayout;
    turnMethodLayout->addWidget(turnMethodLabel);
    turnMethodLayout->addWidget(carTurnMethod);
    turnMethodLayout->setAlignment(Qt::AlignTop);

    QLabel *carCommandTitle = new QLabel(tr("Command:"));
    QRadioButton *carFreeRun = new QRadioButton(tr("Free run"));
    QRadioButton *carDirectRun = new QRadioButton(tr("Direct run"));
    QRadioButton *carReverseRun = new QRadioButton(tr("Reverse run"));
    QRadioButton *carTurnRight = new QRadioButton(tr("Turn right"));
    QRadioButton *carTurnLeft = new QRadioButton(tr("Turn left"));
    QRadioButton *carStop = new QRadioButton(tr("Stop"));

    carFreeRun->setMaximumHeight(CAR_TEST_COMMAND_RADIO_BOX_HEIGHT);
    carDirectRun->setMaximumHeight(CAR_TEST_COMMAND_RADIO_BOX_HEIGHT);
    carReverseRun->setMaximumHeight(CAR_TEST_COMMAND_RADIO_BOX_HEIGHT);
    carTurnRight->setMaximumHeight(CAR_TEST_COMMAND_RADIO_BOX_HEIGHT);
    carTurnLeft->setMaximumHeight(CAR_TEST_COMMAND_RADIO_BOX_HEIGHT);
    carStop->setMaximumHeight(CAR_TEST_COMMAND_RADIO_BOX_HEIGHT);

    connect(carFreeRun, SIGNAL(toggled(bool)), this,\
        SLOT(slotCarTestFreeRun(bool)));
    connect(carDirectRun, SIGNAL(toggled(bool)), this,
        SLOT(slotCarTestDirect(bool)));
    connect(carReverseRun, SIGNAL(toggled(bool)), this,
        SLOT(slotCarTestReverse(bool)));
    connect(carTurnRight, SIGNAL(toggled(bool)), this,
        SLOT(slotCarTestTurnRight(bool)));
    connect(carTurnLeft, SIGNAL(toggled(bool)), this,
        SLOT(slotCarTestTurnLeft(bool)));
    connect(carStop, SIGNAL(toggled(bool)), this,
            SLOT(slotCarTestStop(bool)));

    QVBoxLayout *carTestVBoxLayout = new QVBoxLayout;
    carTestVBoxLayout->addWidget(carCommandTitle);
    carTestVBoxLayout->addWidget(carFreeRun);
    carTestVBoxLayout->addWidget(carDirectRun);
    carTestVBoxLayout->addWidget(carReverseRun);
    carTestVBoxLayout->addWidget(carTurnRight);
    carTestVBoxLayout->addWidget(carTurnLeft);
    carTestVBoxLayout->addWidget(carStop);
    carTestVBoxLayout->setAlignment(Qt::AlignTop);

    QHBoxLayout *carTestWidgetMainLayout = new QHBoxLayout;
    carTestWidgetMainLayout->addLayout(carTestVBoxLayout);
    carTestWidgetMainLayout->addLayout(turnMethodLayout);

    QGroupBox *carTestWidgetGroupBox = new QGroupBox(tr("Enable"));
    carTestWidgetGroupBox->setLayout(carTestWidgetMainLayout);
    carTestWidgetGroupBox->setCheckable(true);
    carTestWidgetGroupBox->setChecked(false);

    QVBoxLayout *carTestWidgetLayout = new QVBoxLayout;
    carTestWidgetLayout->addWidget(carTestWidgetGroupBox);

    QWidget *carTestWidget = new QWidget;
    carTestWidget->setLayout(carTestWidgetLayout);

    return carTestWidget;
}

void MainWidget::slotSetTurnMethod(int index)
{
    int turnMethod = carTurnMethod->itemData(index).toInt();

    car->setTurnMethod(turnMethod);
}

void MainWidget::slotCarTestFreeRun(bool checked)
{
    if (!checked)
        return;

    car->runFree();
}

void MainWidget::slotCarTestDirect(bool checked)
{
    if (!checked)
        return;

    slotCarTestFreeRun(true);
    QTimer::singleShot(CAR_START_DELAY, this,
        SLOT(slotCarTestDirectDelayed()));
}

void MainWidget::slotCarTestDirectDelayed()
{
    car->runDirect();
}

void MainWidget::slotCarTestReverse(bool checked)
{
    if (!checked)
        return;

    slotCarTestFreeRun(true);
    QTimer::singleShot(CAR_START_DELAY, this,
        SLOT(slotCarTestReverseDelayed()));
}

void MainWidget::slotCarTestReverseDelayed()
{
    car->runReverse();
}

void MainWidget::slotCarTestTurnLeft(bool checked)
{
    if (!checked)
        return;

    slotCarTestFreeRun(true);
    QTimer::singleShot(CAR_START_DELAY, this,
        SLOT(slotCarTestTurnLeftDelayed()));
}

void MainWidget::slotCarTestTurnLeftDelayed()
{
    car->runLeft();
}

void MainWidget::slotCarTestTurnRight(bool checked)
{
    if (!checked)
        return;

    slotCarTestFreeRun(true);
    QTimer::singleShot(CAR_START_DELAY, this,
        SLOT(slotCarTestTurnRightDelayed()));
}

void MainWidget::slotCarTestTurnRightDelayed()
{
    car->runRight();
}

void MainWidget::slotCarTestStop(bool checked)
{
    if (!checked)
        return;

    car->stop();
}

QLabel *MainWidget::createPageTitle(const QString &title)
{
    QLabel *titleLabel = new QLabel(title);

    titleLabel->setAlignment(Qt::AlignHCenter);
    titleLabel->setMaximumHeight(15);

    QFont font;
    font.setBold(true);
    titleLabel->setFont(font);

    return titleLabel;
}

QWidget *MainWidget::createStatusPage()
{
    QLabel *titleLabel = createPageTitle(tr("Status"));

    connectionStatusLabel = new QLabel;
    slotUpdateConnection();

    distLabel = new QLabel;
    slotUpdateDistance();

    batteryVoltageLabel = new QLabel;
    batteryChargeLabel = new QLabel;
    slotUpdateBatteryMonitor();

    speedLabel = new QLabel;
    slotUpdateSpeed();

    QVBoxLayout *statusPageLayout = new QVBoxLayout;
    statusPageLayout->setAlignment(Qt::AlignTop);
    statusPageLayout->addWidget(connectionStatusLabel);
    statusPageLayout->addWidget(distLabel);
    statusPageLayout->addWidget(batteryVoltageLabel);
    statusPageLayout->addWidget(batteryChargeLabel);
    statusPageLayout->addWidget(speedLabel);

    QWidget *widget = new QWidget;
    widget->setLayout(statusPageLayout);

    QVBoxLayout *vBoxLayout = new QVBoxLayout;
    vBoxLayout->addWidget(titleLabel);
    vBoxLayout->addWidget(widget);

    QWidget *widgetWithTitle = new QWidget;
    widgetWithTitle->setLayout(vBoxLayout);

    return widgetWithTitle;
}

QWidget *MainWidget::createMotorTestPage()
{
    QGridLayout *motorTestGridLayout = new QGridLayout;
    motorTestGridLayout->addWidget(motorTestCreate(Car::MOTOR_FRONT_RIGHT,
        tr("Front Right Motor")), 0, 1);
    motorTestGridLayout->addWidget(motorTestCreate(Car::MOTOR_FRONT_LEFT,
        tr("Front Left Motor")), 0, 0);
    motorTestGridLayout->addWidget(motorTestCreate(Car::MOTOR_REAR_RIGHT,
        tr("Rear Right Motor")), 1, 1);
    motorTestGridLayout->addWidget(motorTestCreate(Car::MOTOR_REAR_LEFT,
        tr("Rear Left Motor")), 1, 0);

    QLabel *titleLabel = createPageTitle(tr("Motor Test"));

    QVBoxLayout *vBoxLayout = new QVBoxLayout;
    vBoxLayout->addWidget(titleLabel);
    vBoxLayout->addLayout(motorTestGridLayout);

    QWidget *motorTestPageWidget = new QWidget;
    motorTestPageWidget->setLayout(vBoxLayout);

    return motorTestPageWidget;
}

QWidget *MainWidget::createCarTestPage()
{
    QLabel *titleLabel = createPageTitle(tr("Car Test"));

    QVBoxLayout *vBoxLayout = new QVBoxLayout;
    vBoxLayout->addWidget(titleLabel);
    vBoxLayout->addWidget(carTestCreate());

    QWidget *carTestPageWidget = new QWidget;
    carTestPageWidget->setLayout(vBoxLayout);

    return carTestPageWidget;
}

QString MainWidget::getConnectionStatus() const
{
    if (server->isClientConnected())
        return "Connected";

    return "Disconnected";
}

void MainWidget::slotUpdateConnection()
{
    connectionStatusLabel->setText(QString("Connection: ") +
        getConnectionStatus());

    if (server->isClientConnected())
    {
        QHostAddress addr = server->getClientAddr();
        if (addr == QHostAddress::Null)
            critical("Client does not have valid IP address\n");
        else
            mediaStream->start(addr, AUDIO_STREAM_PORT, VIDEO_STREAM_PORT);
    }
    else
    {
        car->stop();
        horn->signalStop();
        mediaStream->stop();
        headlights->off();
    }
}

void MainWidget::sendResponseToClient()
{
    cmd->enqueueResp(Cmd::CMD_RESP_ACK, std::string(""));
    cmd->enqueueResp(Cmd::CMD_RESP_DIST_TO_OBSTACLE,
        std::to_string(distToObstacle));
    cmd->enqueueResp(Cmd::CMD_RESP_BATTERY_CHARGE_LEVEL,
        std::to_string(batteryChargeLevel));
    cmd->enqueueResp(Cmd::CMD_RESP_SPEED, std::to_string(speed));

    server->sendToClient(cmd->getRespData());

    cmd->clearResp();
}

void MainWidget::readSettings()
{
    QSettings settings;

    if (settings.contains(SETTINGS_KEY_TURN_METHOD))
        car->setTurnMethod(settings.value(SETTINGS_KEY_TURN_METHOD).toInt());
}

void MainWidget::saveSettings()
{
    QSettings settings;

    settings.setValue(SETTINGS_KEY_TURN_METHOD, car->getTurnMethod());
}

void MainWidget::slotUpdateDistance()
{
    if (slideWidget->isAnimationActive())
        return;

    distToObstacle = distSensor->getDistance();

    distLabel->setText(QString(tr("Distance to obstacle: ")) +
        (distToObstacle != -1 ? QString::number(distToObstacle / 10.0, 'f', 1)
        : QString("-")) + tr(" cm"));
}

void MainWidget::slotUpdateBatteryMonitor()
{
    if (slideWidget->isAnimationActive())
        return;

    double batteryVoltage = batteryMonitor->getVoltage();
    batteryChargeLevel = batteryMonitor->getChargeLevel();

    batteryVoltageLabel->setText(QString(tr("Battery voltage: ")) +
        (batteryVoltage < 0.0 ? QString("-") :
        QString::number(batteryVoltage, 'f', 2)) + tr(" V"));
    batteryChargeLabel->setText(QString(tr("Battery charge: ")) +
        (batteryChargeLevel != -1 ? QString::number(batteryChargeLevel) :
        QString("-")) + tr(" %"));
}

void MainWidget::slotUpdateSpeed()
{
    if (slideWidget->isAnimationActive())
        return;

    speed = hallSpeedSensor->getSpeed();

    speedLabel->setText(QString(tr("Speed: ")) +
        (speed == -1 ? QString("-") :
        QString::number(speed / 100.0, 'f', 2)) + tr(" m/s"));
}
