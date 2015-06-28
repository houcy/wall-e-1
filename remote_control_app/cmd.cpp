#include "cmd.h"
#include <QTextStream>
#include <QDebug>
#include <sstream>

Cmd::Cmd()
{
    emptyCmd += cmdStart;
    emptyCmd += CMD_NONE;
    emptyCmd += cmdDelim;
    emptyCmd += std::to_string(CMD_DATA_NONE);
    emptyCmd += cmdEnd;
}

void Cmd::enqeue(char cmd, const std::string &cmdData)
{
    cmdBuf += cmdStart;
    cmdBuf += cmd;
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
    char delim, resp;
    std::string respData;
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

        if (!(dataStream >> resp))
        {
            qWarning() << "Failed to extract response ID";
            return;
        }

        switch (resp)
        {
        case Cmd::CMD_RESP_ACK:
        case Cmd::CMD_RESP_DIST_TO_OBSTACLE:
        case Cmd::CMD_RESP_BATTERY_CHARGE_LEVEL:
        case Cmd::CMD_RESP_SPEED:
            if (!(dataStream >> delim))
            {
                qWarning() << "Failed to extract delimiter of command";
                return;
            }

            if (delim != cmdDelim)
            {
                qWarning() << "Wrong delimiter of command";
                return;
            }

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

        RespItem respItem = { resp, respData };
        respList.append(respItem);
    } while (!dataStream.eof());
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
