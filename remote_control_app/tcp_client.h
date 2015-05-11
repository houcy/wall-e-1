#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QAbstractSocket>
#include <QTcpSocket>
#include <QString>

class QObject;

class TcpClient : public QTcpSocket
{
    Q_OBJECT

    QString serverName;
    int serverPort;
    quint16 nextBlockSize;
    bool connected;

public:
    TcpClient(const QString &serverName, int serverPort, QObject *parent = 0);
    TcpClient(const TcpClient &) = delete;
    TcpClient &operator=(const TcpClient &) = delete;
    bool isConnected() const noexcept;
    void sendToServer(const char *packet);

signals:
    void signalError(const QString &error);
    void signalDataReceived(QString data);

public slots:
    void slotReadReady();
    void slotError(QAbstractSocket::SocketError err);
    void slotConnectToServer();
    void slotConnected();
    void slotDisconnectFromServer();
    void slotDisconnected();
};

#endif // TCPCLIENT_H
