#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>

class QTcpSocket;

class TcpServer : public QTcpServer
{
    Q_OBJECT

    quint16 nextBlockSize;
    bool clientConnected;
    QTcpSocket *clientSocket;

public:
    explicit TcpServer(QObject *parent = 0);
    TcpServer(const TcpServer &) = delete;
    TcpServer &operator=(const TcpServer &) = delete;
    bool isClientConnected() const noexcept;
    void sendToClient(const char *data);
    void disconnectClient();
    QHostAddress getClientAddr() const;

signals:
    void signalDataReceived(const std::string &packet);
    void connectionStateChanged();

public slots:
    void slotNewConnection();
    void slotReadClient();
    void slotClientDisconnected();
};

#endif // TCPSERVER_H
