#include "central_widget.h"
#include "player.h"
#include "tcp_client.h"
#include "cmd.h"
#include "joystick_button.h"
#include "animated_button.h"
#include "battery_widget.h"
#include "speedmeter.h"
#include "distance_meter.h"
#include "section.h"
#include <QGridLayout>
#include <QPushButton>
#include <QTimer>
#include <QWidget>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QGroupBox>
#include <QKeyEvent>
#include <QCoreApplication>
#include <QSettings>
#include <QShortcut>

#define VIDEO_STREAM "udp://@:5000"
#define VIDEO_H_RESOLUTION 320
#define VIDIO_V_RESOLUTION 240
#define AUDIO_STREAM "rtp://@:5001"

#define SERVER_NAME "192.168.3.1"
#define SERVER_PORT 5001

#define CMD_SEND_INTERVAL 100
#define CMD_ACK_TIMEOUT 3000

#define CAR_CMD_BUTTON_WIDTH 50
#define CAR_CMD_BUTTON_HEIGHT 50

#define SETTINGS_KEY_TURN_METHOD "turn_method"

#define DISTANCE_TO_OBSTACLE "Distance: "
#define BATTERY_CHARGE "Battery charge: "
#define SPEED "Speed: "

#define KEY_RELEASE_TIME 100

#define IMAGES_PATH ":/images/images/"

#define HEADLIGHTS_IMAGE_PATH IMAGES_PATH "headlights_button/"
#define HEADLIGHTS_IMAGE_NORMAL HEADLIGHTS_IMAGE_PATH "headlights.svg"
#define HEADLIGHTS_IMAGE_PRESSED HEADLIGHTS_IMAGE_PATH "headlights_pressed.svg"
#define HEADLIGHTS_IMAGE_HOVERED HEADLIGHTS_IMAGE_PATH "headlights_hovered.svg"

#define HORN_IMAGE_PATH IMAGES_PATH "horn_button/"
#define HORN_IMAGE_NORMAL HORN_IMAGE_PATH "horn.svg"
#define HORN_IMAGE_PRESSED HORN_IMAGE_PATH "horn_pressed.svg"
#define HORN_IMAGE_HOVERED HORN_IMAGE_PATH "horn_hovered.svg"

#define CONNECT_IMAGE_CONNECT IMAGES_PATH "connect/connect.svg"
#define CONNECT_IMAGE_CONNECTING IMAGES_PATH "connect/connecting.svg"
#define CONNECT_IMAGE_CONNECTED IMAGES_PATH "connect/connected.svg"
#define CONNECT_BUTTON_WIDTH 120
#define CONNECT_BUTTON_ICON_SIZE 30

#define SECTION_CONNECT "Connect to robot"
#define SECTION_CONNECT_STATUS "Connection status"
#define SECTION_BATTERY "Battery state"
#define SECTION_OBSTACLE "Obstacle detection"
#define SECTION_SPEED "Speed"

CentralWidget::CentralWidget(QWidget *parent) :
    QWidget(parent)
{
    createVideoPlayer();
    createAudioPlayer();
    createTcpClient();
    createCmd();

    setLayout(createLayout());

    setupKeyHook();

    readSettings();
}

CentralWidget::~CentralWidget()
{
}

void CentralWidget::createVideoPlayer()
{
    videoPlayer = new Player;
    videoPlayer->setMinimumSize(VIDEO_H_RESOLUTION, VIDIO_V_RESOLUTION);
    videoPlayer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    videoPlayer->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    videoPlayer->playFile(VIDEO_STREAM);
    videoPlayer->showVolumeSlider(false);
}

void CentralWidget::createAudioPlayer()
{
    audioPlayer = new Player;
    audioPlayer->showVideoWidget(false);
    audioPlayer->playFile(AUDIO_STREAM);
}

void CentralWidget::createTcpClient()
{
    tcpClient = new TcpClient(SERVER_NAME, SERVER_PORT, this);
    connect(tcpClient, SIGNAL(connected()), this, SLOT(slotClientConnected()));
    connect(tcpClient, SIGNAL(disconnected()), this,
        SLOT(slotClientDisconnected()));
    connect(tcpClient, SIGNAL(signalDataReceived(const QString &)), this,
        SLOT(slotClientGetData(const QString &)));
    connect(tcpClient, SIGNAL(signalError(const QString &)),
        this, SLOT(slotClientConnectionError(const QString &)));
}

