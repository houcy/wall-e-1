#ifndef CMD_H
#define CMD_H

#include <QByteArray>
#include <QList>
#include <QString>

class Cmd
{
    typedef struct
    {
        char resp;
        QString respData;
    } RespItem;

    QByteArray cmdBuf;
    QByteArray emptyCmd;
    QList< RespItem > respList;

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
        CMD_CAR_TURN_METHOD = '2',
        CMD_HORN_SIGNAL = '3',
        CMD_HEADLIGHTS = '4'
    };
    enum CmdDataNone
    {
        CMD_DATA_NONE
    };
    enum CmdCarMoveData
    {
        CMD_DATA_CAR_MOVE_RUN_FREE,
        CMD_DATA_CAR_MOVE_RUN_DIRECT,
        CMD_DATA_CAR_MOVE_RUN_REVERSE,
        CMD_DATA_CAR_MOVE_TURN_LEFT,
        CMD_DATA_CAR_MOVE_TURN_RIGHT,
        CMD_DATA_CAR_MOVE_STOP
    };
    enum CmdDataTurnMethod
    {
        CMD_DATA_TURN_METHOD_DIRERENTIAL,
        CMD_DATA_TURN_METHOD_SKID_STEER
    };
    enum CmdDataHornSignal
    {
        CMD_DATA_HORN_SIGNAL_START,
        CMD_DATA_HORN_SIGNAL_STOP
    };
    enum CmdDataHeadlights
    {
        CMD_DATA_HEADLIGHTS_ON,
        CMD_DATA_HEADLIGHTS_OFF
    };
    enum CmdRespNumer
    {
        CMD_RESP_ACK = '0',
        CMD_RESP_DIST_TO_OBSTACLE = '1',
        CMD_RESP_BATTERY_CHARGE_LEVEL = '2',
        CMD_RESP_SPEED = '3'
    };

    Cmd();
    void enqeue(char cmd, const QString &cmdData);
    void clear();
    const char *getData() const;
    unsigned int size() const;
    void parseResp(QString &data);
    int getResp(char &resp, QString &respData);
};

#endif // CMD_H
