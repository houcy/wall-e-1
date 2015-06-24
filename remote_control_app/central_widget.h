#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include <memory>
#include <map>

class Player;
class TcpClient;
class QTimer;
class Cmd;
class QComboBox;
class QPushButton;
class QLabel;
class QGroupBox;
class QFrame;
class QVBoxLayout;
class QHBoxLayout;
class JoystickButton;
class AnimatedButton;
class BatteryWidget;
class Speedmeter;
class DistanceMeter;

class CentralWidget : public QWidget
{
    Q_OBJECT

    enum ConnectionStatusId
    {
        CS_DISCONNECTED = 0,
        CS_CONNECTING = 1,
        CS_CONNECTED = 2,
        CS_DISCONNECTING = 3,
        CS_ERROR = 4
    };

    int turnMethod;
    Player *videoPlayer;
    Player *audioPlayer;
    TcpClient *tcpClient;
    QTimer *sendCmdTimer;
    QTimer *receiveCmdAckTimer;
    std::unique_ptr< Cmd > cmd;
    QPushButton *carDirectCmdButton;
    QPushButton *carReverseCmdButton;
    QPushButton *carRightCmdButton;
    QPushButton *carLeftCmdButton;
    QPushButton *carStopCmdButton;
    JoystickButton *joystick;
    AnimatedButton *hornSignalButton;
    QPushButton *connectButton;
    QLabel *connectionStatusLabel;
    BatteryWidget *batteryWidget;
    Speedmeter *speedmeter;
    DistanceMeter *distMeter;
    QLabel *speedLabel;
    bool connError;
#ifdef Q_OS_LINUX
    typedef struct
    {
        bool isPressed;
        QTimer *releaseTimer;
    } keyState;
    std::map< int, keyState > keyMap;
#endif
    std::map< ConnectionStatusId, const char * > connetionStatus =
    {
        { CS_DISCONNECTED, "Status: Disconnected" },
        { CS_CONNECTING, "Status: Connecting" },
        { CS_CONNECTED, "Status: Connected"},
        { CS_DISCONNECTING, "Status: Disconnecting" },
        { CS_ERROR, "Status: "}
    };

    bool eventFilter(QObject *object, QEvent *event) override;
#ifdef Q_OS_LINUX
    bool linuxEventFilter(QEvent *event);
    void keyPress(int key, void (CentralWidget::*cmd)());
    void keyReleaseDelayed(int key);
    void keyRelease(int key, void (CentralWidget::*cmd)());
#else
    bool generalEventFilter(QEvent *event);
#endif
    void createVideoPlayer();
    void createAudioPlayer();
    void createCmd();
    void createTcpClient();
    QLabel *createConnectionStatusUi();
    QPushButton *createConnectButton();
    DistanceMeter *createDistanceToObstacleUi();
    BatteryWidget *createBatteryChargeUi();
    Speedmeter *createSpeedUi();
    QFrame *createRightPanel();
    QWidget *createCmdWidget();
    QFrame *createBottomPanel();
    QFrame *createBottomLeftPanel();
    QFrame *createBottomRightPanel();
    QVBoxLayout *createLeftPanel();
    QHBoxLayout *createLayout();
    void setupKeyHook();
    void readSettings();
    void saveSettings();
    void sendSettings();
    void handleServerResp(char resp, const std::string &respData);
    void updateDistanceToObstacle(int dist);
    void updateBatteryChargeLevel(int batteryChargeLevelLabel);
    void updateSpeed(int speed);

public:
    explicit CentralWidget(QWidget *parent = 0);
    CentralWidget(const CentralWidget &) = delete;
    CentralWidget &operator=(const CentralWidget &) = delete;
    ~CentralWidget();

    void setTurnMethod(int turnMethod);
    int getTurnMethod();

public slots:
    void slotSendCmdToServer();
    void slotCarDirectCmd();
    void slotCarReverseCmd();
    void slotCarLeftCmd();
    void slotCarRightCmd();
    void slotCarStopCmd();
    void slotHornSignalStart();
    void slotHornSignalStop();
    void slotHeadlightsStageChange(bool on);
    void slotClientConnected();
    void slotClientDisconnected();
    void slotClientConnecting();
    void slotClientDisconnecting();
    void slotClientAbortConnect();
    void slotClientConnectionError(const QString &error);
    void slotClientGetData(const std::string &data);
    void slotClientCommunicationTimeout();
    void slotKeyRelease();
};

#endif // CENTRALWIDGET_H
