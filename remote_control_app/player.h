#ifndef PLAYER_H
#define PLAYER_H

#include <vlc/vlc.h>
#ifdef Q_WS_X11
#include <QX11EmbedContainer>
#endif
#include <QFrame>

class QTimer;
class VolumeSlider;

class Player : public QFrame
{
    Q_OBJECT

    VolumeSlider *volumeSlider;
    // on Linux/Ubuntu Lucid and VLC >= 1.0 (at least 1.1.5 was tested) XWindow
    // handle have to be passed therefore QX11EmbedContainer have to be used
    // instead of QFrame
#ifdef Q_WS_X11
    QX11EmbedContainer *videoWidget;
#else
    // If KDE is used like unique desktop environment, only use QFrame *videoWidget;
    QFrame *videoWidget;
#endif
    QTimer *poller;
    bool isPlaying;
    libvlc_instance_t *vlcInstance;
    libvlc_media_player_t *mp;
    libvlc_media_t *m;

public:
    explicit Player(QWidget *parent = 0);
    Player(const Player &) = delete;
    Player &operator=(const Player &) = delete;
    ~Player();
    void showVolumeSlider(bool show);
    void showVideoWidget(bool show);
    void restart();

public slots:
    void playFile(const char *file);
    void updateInterface();
    void changeVolume(int newVolume);
};

#endif // PLAYER_H
