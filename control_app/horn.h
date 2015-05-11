#ifndef HORN_H
#define HORN_H

#include <QObject>
#include <memory>
#include <QProcess>

class QProcess;

class Horn : public QObject
{
    Q_OBJECT

    QProcess *hornProcess;
    bool isPlaying;

public:
    explicit Horn(QObject *parent = 0);
    ~Horn();
    Horn(const Horn &) = delete;
    Horn &operator=(const Horn &) = delete;

    void signalStart();
    void signalStop();

signals:

public slots:
    void hornProcessError(QProcess::ProcessError error);
    void hornProcessFinished(int exitCode,
        QProcess::ExitStatus exitStatus);
    void hornProcessStarted();
};

#endif // HORN_H
