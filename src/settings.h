#pragma once

#include <QFont>
#include <QMap>
#include <QSettings>
#include <QVector>

class Settings {
    QSettings settings;

    bool FirstRun;
    QString Language;
    QString Theme;
    QString Stats;
    bool SoundState;
    bool MusicState;
    bool IntelMode;
    int TableSide_From, TableSide_To;
    int ExerciseTimer; // min, 0 - infinity

    // Temporary settings
    QFont Font1, Font2, Font3, Font4;
    double LogicalDPI;

    Settings();
    Settings(const Settings &) = delete;
    Settings &operator=(const Settings &) = delete;
    void upgradeOldSettings();

  public:
    static Settings &instance();

    static bool getFirstRun();

    static QString getLanguage();
    static void setLanguage(QString newLanguage);

    static QString getTheme();
    static void setTheme(QString newTheme);

    static QString getStats();
    static void setStats(QString);

    static bool getSoundState();
    static void setSoundState(bool newSoundState);

    static bool getMusicState();
    static void setMusicState(bool newMusicState);

    static bool getIntelMode();
    static void setIntelMode(bool newIntelMode);

    static int getTableSide_From();
    static void setTableSide_From(int newTableSide_From);

    static int getTableSide_To();
    static void setTableSide_To(int newTableSide_To);

    static int getExerciseTimer();
    static void setExerciseTimer(int newExerciseTimer);

    // Temporary settings
    static QSize getWindowSize();

    static QFont getFont1();
    static void setFont1(QFont newFont1);
    static QFont getFont2();
    static void setFont2(QFont newFont2);
    static QFont getFont3();
    static void setFont3(QFont newFont3);
    static QFont getFont4();
    static void setFont4(QFont newFont4);

    static double getLogicalDPI();
    static void setLogicalDPI(double newLogicalDPI);
};