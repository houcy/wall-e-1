#include "cmd.h"
#include "log.h"
#include <QTextStream>

void Cmd::parse(QString &data)
{
    char cmdStart, cmd, cmdData, cmdEnd;
    QTextStream textStream(&data);

    while (!textStream.atEnd())
    {
        textStream >> cmdStart;

        if (cmdStart == CMD_START)
        {
            textStream >> cmd >> cmdData >> cmdEnd;

            if (cmdEnd != CMD_END)
            {
                debug("Wrong end of command");
                return;
            }

            switch (cmd)
            {
            case CMD_NONE:
                break;
            case CMD_CAR_MOVE:
                switch (cmdData)
                {
                case CMD_DATA_CAR_MOVE_RUN_FREE:
                case CMD_DATA_CAR_MOVE_RUN_DIRECT:
                case CMD_DATA_CAR_MOVE_RUN_REVERSE:
                case CMD_DATA_CAR_MOVE_TURN_LEFT:
                case CMD_DATA_CAR_MOVE_TURN_RIGHT:
                case CMD_DATA_CAR_MOVE_STOP:
                    break;
                default:
                    debug("Wrong car command data");
                    return;
                }
                break;
            case CMD_CAR_TURN_METHOD:
                switch (cmdData)
                {
                case CMD_DATA_TURN_METHOD_DIRERENTIAL:
                case CMD_DATA_TURN_METHOD_SKID_STEER:
                    break;
                default:
                    debug("Wrong car command data");
                    return;
                }
                break;
            case CMD_HORN_SIGNAL:
                switch (cmdData)
                {
                case CMD_DATA_HORN_SIGNAL_START:
                case CMD_DATA_HORN_SIGNAL_STOP:
                    break;
                default:
                    debug("Wrong horn signal action");
                    return;
                }
                break;
            case CMD_HEADLIGHTS:
                switch (cmdData)
                {
                case CMD_DATA_HEADLIGHTS_ON:
                case CMD_DATA_HEADLIGHTS_OFF:
                    break;
                default:
                    debug("Wrong headlights action");
                    return;
                }
                break;
            default:
                debug("Wrong car command");
                return;
            };

            CmdItem cmdItem = { cmd, cmdData };
            cmdList.append(cmdItem);
        }
    }
}

int Cmd::get(char &cmd, char &cmdData)
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