void CentralWidget::createCmd()
{
    cmd = std::unique_ptr< Cmd >(new Cmd());

    sendCmdTimer = new QTimer(this);
    connect(sendCmdTimer, SIGNAL(timeout()), this,
        SLOT(slotSendCmdToServer()));

    receiveCmdAckTimer = new QTimer(this);
    connect(receiveCmdAckTimer, SIGNAL(timeout()), this,
        SLOT(slotClientCommunicationTimeout()));
}

QPushButton *CentralWidget::createConnectButton()
{
    connectButton = new QPushButton;
    connectButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connectButton->setText(tr("Connect"));
    connectButton->setStyleSheet ("text-align: left;");
    connectButton->setMinimumWidth(CONNECT_BUTTON_WIDTH);
    connectButton->setIconSize(QSize(CONNECT_BUTTON_ICON_SIZE,
        CONNECT_BUTTON_ICON_SIZE));
    connectButton->setIcon(QIcon(CONNECT_IMAGE_CONNECT));
    connect(connectButton, SIGNAL(clicked()), this,
        SLOT(slotClientConnecting()));
    connError = false;

    return connectButton;
}

QLabel *CentralWidget::createConnectionStatusUi()
{
    connectionStatusLabel = new QLabel;
    connectionStatusLabel->setWordWrap(true);
    connectionStatusLabel->setText(tr(connetionStatus[CS_DISCONNECTED]));

    return connectionStatusLabel;
}

DistanceMeter *CentralWidget::createDistanceToObstacleUi()
{
    distMeter = new DistanceMeter;
    distMeter->setMinimumSize(180, 50);
    distMeter->setMaximumSize(180, 50);

    return distMeter;
}

BatteryWidget *CentralWidget::createBatteryChargeUi()
{
    return batteryWidget = new BatteryWidget;
}

Speedmeter *CentralWidget::createSpeedUi()
{
    speedmeter = new Speedmeter;
    speedmeter->setMinimumSize(180, 180);
    speedmeter->setMaximumSize(180, 180);

    return speedmeter;
}

QFrame *CentralWidget::createRightPanel()
{
    QVBoxLayout *rightPanelLayout = new QVBoxLayout;

    rightPanelLayout->addWidget(new Section(tr(SECTION_CONNECT)));
    rightPanelLayout->addWidget(createConnectButton());

    rightPanelLayout->addWidget(new Section(tr(SECTION_CONNECT_STATUS)));
    rightPanelLayout->addWidget(createConnectionStatusUi());

    rightPanelLayout->addWidget(new Section(tr(SECTION_BATTERY)));
    rightPanelLayout->addWidget(createBatteryChargeUi());

    rightPanelLayout->addWidget(new Section(tr(SECTION_OBSTACLE)));
    rightPanelLayout->addWidget(createDistanceToObstacleUi());

    rightPanelLayout->addWidget(new Section(tr(SECTION_SPEED)));
    rightPanelLayout->addWidget(createSpeedUi());
    rightPanelLayout->addStretch(1);

    QFrame *rightPanelWidget = new QFrame;
    rightPanelWidget->setMinimumWidth(200);
    rightPanelWidget->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    rightPanelWidget->setLayout(rightPanelLayout);

    return rightPanelWidget;
}

QWidget *CentralWidget::createCmdWidget()
{
    joystick = new JoystickButton;

    connect(joystick, SIGNAL(upButtonPressed()), this,
        SLOT(slotCarDirectCmd()));
    connect(joystick, SIGNAL(upButtonReleased()), this,
        SLOT(slotCarStopCmd()));
    connect(joystick, SIGNAL(downButtonPressed()), this,
        SLOT(slotCarReverseCmd()));
    connect(joystick, SIGNAL(downButtonReleased()), this,
        SLOT(slotCarStopCmd()));
    connect(joystick, SIGNAL(rightButtonPressed()), this,
        SLOT(slotCarRightCmd()));
    connect(joystick, SIGNAL(rightButtonReleased()), this,
        SLOT(slotCarStopCmd()));
    connect(joystick, SIGNAL(leftButtonPressed()), this,
        SLOT(slotCarLeftCmd()));
    connect(joystick, SIGNAL(leftButtonReleased()), this,
        SLOT(slotCarStopCmd()));
    connect(joystick, SIGNAL(centerButtonPressed()), this,
        SLOT(slotCarStopCmd()));

    return joystick;
}

