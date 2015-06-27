#ifndef CMD_H
#define CMD_H

#include <QList>

class Cmd
{
    typedef struct
    {
        char cmd;
        std::string cmdData;
    } CmdItem;
    QList< CmdItem > cmdList;
    std::string respBuf;
    const char cmdStart = ':';
    const char cmdEnd = ';';

public:
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
        CMD_DATA_NONE = 1
    };
    enum CmdCarMoveData
    {
        CMD_DATA_CAR_MOVE_RUN_FREE = 1,
        CMD_DATA_CAR_MOVE_RUN_DIRECT = 2,
        CMD_DATA_CAR_MOVE_RUN_REVERSE = 3,
        CMD_DATA_CAR_MOVE_TURN_LEFT = 4,
        CMD_DATA_CAR_MOVE_TURN_RIGHT = 5,
        CMD_DATA_CAR_MOVE_STOP = 6
    };
    enum CmdDataTurnMethod
    {
        CMD_DATA_TURN_METHOD_DIRERENTIAL = 1,
        CMD_DATA_TURN_METHOD_SKID_STEER = 2
    };
    enum CmdDataHornSignal
    {
        CMD_DATA_HORN_SIGNAL_START = 1,
        CMD_DATA_HORN_SIGNAL_STOP = 2
    };
    enum CmdDataHeadlights
    {
        CMD_DATA_HEADLIGHTS_ON = 1,
        CMD_DATA_HEADLIGHTS_OFF = 2
    };
    enum CmdRespNumer
    {
        CMD_RESP_ACK = '0',
        CMD_RESP_DIST_TO_OBSTACLE = '1',
        CMD_RESP_BATTERY_CHARGE_LEVEL = '2',
        CMD_RESP_SPEED = '3'
    };

    void parse(const std::string &data);
    int get(char &cmd, std::string &cmdData);
    void enqueueResp(char cmdResp, const std::string &respData);
    const char *getRespData();
    void clearResp();
};

#endif // CMD_H
