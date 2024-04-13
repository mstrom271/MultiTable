#pragma once

// #include <QMediaPlayer>
#include <QSoundEffect>

class Sound {
    QSoundEffect *clickSound = nullptr;
    QSoundEffect *switchingSound = nullptr;
    // QMediaPlayer *bgMusicPL = nullptr;

    Sound();
    Sound(const Sound &) = delete;
    Sound &operator=(const Sound &) = delete;

  public:
    ~Sound();
    static Sound &instance();

    static void click();
    static void switching();
    static void bgMusicPlay();
    static void bgMusicPlayNext();
    static void bgMusicPause();
    static void bgMusicStop();
};