QFrame *CentralWidget::createBottomLeftPanel()
{
    AnimatedButton *headlightsButton =
        new AnimatedButton(HEADLIGHTS_IMAGE_NORMAL, HEADLIGHTS_IMAGE_HOVERED,
        HEADLIGHTS_IMAGE_PRESSED);
    headlightsButton->setCheckable(true);

    connect(headlightsButton, SIGNAL(toggled(bool)), this,
        SLOT(slotHeadlightsStageChange(bool)));

    QShortcut *shortcut = new QShortcut(QKeySequence("F2"), headlightsButton);
    shortcut->setAutoRepeat(false);
    connect(shortcut, SIGNAL(activated()), headlightsButton, SLOT(click()));

    QVBoxLayout *bottomLeftPanelLayout = new QVBoxLayout;
    bottomLeftPanelLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    bottomLeftPanelLayout->addWidget(headlightsButton);

    QFrame *bottomLeftPanel = new QFrame;
    bottomLeftPanel->setLayout(bottomLeftPanelLayout);

    return bottomLeftPanel;
}

QFrame *CentralWidget::createBottomRightPanel()
{
    hornSignalButton = new AnimatedButton(HORN_IMAGE_NORMAL,
        HORN_IMAGE_HOVERED, HORN_IMAGE_PRESSED);
    connect(hornSignalButton, SIGNAL(pressed()), this,
        SLOT(slotHornSignalStart()));
    connect(hornSignalButton, SIGNAL(released()), this,
        SLOT(slotHornSignalStop()));

    QVBoxLayout *bottomRightPanelLayout = new QVBoxLayout;
    bottomRightPanelLayout->setAlignment(Qt::AlignTop | Qt::AlignRight);
    bottomRightPanelLayout->addWidget(hornSignalButton);

    QFrame *bottomRightPanel = new QFrame;
    bottomRightPanel->setLayout(bottomRightPanelLayout);

    return bottomRightPanel;
}

QFrame *CentralWidget::createBottomPanel()
{
    QHBoxLayout *bottomPanelLayout = new QHBoxLayout;
    bottomPanelLayout->addWidget(createBottomLeftPanel());
    bottomPanelLayout->addWidget(createCmdWidget());
    bottomPanelLayout->addWidget(createBottomRightPanel());

    QFrame *bottomPanel = new QFrame;
    bottomPanel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bottomPanel->setLayout(bottomPanelLayout);

    return bottomPanel;
}

QVBoxLayout *CentralWidget::createLeftPanel()
{
    QVBoxLayout *leftPanelLayout = new QVBoxLayout;
    leftPanelLayout->addWidget(videoPlayer);
    leftPanelLayout->addWidget(audioPlayer);
    leftPanelLayout->addWidget(createBottomPanel());

    return leftPanelLayout;
}

QHBoxLayout *CentralWidget::createLayout()
{
    QHBoxLayout *centralWidgetLayout = new QHBoxLayout;
    centralWidgetLayout->addLayout(createLeftPanel());
    centralWidgetLayout->addWidget(createRightPanel());

    return centralWidgetLayout;
}

void CentralWidget::setupKeyHook()
{
#ifdef Q_OS_LINUX
    keyMap[Qt::Key_Up].isPressed = false;
    keyMap[Qt::Key_Down].isPressed = false;
    keyMap[Qt::Key_Right].isPressed = false;
    keyMap[Qt::Key_Left].isPressed = false;
    keyMap[Qt::Key_Space].isPressed = false;
    for (std::map< int, keyState >::iterator it = keyMap.begin();
         it != keyMap.end(); it++)
    {
        it->second.releaseTimer = new QTimer(this);
        it->second.releaseTimer->setInterval(KEY_RELEASE_TIME);
        connect(it->second.releaseTimer, SIGNAL(timeout()), this,
            SLOT(slotKeyRelease()));
    }
#endif
    QCoreApplication::instance()->installEventFilter(this);
}

void CentralWidget::slotSendCmdToServer()
{
    if (tcpClient->isConnected())
        tcpClient->sendToServer(cmd->getData());
    cmd->clear();
}

void CentralWidget::slotCarDirectCmd()
{
    cmd->enqeue(Cmd::CMD_CAR_MOVE, Cmd::CMD_DATA_CAR_MOVE_RUN_DIRECT);
}

void CentralWidget::slotCarReverseCmd()
{
    cmd->enqeue(Cmd::CMD_CAR_MOVE, Cmd::CMD_DATA_CAR_MOVE_RUN_REVERSE);
}

