#include "player.h"
#include "volume_slider.h"

#ifdef Q_WS_X11
#include <QX11EmbedContainer>
#endif
#include <QVBoxLayout>
#include <QTimer>
#include <QFrame>

Player::Player(QWidget *parent) : QFrame(parent)
{
    const char * const vlcArgs[] =
    {
        "-I", "dummy", // Don't use any interface
        "--ignore-config", // Don't use VLC's config
        "--extraintf=logger", // Log anything
        "--verbose=1", // 1 - Verbose is normal
        "--network-caching=100", // Set low network caching
    };

#ifdef Q_WS_X11
    videoWidget = new QX11EmbedContainer(this);
#else
    // If KDE is used like unique desktop environment use QFrame
    videoWidget = new QFrame(this);
#endif

    volumeSlider = new VolumeSlider(this);
    volumeSlider->setFixedWidth(200);
    volumeSlider->setToolTip(tr("Volume"));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(videoWidget);
    layout->addWidget(volumeSlider);
    setLayout(layout);

    isPlaying = false;
    poller = new QTimer(this);

    // Create a new libvlc instance
    vlcInstance = libvlc_new(sizeof(vlcArgs) / sizeof(vlcArgs[0]), vlcArgs);
    if (!vlcInstance)
        qFatal("Failed to create VLC instance");

    // Create a media player playing environement
    mp = libvlc_media_player_new(vlcInstance);
    if (!mp)
        qFatal("Failed to create VLC media player object");

    connect(poller, SIGNAL(timeout()), this, SLOT(updateInterface()));
    connect(volumeSlider, SIGNAL(sliderMoved(int)), this,
        SLOT(changeVolume(int)));

    // Start timer to trigger every 100 ms the updateInterface slot
    poller->start(1000);
}

Player::~Player()
{
    libvlc_media_player_stop(mp);
    libvlc_media_player_release(mp);
    libvlc_release(vlcInstance);
}

void Player::playFile(const char *file)
{
    /* The file has to be in the following format:
     * udp:[[<source address>]@[<bind address>][:<bind port>]]
     */

    // Create a new LibVLC media descriptor
    m = libvlc_media_new_location(vlcInstance, file);
    if (!m)
        qFatal("Failed to create a VLC media");

    libvlc_media_player_set_media(mp, m);

    /* Note:
     *
     * passing the widget to the lib shows vlc at which position it should
     * show up vlc automatically resizes the video to the given size of the
     * widget and it even resizes it, if the size changes at the playing
     */

    // Get media instance to use our window
#if defined(Q_OS_WIN)
    libvlc_media_player_set_hwnd(mp, (void *)videoWidget->winId());
#elif defined(Q_OS_MAC)
    libvlc_media_player_set_drawable(mp, videoWidget->winId());
#else // Linux
    /* Note:
     * X11 handle on Linux is needed,
     * winID() returns X11 handle when QX11EmbedContainer us used
     */
    int windId = videoWidget->winId();
    libvlc_media_player_set_xwindow(mp, windId);
#endif

    if (libvlc_media_player_play(mp) == -1)
        qFatal("VLC failed to start play");
    isPlaying = true;
}

void Player::changeVolume(int newVolume)
{
    libvlc_audio_set_volume(mp, newVolume);
}

void Player::updateInterface()
{
    if (!isPlaying)
        return;

    // It's possible that the vlc doesn't play anything so check before
    libvlc_media_t *curMedia = libvlc_media_player_get_media(mp);
    if (!curMedia)
        return;

    int volume = libvlc_audio_get_volume(mp);
    volumeSlider->setValue(volume);
}

void Player::showVolumeSlider(bool show)
{
    volumeSlider->setVisible(show);
}

void Player::showVideoWidget(bool show)
{
    videoWidget->setVisible(show);
}

void Player::restart()
{
    libvlc_media_player_stop(mp);
    libvlc_media_player_play(mp);
}

