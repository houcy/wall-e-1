#include "cmd.h"
#include <QTextStream>
#include <QDebug>

Cmd::Cmd()
{
    emptyCmd += CMD_START;
    emptyCmd += CMD_NONE;
    emptyCmd += QString::number(CMD_DATA_NONE);
    emptyCmd += CMD_END;
}

void Cmd::enqeue(char cmd, const QString &cmdData)
{
    cmdBuf += CMD_START;
    cmdBuf += cmd;
    cmdBuf += cmdData;
    cmdBuf += CMD_END;
}

const char *Cmd::getData() const
{
    if (cmdBuf.isEmpty())
        return emptyCmd.constData();

    return cmdBuf.constData();
}

unsigned int Cmd::size() const
{
    if (cmdBuf.isEmpty())
        return emptyCmd.size();

    return cmdBuf.size();
}

void Cmd::clear()
{
    if (!cmdBuf.isEmpty())
        cmdBuf.clear();
}

void Cmd::parseResp(QString &data)
{
    char respStart, resp, respEnd;
    QString respData;
    QTextStream textStream(&data);

    while (!textStream.atEnd())
    {
        textStream >> respStart;

        if (respStart != CMD_START)
            continue;

        textStream >> resp;

        switch (resp)
        {
        case Cmd::CMD_RESP_ACK:
            break;
        case Cmd::CMD_RESP_DIST_TO_OBSTACLE:
        case Cmd::CMD_RESP_BATTERY_CHARGE_LEVEL:
        case Cmd::CMD_RESP_SPEED:
            textStream >> respData;
            break;
        default:
            qDebug() << "Wrong response";
            return;
        };

        textStream >> respEnd;
        if (respEnd != CMD_END)
        {
            qDebug() << "Wrong end of response";
            return;
        }

        RespItem respItem = { resp, respData };
        respList.append(respItem);
    }
}

int Cmd::getResp(char &resp, QString &respData)
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
