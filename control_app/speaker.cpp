#include "speaker.h"
#include "log.h"
#include <QString>

#define SPEAKER_STREAM_PROGRAM "gst-launch"

#define SPEAKER_STREAM_PROGRAM_ARGS "udpsrc port=%1 " \
    "caps=\"application/x-rtp\" ! queue ! rtppcmudepay ! mulawdec ! " \
    "audioconvert ! osssink "

Speaker::Speaker(QObject *parent) : QObject(parent)
{
    speakerStreamProcess = new QProcess(this);

    connect(speakerStreamProcess, SIGNAL(error(QProcess::ProcessError)), this,
        SLOT(speakerProcessError(QProcess::ProcessError)));
    connect(speakerStreamProcess, SIGNAL(finished(int, QProcess::ExitStatus)),
        this, SLOT(speakerProcessFinished(int, QProcess::ExitStatus)));
    connect(speakerStreamProcess, SIGNAL(started()), this,
        SLOT(speakerProcessStarted()));
}

void Speaker::start(uint16_t port)
{
    if (speakerStreamProcess->state() != QProcess::NotRunning)
        return;

    speakerStreamProcess->start(SPEAKER_STREAM_PROGRAM,
        (QString(SPEAKER_STREAM_PROGRAM_ARGS).arg(port)).split(" "));
}

void Speaker::stop()
{
    if (speakerStreamProcess->state() == QProcess::NotRunning)
        return;

    speakerStreamProcess->terminate();
}

void Speaker::speakerProcessError(QProcess::ProcessError error)
{
    debug("Speaker process error %d\n", error);
}

void Speaker::speakerProcessFinished(int exitCode,
    QProcess::ExitStatus exitStatus)
{
    debug("Speaker process finished with exit code %d and exit status "
        "%d\n", exitCode, exitStatus);
}

void Speaker::speakerProcessStarted()
{
    debug("Speaker process has been started\n");
}
