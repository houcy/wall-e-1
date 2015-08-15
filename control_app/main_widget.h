#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <memory>

class TcpServer;
class Gpio;
class Car;
class QGroupBox;
class QComboBox;
class Cmd;
class QTimer;
class QLabel;
class SlidingStackedWidget;
class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QHostAddress;
class MediaStream;
class Horn;
class UltrasonicDistSensor;
class BatteryMonitor;
class HallSpeedSensor;
class Headlights;
class Joystick;
class Speaker;

class MainWidget : public QWidget
{
    Q_OBJECT

    TcpServer *server;
    std::unique_ptr< Car > car;
    QComboBox *carTurnMethod;
    std::unique_ptr< Cmd > cmd;
    QTimer *cmdTimeoutTimer;
    QLabel *connectionStatusLabel;
    QLabel *distLabel;
    QLabel *batteryVoltageLabel;
    QLabel *batteryChargeLabel;
    QLabel *speedLabel;
    MediaStream *mediaStream;
    Horn *horn;
    std::unique_ptr< UltrasonicDistSensor > distSensor;
    QTimer *distSensorTimer;
    int distToObstacle;
    int batteryChargeLevel;
    int speed;
    std::unique_ptr< BatteryMonitor > batteryMonitor;
    QTimer *batteryMonitorTimer;
    std::unique_ptr< HallSpeedSensor > hallSpeedSensor;
    QTimer *hallSpeedSensorTimer;
    SlidingStackedWidget *slideWidget;
    std::unique_ptr< Headlights > headlights;
    bool sendTurnMethod;
    Joystick *joystick;
    Speaker *speaker;

    QGroupBox *motorTestCreate(int motor, const QString &groupText);
    QWidget *carTestCreate();
    void executeCommand(int cmd, const std::string &cmdData);
    QLabel *createPageTitle(const QString &title);
    QWidget *createStatusPage();
    QWidget *createMotorTestPage();
    QWidget *createCarTestPage();
    QString getConnectionStatus() const;
    void sendResponseToClient();
    void readSettings();
    void saveSettings();
    void setupWindow();
    void setupCmd();
    void setupServer();
    void setupCar();
    void setupMediaStream();
    void setupHorn();
    void setupDistSensor();
    void setupBatteryMonitor();
    void setupSpeedSensor();
    void setupHeadlights();
    void setupJoystick();
    void setupSpeaker();
    SlidingStackedWidget *createSlideWidget();
    QVBoxLayout *createNavigationButton(const char *imagePath, SlidingStackedWidget *sw,
        const char *slot);
    QHBoxLayout *createMainLayout();

public:
    explicit MainWidget(QWidget *parent = 0);
    MainWidget(const QWidget &) = delete;
    MainWidget &operator=(const MainWidget &) = delete;
    ~MainWidget();

signals:

public slots:
    void slotDataReceived(const std::string &data);
    void slotMotorTestCmd(int motor, int cmd);
    void slotCarTestFreeRun(bool);
    void slotCarTestDirect(bool);
    void slotCarTestDirectDelayed();
    void slotCarTestReverse(bool);
    void slotCarTestReverseDelayed();
    void slotCarTestTurnLeft(bool);
    void slotCarTestTurnLeftDelayed();
    void slotCarTestTurnRight(bool);
    void slotCarTestTurnRightDelayed();
    void slotCarTestStop(bool);
    void slotSetTurnMethod(int index);
    void slotCmdTimeoutHandler();
    void slotUpdateConnection();
    void slotUpdateDistance();
    void slotUpdateBatteryMonitor();
    void slotUpdateSpeed();
    void slotJoystickEvent(std::uint8_t type, std::uint8_t num,
        std::int16_t value);
};

#endif // MAINWIDGET_H
