#include "cmd.h"
#include <QTextStream>
#include <QDebug>
#include <sstream>

Cmd::Cmd()
{
    emptyCmd += cmdStart;
    emptyCmd += CMD_NONE;
    emptyCmd += CMD_DATA_NONE;
    emptyCmd += cmdEnd;
}

void Cmd::enqeue(char cmd, const std::string &cmdData)
{
    cmdBuf += cmdStart;
    cmdBuf += cmd;
    cmdBuf += cmdData;
    cmdBuf += cmdEnd;
}

const char *Cmd::getData() const
{
    if (cmdBuf.empty())
        return emptyCmd.c_str();

    return cmdBuf.c_str();
}

unsigned int Cmd::size() const
{
    if (cmdBuf.empty())
        return emptyCmd.size();

    return cmdBuf.size();
}

void Cmd::clear()
{
    if (!cmdBuf.empty())
        cmdBuf.clear();
}

void Cmd::parseResp(const std::string &data)
{
    char respStart, resp;
    std::string respData;
    std::istringstream dataStream(data);

    while (!dataStream.eof())
    {
        dataStream >> respStart;

        if (respStart != cmdStart)
        {
            qDebug() << "Wrong start of response";
            continue;
        }

        dataStream >> resp;

        switch (resp)
        {
        case Cmd::CMD_RESP_ACK:
        case Cmd::CMD_RESP_DIST_TO_OBSTACLE:
        case Cmd::CMD_RESP_BATTERY_CHARGE_LEVEL:
        case Cmd::CMD_RESP_SPEED:
            std::getline(dataStream, respData, cmdEnd);
            break;
        default:
            qDebug() << "Wrong response";
            return;
        };

        RespItem respItem = { resp, respData };
        respList.append(respItem);
    }
}

int Cmd::getResp(char &resp, std::string &respData)
{
    if (!respList.isEmpty())
    {
        RespItem respItem = respList.takeFirst();
        resp = respItem.resp;
        respData = respItem.respData;

        return 0;
    }

    return -1;
}
