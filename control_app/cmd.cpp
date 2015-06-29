#include "cmd.h"
#include "log.h"
#include <sstream>

void Cmd::parse(const std::string &data)
{
    char delim;
    long cmdId;
    std::string cmdIdStr, cmdData;
    std::istringstream dataStream(data);

    do
    {
        if (!(dataStream >> delim))
        {
            warning("Failed to extract start delimiter");
            return;
        }

        if (delim != cmdStart)
        {
            warning("Wrong start delimiter of command");
            return;
        }

        if (!std::getline(dataStream, cmdIdStr, cmdDelim))
        {
            warning("Failed to extract command ID");
            return;
        }

        cmdId = strtol(cmdIdStr.c_str(), nullptr, 10);
        if (!cmdId || cmdId == LONG_MAX || cmdId == LONG_MIN)
        {
            warning("Failed to convert command ID");
            return;
        }

        switch (cmdId)
        {
        case CMD_NONE:
        case CMD_CAR_MOVE:
        case CMD_CAR_TURN_METHOD:
        case CMD_HORN_SIGNAL:
        case CMD_HEADLIGHTS:
            if (!std::getline(dataStream, cmdData, cmdEnd))
            {
                warning("Failed to extract data of command");
                return;
            }

            // Set eofbit of stream by reading new character
            dataStream.peek();
            break;
        default:
            warning("Wrong command ID");
            return;
        };

        CmdItem cmdItem = { cmdId, cmdData };
        cmdList.append(cmdItem);
    } while (!dataStream.eof());
}

int Cmd::get(int &cmdId, std::string &cmdData)
{
    if (!cmdList.isEmpty())
    {
        CmdItem cmdItem = cmdList.takeFirst();
        cmdId = cmdItem.cmdId;
        cmdData = cmdItem.cmdData;

        return 0;
    }
    else
        return -1;
}

void Cmd::enqueueResp(int cmdRespId, const std::string &respData)
{
    std::ostringstream oss;

    oss << cmdRespId;

    respBuf += cmdStart;
    respBuf += oss.str();
    respBuf += cmdDelim;
    respBuf += respData;
    respBuf += cmdEnd;
}

const char *Cmd::getRespData()
{
    return respBuf.c_str();
}

void Cmd::clearResp()
{
    respBuf.clear();
}
