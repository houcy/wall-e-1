#include "tcp_client.h"
#include <QTcpSocket>
#include <QDataStream>
#include <QDebug>

TcpClient::TcpClient(const QString &serverName, int serverPort,
    QObject *parent) : QTcpSocket(parent), nextBlockSize(0)
{
    this->serverName = serverName;
    this->serverPort = serverPort;
    connected = 0;

    connect(this, SIGNAL(connected()),
            SLOT(slotConnected()));
    connect(this, SIGNAL(disconnected()),
            this, SLOT(slotDisconnected()));
    connect(this, SIGNAL(readyRead()),
            this, SLOT(slotReadReady()));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(slotError(QAbstractSocket::SocketError)));
}

void TcpClient::slotReadReady()
{
    char *packet;
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_8);

    for (;;)
    {
        if (!bytesAvailable())
            break;

        in >> packet;
        std::string packetStr(packet);
        delete[] packet;

        emit signalDataReceived(packetStr);
    }
}

void TcpClient::slotError(QAbstractSocket::SocketError err)
{
    (void)err;

    qWarning() << this->errorString();

    emit signalError(this->errorString());

    connected = 0;
}

void TcpClient::slotConnected()
{
    qDebug() << "TCP client is connected\n";

    connected = 1;
}

void TcpClient::slotDisconnected()
{
    qDebug() << "TCP client is disconnected\n";

    connected = 0;
}

void TcpClient::slotConnectToServer()
{
    connectToHost(serverName, serverPort);
}

void TcpClient::sendToServer(const char *packet)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << packet;

    if (write(arrBlock) == -1)
        qWarning("Failed send data to server");
}

bool TcpClient::isConnected() const noexcept
{
    return connected;
}

void TcpClient::slotDisconnectFromServer()
{
    disconnectFromHost();
}
