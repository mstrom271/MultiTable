#include "sound.h"
#include "settings.h"
#include <QApplication>
// #include <QMediaPlaylist>

Sound::Sound() {
    clickSound = new QSoundEffect;
    switchingSound = new QSoundEffect;

    clickSound->setSource(QUrl("qrc:/res/Sounds/clickSound.wav"));
    // switchingSound->setMedia(QUrl("qrc:/res/Sounds/switchingSound.mp3"));
    switchingSound->setSource(QUrl("qrc:/res/Sounds/clickSound.wav"));

    // QMediaPlaylist *playlist = new QMediaPlaylist;
    // playlist->addMedia(QUrl("qrc:/res/Sounds/bg_music1.mp3"));
    // playlist->addMedia(QUrl("qrc:/res/Sounds/bg_music2.mp3"));
    // playlist->addMedia(QUrl("qrc:/res/Sounds/bg_music3.mp3"));
    // playlist->addMedia(QUrl("qrc:/res/Sounds/bg_music4.mp3"));
    // playlist->addMedia(QUrl("qrc:/res/Sounds/bg_music5.mp3"));
    // playlist->addMedia(QUrl("qrc:/res/Sounds/bg_music6.mp3"));
    // playlist->addMedia(QUrl("qrc:/res/Sounds/bg_music7.mp3"));

    // bgMusicPL = new QMediaPlayer;
    // bgMusicPL->setVolume(20);
    // bgMusicPL->setPlaylist(playlist);
    // bgMusicPL->playlist()->setPlaybackMode(QMediaPlaylist::Random);
    // bgMusicPL->playlist()->next();
}

Sound::~Sound() {
    delete clickSound;
    delete switchingSound;
    // delete bgMusicPL;
}

Sound &Sound::instance() {
    static Sound instance;
    return instance;
}

void Sound::click() {
    if (Settings::getSoundState()) {
        instance().clickSound->stop();
        instance().clickSound->play();
    }
}

void Sound::switching() {
    if (Settings::getSoundState()) {
        instance().switchingSound->stop();
        instance().switchingSound->play();
    }
}

void Sound::bgMusicPlay() {
    if (Settings::getMusicState()) {
        // bgMusicPL->play();
    }
}

void Sound::bgMusicPlayNext() {
    if (Settings::getMusicState()) {
        // bgMusicPL->playlist()->next();
        // bgMusicPL->play();
    }
}

void Sound::bgMusicPause() { /* bgMusicPL->pause(); */
}

void Sound::bgMusicStop() { /* bgMusicPL->stop(); */
}
