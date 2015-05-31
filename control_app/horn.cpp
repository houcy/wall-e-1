#include "horn.h"
#include "log.h"

#define HORN_PROGRAM "mplayer"
#define HORN_PROGRAM_ARG "-loop 0 -af volume=10 -slave /home/horn_signal.wav"
#define CMD_PLAY "seek 0 2\n"
#define CMD_PAUSE "pause\n"

Horn::Horn(QObject *parent) : QObject(parent)
{
    hornProcess = new QProcess(this);

    connect(hornProcess, SIGNAL(error(QProcess::ProcessError)), this,
        SLOT(hornProcessError(QProcess::ProcessError)));
    connect(hornProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this,
        SLOT(hornProcessFinished(int, QProcess::ExitStatus)));
    connect(hornProcess, SIGNAL(started()), this, SLOT(hornProcessStarted()));

    isPlaying = false;

    hornProcess->start(HORN_PROGRAM,
        QStringList(QString(HORN_PROGRAM_ARG).split(" ")));

    if (hornProcess->write(CMD_PAUSE) == -1)
        warning("Horn process failed to pause\n");
}

Horn::~Horn()
{
    hornProcess->terminate();
}

void Horn::signalStart()
{
    if (isPlaying)
        return;

    isPlaying = true;

    if (hornProcess->write(CMD_PLAY) == -1)
        warning("Horn process failed to play\n");
}

void Horn::signalStop()
{
    if (!isPlaying)
        return;

    isPlaying = false;

    if (hornProcess->write(CMD_PAUSE) == -1)
        warning("Horn process failed to pause\n");
}

void Horn::hornProcessError(QProcess::ProcessError error)
{
    debug("Horn process error %d\n", error);
}

void Horn::hornProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    debug("Horn process finished with exit code %d and exit status %d\n",
        exitCode, exitStatus);
}

void Horn::hornProcessStarted()
{
    debug("Horn process has been started\n");
}