void CentralWidget::slotCarLeftCmd()
{
    cmd->enqeue(Cmd::CMD_CAR_MOVE, Cmd::CMD_DATA_CAR_MOVE_TURN_LEFT);
}

void CentralWidget::slotCarRightCmd()
{
    cmd->enqeue(Cmd::CMD_CAR_MOVE, Cmd::CMD_DATA_CAR_MOVE_TURN_RIGHT);
}

void CentralWidget::slotCarStopCmd()
{
    cmd->enqeue(Cmd::CMD_CAR_MOVE, Cmd::CMD_DATA_CAR_MOVE_STOP);
}

void CentralWidget::slotHornSignalStart()
{
    cmd->enqeue(Cmd::CMD_HORN_SIGNAL, Cmd::CMD_DATA_HORN_SIGNAL_START);
}

void CentralWidget::slotHornSignalStop()
{
    cmd->enqeue(Cmd::CMD_HORN_SIGNAL, Cmd::CMD_DATA_HORN_SIGNAL_STOP);
}

void CentralWidget::slotHeadlightsStageChange(bool on)
{
    cmd->enqeue(Cmd::CMD_HEADLIGHTS, on ? Cmd::CMD_DATA_HEADLIGHTS_ON :
        Cmd::CMD_DATA_HEADLIGHTS_OFF);
}

#ifdef Q_OS_LINUX
void CentralWidget::keyPress(int key, void (CentralWidget::*cmd)())
{
    if (!keyMap[key].isPressed)
    {
        keyMap[key].isPressed = true;
        (this->*cmd)();
    }
    keyMap[key].releaseTimer->stop();
}

void CentralWidget::keyReleaseDelayed(int key)
{
    if (keyMap[key].isPressed)
        keyMap[key].releaseTimer->start();
}

void CentralWidget::keyRelease(int key, void (CentralWidget::*cmd)())
{
    if (cmd)
        (this->*cmd)();
    keyMap[key].isPressed = false;
}

void CentralWidget::slotKeyRelease()
{
    int key = 0;
    QTimer *timer = dynamic_cast< QTimer * >(QObject::sender());
    bool needStop = (keyMap[Qt::Key_Up].isPressed +
        keyMap[Qt::Key_Down].isPressed + keyMap[Qt::Key_Right].isPressed +
        keyMap[Qt::Key_Left].isPressed) == 1;

    for (std::map< int, keyState >::iterator it = keyMap.begin();
         it != keyMap.end(); it++)
    {
        if (it->second.isPressed && it->second.releaseTimer == timer)
        {
            key = it->first;
            break;
        }
    }

    switch (key)
    {
    case Qt::Key_Up:
        joystick->setUpButtonPressed(false);
        keyRelease(key, needStop ? &CentralWidget::slotCarStopCmd : nullptr);

        break;
    case Qt::Key_Down:
        joystick->setDownButtonPressed(false);
        keyRelease(key, needStop ? &CentralWidget::slotCarStopCmd : nullptr);

        break;
    case Qt::Key_Right:
        joystick->setRightButtonPressed(false);
        keyRelease(key, needStop ? &CentralWidget::slotCarStopCmd : nullptr);

        break;
    case Qt::Key_Left:
        joystick->setLeftButtonPressed(false);
        keyRelease(key, needStop ? &CentralWidget::slotCarStopCmd : nullptr);

        break;
    case Qt::Key_Space:
        hornSignalButton->setDown(false);
        keyRelease(key, &CentralWidget::slotHornSignalStop);

        break;
    default:
        break;
    }
}

bool CentralWidget::linuxEventFilter(QEvent *event)
{
    QKeyEvent *keyEvent = dynamic_cast< QKeyEvent * >(event);

    if (event->type() == QEvent::KeyPress)
    {
        int key = keyEvent->key();

        switch (key)
        {
        case Qt::Key_Up:
            joystick->setUpButtonPressed(true);
            keyPress(key, &CentralWidget::slotCarDirectCmd);

            return true;
        case Qt::Key_Down:
            joystick->setDownButtonPressed(true);
            keyPress(key, &CentralWidget::slotCarReverseCmd);

            return true;
        case Qt::Key_Right:
            joystick->setRightButtonPressed(true);
            keyPress(key, &CentralWidget::slotCarRightCmd);

            return true;
        case Qt::Key_Left:
            joystick->setLeftButtonPressed(true);
            keyPress(key, &CentralWidget::slotCarLeftCmd);

            return true;
        case Qt::Key_Space:
            hornSignalButton->setDown(true);
            keyPress(key, &CentralWidget::slotHornSignalStart);

            return true;
        default:
            break;
        }
    }
    else if (event->type() == QEvent::KeyRelease)
    {
        int key = keyEvent->key();

        switch (key)
        {
        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Right:
        case Qt::Key_Left:
        case Qt::Key_Space:
            keyReleaseDelayed(key);

            return true;
        default:
            break;
        }
    }
    return false;
}
#else

