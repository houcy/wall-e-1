#include "cmd.h"
#include <QTextStream>
#include <QDebug>
#include <sstream>

Cmd::Cmd()
{
    emptyCmd += cmdStart;
    emptyCmd += std::to_string(CMD_NONE);
    emptyCmd += cmdDelim;
    emptyCmd += std::to_string(CMD_DATA_NONE);
    emptyCmd += cmdEnd;
}

void Cmd::enqeue(int cmdId, const std::string &cmdData)
{
    cmdBuf += cmdStart;
    cmdBuf += std::to_string(cmdId);
    cmdBuf += cmdDelim;
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
    char delim;
    long respId;
    std::string respIdStr, respData;
    std::istringstream dataStream(data);

    do
    {
        if (!(dataStream >> delim))
        {
            qWarning() << "Failed to extract start delimiter of response";
            return;
        }

        if (delim != cmdStart)
        {
            qWarning() << "Wrong start delimiter of response";
            return;
        }

        if (!std::getline(dataStream, respIdStr, cmdDelim))
        {
            qWarning() << "Failed to extract response ID";
            return;
        }

        respId = strtol(respIdStr.c_str(), nullptr, 10);
        if (!respId || respId == LONG_MAX || respId == LONG_MIN)
        {
            qWarning() << "Failed to convert response ID";
            return;
        }

        switch (respId)
        {
        case Cmd::CMD_RESP_ACK:
        case Cmd::CMD_RESP_DIST_TO_OBSTACLE:
        case Cmd::CMD_RESP_BATTERY_CHARGE_LEVEL:
        case Cmd::CMD_RESP_SPEED:
        case Cmd::CMD_RESP_TURN_METHOD:
            if (!std::getline(dataStream, respData, cmdEnd))
            {
                qWarning() << "Failed to extract data of command";
                return;
            }

            // Set eofbit of stream by reading new character
            dataStream.peek();
            break;
        default:
            qWarning() << "Wrong response ID";
            return;
        };

        RespItem respItem = { respId, respData };
        respList.append(respItem);
    } while (!dataStream.eof());
}

int Cmd::getResp(int &respId, std::string &respData)
{
    if (!respList.isEmpty())
    {
        RespItem respItem = respList.takeFirst();
        respId = respItem.respId;
        respData = respItem.respData;

        return 0;
    }

    return -1;
}
