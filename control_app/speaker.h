#ifndef SPEAKER_H
#define SPEAKER_H

#include <QObject>
#include <QHostAddress>
#include <QProcess>

class Speaker : public QObject
{
    Q_OBJECT

    QProcess *speakerStreamProcess;

public:
    explicit Speaker(QObject *parent = 0);

    Speaker(const Speaker &) = delete;
    Speaker &operator=(const Speaker &) = delete;
    void start(uint16_t port);
    void stop();

public slots:
    void speakerProcessError(QProcess::ProcessError error);
    void speakerProcessFinished(int exitCode,
        QProcess::ExitStatus exitStatus);
    void speakerProcessStarted();
};

#endif // SPEAKER_H