bool CentralWidget::generalEventFilter(QEvent *event)
{
    QKeyEvent *keyEvent = dynamic_cast< QKeyEvent * >(event);

    if (event->type() == QEvent::KeyPress)
    {
        switch (keyEvent->key())
        {
        case Qt::Key_Up:
            if (keyEvent->isAutoRepeat())
                return true;

            joystick->setUpButtonPressed(true);
            slotCarDirectCmd();

            return true;
        case Qt::Key_Down:
            if (keyEvent->isAutoRepeat())
                return true;

            joystick->setDownButtonPressed(true);
            slotCarReverseCmd();

            return true;
        case Qt::Key_Right:
            if (keyEvent->isAutoRepeat())
                return true;

            joystick->setRightButtonPressed(true);
            slotCarRightCmd();

            return true;
        case Qt::Key_Left:
            if (keyEvent->isAutoRepeat())
                return true;

            joystick->setLeftButtonPressed(true);
            slotCarLeftCmd();

            return true;
        case Qt::Key_Space:
            if (keyEvent->isAutoRepeat())
                return true;

            hornSignalButton->setDown(true);
            slotHornSignalStart();

            return true;
        default:
            break;
        }
    }
    else if (event->type() == QEvent::KeyRelease)
    {
        switch (keyEvent->key())
        {
        case Qt::Key_Up:
            if (keyEvent->isAutoRepeat())
                return true;

            joystick->setUpButtonPressed(false);
            slotCarStopCmd();

            return true;
        case Qt::Key_Down:
            if (keyEvent->isAutoRepeat())
                return true;

            joystick->setDownButtonPressed(false);
            slotCarStopCmd();

            return true;
        case Qt::Key_Right:
            if (keyEvent->isAutoRepeat())
                return true;

            joystick->setRightButtonPressed(false);
            slotCarStopCmd();

            return true;
        case Qt::Key_Left:
            if (keyEvent->isAutoRepeat())
                return true;

            joystick->setLeftButtonPressed(false);
            slotCarStopCmd();

            return true;
        case Qt::Key_Space:
            if (keyEvent->isAutoRepeat())
                return true;

            hornSignalButton->setDown(false);
            slotHornSignalStop();
            return true;
        default:
            break;
        }
    }

    return false;
}
#endif

bool CentralWidget::eventFilter(QObject *object, QEvent *event)
{
    (void)object;

#ifdef Q_OS_LINUX
    // QEvent::isAutoRepeat gives wrong values on linux
    return linuxEventFilter(event);
#else
    return generalEventFilter(event);
#endif
}

void CentralWidget::slotClientConnected()
{
    connectionStatusLabel->setText(tr(connetionStatus[CS_CONNECTED]));
    connectButton->setText(tr("Disconnect"));
    connectButton->setEnabled(true);
    connectButton->setIcon(QIcon(CONNECT_IMAGE_CONNECTED));

    disconnect(connectButton, SIGNAL(clicked()), this,
        SLOT(slotClientAbortConnect()));
    connect(connectButton, SIGNAL(clicked()), this,
        SLOT(slotClientDisconnecting()));

    cmd->clear();
    sendCmdTimer->start(CMD_SEND_INTERVAL);
    receiveCmdAckTimer->start(CMD_ACK_TIMEOUT);
    videoPlayer->restart();
    videoPlayer->showVideoWidget(true);
    sendSettings();
}

void CentralWidget::slotClientDisconnected()
{
    if (!connError)
        connectionStatusLabel->setText(tr(connetionStatus[CS_DISCONNECTED]));
    connectButton->setText(tr("Connect"));
    connectButton->setEnabled(true);
    connectButton->setIcon(QIcon(CONNECT_IMAGE_CONNECT));

    disconnect(connectButton, SIGNAL(clicked()), this,
        SLOT(slotClientAbortConnect()));
    disconnect(connectButton, SIGNAL(clicked()), this,
        SLOT(slotClientDisconnecting()));
    connect(connectButton, SIGNAL(clicked()), this,
        SLOT(slotClientConnecting()));

    sendCmdTimer->stop();
    receiveCmdAckTimer->stop();
    videoPlayer->showVideoWidget(false);
    updateBatteryChargeLevel(0);
    updateSpeed(0);
    updateDistanceToObstacle(0);
}

