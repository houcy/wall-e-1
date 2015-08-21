#include "speaker.h"

Speaker::Speaker(QObject *parent) : QObject(parent)
{
    vlcInstance = libvlc_new(0, nullptr);
    if (!vlcInstance)
        qCritical("Failed to create VLC instance");
}

Speaker::~Speaker()
{
    libvlc_vlm_release(vlcInstance);
}

void Speaker::startStream(const char *ip, const char *port)
{
    QString sOut(QString("#transcode{acodec=ulaw,channels=1,samplerate=8000}" \
        ":duplicate{dst=rtp{dst=%1,port-audio=%2}}").arg(ip).arg(port));

    if (libvlc_vlm_add_broadcast(vlcInstance, mediaName, url,
        sOut.toLatin1().constData(), 0, NULL, true, false))
    {
        qCritical("Failed to initialize voice stream");
    }
    if (libvlc_vlm_play_media(vlcInstance, mediaName))
        qCritical("Failed to start voice stream");
}

void Speaker::stopStream()
{
    if (libvlc_vlm_stop_media(vlcInstance, mediaName))
        qCritical("Failed to stop voice stream");
}
