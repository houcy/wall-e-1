#include "cmd.h"
#include "log.h"
#include <sstream>

void Cmd::parse(const std::string &data)
{
    char delim, cmd;
    std::string cmdData;
    std::istringstream dataStream(data);

    while (!dataStream.eof())
    {
        dataStream >> delim;

        if (delim != cmdStart)
        {
            debug("Wrong start of command");
            continue;
        }

        dataStream >> cmd;

        switch (cmd)
        {
        case CMD_NONE:
        case CMD_CAR_MOVE:
        case CMD_CAR_TURN_METHOD:
        case CMD_HORN_SIGNAL:
        case CMD_HEADLIGHTS:
           std::getline(dataStream, cmdData, cmdEnd);
           if (dataStream.bad())
           {
               debug("Failed to get command's data");
               continue;
           }
           break;
        default:
           debug("Wrong car command");
           continue;
        };

        CmdItem cmdItem = { cmd, cmdData };
        cmdList.append(cmdItem);
    }
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
