#include "media_stream.h"
#include "log.h"
#include <QString>

#define MEDIA_STREAM_PROGRAM "gst-launch"
#define MEDIA_STREAM_PROGRAM_ARGS "v4l2src device=/dev/video1 ! videoscale !" \
    "video/x-raw-yuv,width=160,height=120 ! ffmpegcolorspace ! jpegenc ! " \
    "multipartmux ! udpsink host=%1 port=%2 osssrc device=/dev/dsp1 ! " \
    "mulawenc ! rtppcmupay ! udpsink host=%3 port=%4"

MediaStream::MediaStream(QObject *parent) : QObject(parent)
{
    mediaStreamProcess = new QProcess(this);

    connect(mediaStreamProcess, SIGNAL(error(QProcess::ProcessError)), this,
        SLOT(mediaStreamProcessError(QProcess::ProcessError)));
    connect(mediaStreamProcess, SIGNAL(finished(int, QProcess::ExitStatus)),
        this, SLOT(mediaStreamProcessFinished(int, QProcess::ExitStatus)));
    connect(mediaStreamProcess, SIGNAL(started()), this,
        SLOT(mediaStreamProcessStarted()));
}

void MediaStream::start(const QHostAddress &addr,  uint16_t audioPort,
    uint16_t videoPort)
{
    if (mediaStreamProcess->state() != QProcess::NotRunning)
        return;

    mediaStreamProcess->start(MEDIA_STREAM_PROGRAM,
        (QString(MEDIA_STREAM_PROGRAM_ARGS).arg(addr.toString()).arg(videoPort)
        .arg(addr.toString()).arg(audioPort)).split(" "));
}

void MediaStream::stop()
{
    if (mediaStreamProcess->state() == QProcess::NotRunning)
        return;

    mediaStreamProcess->terminate();
}

void MediaStream::mediaStreamProcessError(QProcess::ProcessError error)
{
    debug("Media stream process error %d\n", error);
}

void MediaStream::mediaStreamProcessFinished(int exitCode,
    QProcess::ExitStatus exitStatus)
{
    debug("Media stream process finished with exit code %d and exit status "
        "%d\n", exitCode, exitStatus);
}

void MediaStream::mediaStreamProcessStarted()
{
    debug("Media stream process has been started\n");
}
