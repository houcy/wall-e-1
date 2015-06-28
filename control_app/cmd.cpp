#include "cmd.h"
#include "log.h"
#include <sstream>

void Cmd::parse(const std::string &data)
{
    char delim, cmd;
    std::string cmdData;
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

        if (!(dataStream >> cmd))
        {
            warning("Failed to extract command ID");
            return;
        }

        switch (cmd)
        {
        case CMD_NONE:
        case CMD_CAR_MOVE:
        case CMD_CAR_TURN_METHOD:
        case CMD_HORN_SIGNAL:
        case CMD_HEADLIGHTS:
            if (!(dataStream >> delim))
            {
                warning("Failed to extract delimiter of command");
                return;
            }

            if (delim != cmdDelim)
            {
                warning("Wrong delimiter of command");
                return;
            }

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

        CmdItem cmdItem = { cmd, cmdData };
        cmdList.append(cmdItem);
    } while (!dataStream.eof());
}

int Cmd::get(char &cmd, std::string &cmdData)
{
    if (!cmdList.isEmpty())
    {
        CmdItem cmdItem = cmdList.takeFirst();
        cmd = cmdItem.cmd;
        cmdData = cmdItem.cmdData;

        return 0;
    }
    else
        return -1;
}

void Cmd::enqueueResp(char resp, const std::string &respData)
{
    respBuf += cmdStart;
    respBuf += resp;
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
