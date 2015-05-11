#ifndef CMD_H
#define CMD_H

#include <QString>
#include <QList>

class Cmd
{
    typedef struct
    {
        char cmd;
        char cmdData;
    } CmdItem;
    QList< CmdItem > cmdList;
    QByteArray respBuf;

public:
    enum CmdFormat
    {
        CMD_START = ':',
        CMD_END = '\n'
    };
    enum CmdNumer
    {
        CMD_NONE = '0',
        CMD_CAR_MOVE = '1',
        CMD_CAR_TURN_METHOD ='2',
        CMD_HORN_SIGNAL = '3',
        CMD_HEADLIGHTS = '4'
    };
    enum CmdDataNone
    {
        CMD_DATA_NONE = '0'
    };
    enum CmdCarMoveData
    {
        CMD_DATA_CAR_MOVE_RUN_FREE = '0',
        CMD_DATA_CAR_MOVE_RUN_DIRECT = '1',
        CMD_DATA_CAR_MOVE_RUN_REVERSE = '2',
        CMD_DATA_CAR_MOVE_TURN_LEFT = '3',
        CMD_DATA_CAR_MOVE_TURN_RIGHT = '4',
        CMD_DATA_CAR_MOVE_STOP = '5'
    };
    enum CmdDataTurnMethod
    {
        CMD_DATA_TURN_METHOD_DIRERENTIAL = '0',
        CMD_DATA_TURN_METHOD_SKID_STEER = '1'
    };
    enum CmdDataHornSignal
    {
        CMD_DATA_HORN_SIGNAL_START = '0',
        CMD_DATA_HORN_SIGNAL_STOP = '1'
    };
    enum CmdDataHeadlights
    {
        CMD_DATA_HEADLIGHTS_ON = '0',
        CMD_DATA_HEADLIGHTS_OFF = '1'
    };
    enum CmdRespNumer
    {
        CMD_RESP_ACK = '0',
        CMD_RESP_DIST_TO_OBSTACLE = '1',
        CMD_RESP_BATTERY_CHARGE_LEVEL = '2',
        CMD_RESP_SPEED = '3'
    };

    void parse(QString &data);
    int get(char &cmd, char &cmdData);
    void enqueueResp(char cmdResp, const QString &respData);
    const char *getRespData();
    void clearResp();
};

#endif // CMD_H