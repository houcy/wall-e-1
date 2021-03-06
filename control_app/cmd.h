#ifndef CMD_H
#define CMD_H

#include <QList>

class Cmd
{
    typedef struct
    {
        int cmdId;
        std::string cmdData;
    } CmdItem;
    QList< CmdItem > cmdList;
    std::string respBuf;
    const char cmdStart = ':';
    const char cmdDelim = ',';
    const char cmdEnd = ';';

public:
    enum CmdNumer
    {
        CMD_NONE = 1,
        CMD_CAR_MOVE = 2,
        CMD_CAR_SET_TURN_METHOD = 3,
        CMD_CAR_GET_TURN_METHOD = 4,
        CMD_HORN_SIGNAL = 5,
        CMD_HEADLIGHTS = 6,
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
        CMD_DATA_TURN_METHOD_UNDEF = 0,
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
        CMD_RESP_ACK = 1,
        CMD_RESP_DIST_TO_OBSTACLE = 2,
        CMD_RESP_BATTERY_CHARGE_LEVEL = 3,
        CMD_RESP_SPEED = 4,
        CMD_RESP_TURN_METHOD = 5
    };

    void parse(const std::string &data);
    int get(int &cmdId, std::string &cmdData);
    void enqueueResp(int cmdRespId, const std::string &respData);
    const char *getRespData();
    void clearResp();
};

#endif // CMD_H
