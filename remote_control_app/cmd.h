#ifndef CMD_H
#define CMD_H

#include <QByteArray>
#include <QList>

class Cmd
{
    typedef struct
    {
        char resp;
        std::string respData;
    } RespItem;

    std::string cmdBuf;
    std::string emptyCmd;
    QList< RespItem > respList;
    const char cmdStart = ':';
    const char cmdEnd = ';';

public:
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

    Cmd();
    void enqeue(char cmd, const std::string &cmdData);
    void clear();
    const char *getData() const;
    unsigned int size() const;
    void parseResp(const std::string &data);
    int getResp(char &resp, std::string &respData);
};

#endif // CMD_H
