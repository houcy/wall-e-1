#include "cmd.h"
#include "log.h"
#include <QTextStream>

void Cmd::parse(QString &data)
{
    char delim, cmd;
    QString cmdData;
    QTextStream textStream(&data);

    while (!textStream.atEnd())
    {
        textStream >> delim;

        if (delim != CMD_START)
        {
            debug("Wrong start of command");
            continue;
        }

        textStream >> cmd;

        switch (cmd)
        {
        case CMD_NONE:
        case CMD_CAR_MOVE:
        case CMD_CAR_TURN_METHOD:
        case CMD_HORN_SIGNAL:
        case CMD_HEADLIGHTS:
           textStream >> cmdData;
           break;
        default:
           debug("Wrong car command");
           continue;
        };

        textStream >> delim;
        if (delim != CMD_END)
        {
            debug("Wrong end of command");
            continue;
        }

        CmdItem cmdItem = { cmd, cmdData };
        cmdList.append(cmdItem);
    }
}

int Cmd::get(char &cmd, QString &cmdData)
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

void Cmd::enqueueResp(char resp, const QString &respData)
{
    respBuf += CMD_START;
    respBuf += resp;
    respBuf += respData;
    respBuf += CMD_END;
}

const char *Cmd::getRespData()
{
    return respBuf.constData();
}

void Cmd::clearResp()
{
    respBuf.clear();
}
