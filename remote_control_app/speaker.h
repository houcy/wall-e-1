#ifndef SPEAKER_H
#define SPEAKER_H

#include <vlc/vlc.h>
#include <QObject>

class Speaker : public QObject
{
    Q_OBJECT

    libvlc_instance_t *vlcInstance;
    const char *mediaName = "Voice stream";
    const char *url = "alsa://plughw";

public:
    explicit Speaker(QObject *parent = 0);
    ~Speaker();

    void startStream(const char *ip, const char *port);
    void stopStream();
};

#endif // SPEAKER_H