void CentralWidget::slotClientConnecting()
{
    connectionStatusLabel->setText(tr(connetionStatus[CS_CONNECTING]));
    connectButton->setText(tr("Abort"));
    connectButton->setEnabled(true);
    connectButton->setIcon(QIcon(CONNECT_IMAGE_CONNECTING));

    disconnect(connectButton, SIGNAL(clicked()), this,
        SLOT(slotClientConnecting()));
    connect(connectButton, SIGNAL(clicked()), this,
        SLOT(slotClientAbortConnect()));

    tcpClient->slotConnectToServer();
    connError = false;
}

void CentralWidget::slotClientAbortConnect()
{
    disconnect(connectButton, SIGNAL(clicked()), this,
        SLOT(slotClientAbortConnect()));

    slotClientDisconnected();

    tcpClient->abort();
}

void CentralWidget::slotClientDisconnecting()
{
    connectionStatusLabel->setText(tr(connetionStatus[CS_DISCONNECTING]));
    connectButton->setEnabled(false);
    tcpClient->slotDisconnectFromServer();
}

void CentralWidget::slotClientConnectionError(const QString &error)
{
    connectionStatusLabel->setText(tr(connetionStatus[CS_ERROR]) + error);
    connectButton->setEnabled(true);
    connError = true;

    slotClientDisconnected();
}

void CentralWidget::updateDistanceToObstacle(int dist)
{
    distMeter->setValue(dist == -1 ? distMeter->maximum() : dist);
}

void CentralWidget::updateBatteryChargeLevel(int batteryChargeLevel)
{
    batteryWidget->setValue(batteryChargeLevel);
}

void CentralWidget::updateSpeed(int speed)
{
    speedmeter->setValue(speed);
}

void CentralWidget::handleServerResp(char resp, const QString &respData)
{
    switch (resp)
    {
    case Cmd::CMD_RESP_ACK:
        receiveCmdAckTimer->start(CMD_ACK_TIMEOUT);
        break;
    case Cmd::CMD_RESP_DIST_TO_OBSTACLE:
    {
        bool ok;
        int dist = respData.toInt(&ok);
        updateDistanceToObstacle(ok ? dist : 0);
        break;
    }
    case Cmd::CMD_RESP_BATTERY_CHARGE_LEVEL:
    {
        bool ok;
        int batteryChargeLevel = respData.toInt(&ok);
        updateBatteryChargeLevel(ok ? batteryChargeLevel : 0);
        break;
    }
    case Cmd::CMD_RESP_SPEED:
    {
        bool ok;
        int speed = respData.toInt(&ok);
        updateSpeed(ok ? speed : 0);
        break;
    }
    default:
        qCritical("Wrong response from server");
        break;
    }
}

void CentralWidget::slotClientGetData(QString data)
{
    char resp;
    QString respData;

    cmd->parseResp(data);

    while (!cmd->getResp(resp, respData))
        handleServerResp(resp, respData);
}

void CentralWidget::slotClientCommunicationTimeout()
{
    tcpClient->close();
    connectionStatusLabel->setText(tr(connetionStatus[CS_ERROR]) +
        tr("Communication timeout"));
    qCritical("Communication timeout");
}

void CentralWidget::readSettings()
{
    QSettings settings;

    if (settings.contains(SETTINGS_KEY_TURN_METHOD))
        setTurnMethod(settings.value(SETTINGS_KEY_TURN_METHOD).toInt());
}

void CentralWidget::saveSettings()
{
    QSettings settings;

    settings.setValue(SETTINGS_KEY_TURN_METHOD, getTurnMethod());
}

void CentralWidget::sendSettings()
{
    cmd->enqeue(Cmd::CMD_CAR_TURN_METHOD, getTurnMethod());
}

int CentralWidget::getTurnMethod()
{
    return turnMethod;
}

void CentralWidget::setTurnMethod(int turnMethod)
{
    this->turnMethod = turnMethod;

    cmd->enqeue(Cmd::CMD_CAR_TURN_METHOD, turnMethod);

    saveSettings();
}
