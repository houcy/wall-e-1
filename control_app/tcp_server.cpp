#include "tcp_server.h"
#include "log.h"
#include <QTcpSocket>
#include <QString>
#include <QDataStream>

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{
    nextBlockSize = 0;
    clientConnected = false;

    connect(this, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
}

void TcpServer::slotNewConnection()
{
    clientSocket = nextPendingConnection();

    connect(clientSocket, SIGNAL(disconnected()),
            clientSocket, SLOT(deleteLater()));
    connect(clientSocket, SIGNAL(readyRead()),
            this, SLOT(slotReadClient()));
    connect(clientSocket, SIGNAL(disconnected()),
            this, SLOT(slotClientDisconnected()));

    debug("Client connected");
    clientConnected = true;
    emit connectionStateChanged();
}

void TcpServer::slotReadClient()
{
    QString packet;
    QTcpSocket *clientSocket = (QTcpSocket *)sender();
    QDataStream in(clientSocket);
    in.setVersion(QDataStream::Qt_4_8);

    for(;;)
    {
        if (!clientSocket->bytesAvailable())
            break;

        in >> packet;
        emit signalDataReceived(packet);
    }
}

void TcpServer::sendToClient(const char *data)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << QString(data);

    clientSocket->write(arrBlock);
}

void TcpServer::slotClientDisconnected()
{
    debug("Client disconnected");
    clientConnected = false;
    emit connectionStateChanged();
}

bool TcpServer::isClientConnected() const noexcept
{
    return clientConnected;
}

void TcpServer::disconnectClient()
{
    if (clientConnected)
        clientSocket->close();
}

QHostAddress TcpServer::getClientAddr() const
{
    return clientSocket->peerAddress();
}
