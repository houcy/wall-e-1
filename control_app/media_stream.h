#ifndef MEDIA_STREAM_H
#define MEDIA_STREAM_H

#include <QObject>
#include <QHostAddress>
#include <QProcess>

class MediaStream : public QObject
{
    Q_OBJECT

    QProcess *mediaStreamProcess;

public:
    explicit MediaStream(QObject *parent = 0);
    MediaStream(const MediaStream &) = delete;
    MediaStream &operator=(const MediaStream &) = delete;
    void start(const QHostAddress &addr, uint16_t audioPort,
        uint16_t videoPort);
    void stop();

signals:

public slots:
    void mediaStreamProcessError(QProcess::ProcessError error);
    void mediaStreamProcessFinished(int exitCode,
        QProcess::ExitStatus exitStatus);
    void mediaStreamProcessStarted();
};

#endif // MEDIA_STREAM_H
