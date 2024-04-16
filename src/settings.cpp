#include "settings.h"
#include "config.h"
#include <QApplication>
#include <QScreen>
#include <QVector>
#include <QWidget>

Settings::Settings() {
    upgradeOldSettings();

    settings.setValue("/Version", PROJECT_VERSION);

    FirstRun = settings.value("/FirstRun", true).toBool();
    if (FirstRun)
        settings.setValue("/FirstRun", false);

    Language = settings.value("/Language", "System").toString();
    Theme = settings.value("/Theme", "System").toString();

    Stats = settings.value("/Stats", "").toString();
    SoundState = settings.value("/SoundState", false).toBool();
    IntelMode = settings.value("/IntelMode", true).toBool();

    constexpr int table_side = 12;
    TableSide_From =
        std::clamp(settings.value("/TableSide_From", 2).toInt(), 2, table_side);
    TableSide_To =
        std::clamp(settings.value("/TableSide_To", 6).toInt(), 2, table_side);
    ExerciseTimer = settings.value("/ExerciseTimer", 3).toInt();

    // Temporary settings
    Font1 = Font2 = Font3 = Font4 = QApplication::font();
#ifdef Q_OS_ANDROID
    Font1.setPixelSize(QApplication::primaryScreen()->physicalDotsPerInch() *
                       0.4);
    Font2.setPixelSize(QApplication::primaryScreen()->physicalDotsPerInch() *
                       0.25);
    Font3.setPixelSize(QApplication::primaryScreen()->physicalDotsPerInch() *
                       0.14);
    Font4.setPixelSize(QApplication::primaryScreen()->physicalDotsPerInch() *
                       0.09);
    LogicalDPI = QApplication::primaryScreen()->physicalDotsPerInch() * 1.0;
#else
    Font1.setPixelSize(QApplication::primaryScreen()->physicalDotsPerInch() *
                       0.45);
    Font2.setPixelSize(QApplication::primaryScreen()->physicalDotsPerInch() *
                       0.30);
    Font3.setPixelSize(QApplication::primaryScreen()->physicalDotsPerInch() *
                       0.20);
    Font4.setPixelSize(QApplication::primaryScreen()->physicalDotsPerInch() *
                       0.13);
    LogicalDPI = QApplication::primaryScreen()->physicalDotsPerInch() * 1.5;
#endif
}

Settings &Settings::instance() {
    static Settings instance;
    return instance;
}

void Settings::upgradeOldSettings() {
    // evaluate from 0.1 to 0.2
    if (settings.contains("/firsthelp") && !settings.contains("/version")) {
        // remove obsolete keys
        settings.remove("/firsthelp");
        settings.remove("/stats");

        // create /firstrun
        settings.setValue("/firstrun", false);

        // create /version
        settings.setValue("/version", "0.2");
    }

    // evaluate from 0.2 to 0.3
    if (settings.value("/version", "") == "0.2") {
        // pairs to convert {old_name, new_name}
        QVector<QPair<QString, QString>> names = {
            // clang-format off
            {"/exerciseTimer",      "/ExerciseTimer"},
            {"/firstrun",           "/FirstRun"},
            {"/intelMode",          "/IntelMode"},
            {"/language",           "/Language"},
            {"/clickSound",         "/SoundState"},
            {"/music",              "/MusicState"},
            // {"/stats",              "/Stats"},
            {"/tableSide_from",     "/TableSide_From"},
            {"/tableSide_to",       "/TableSide_To"},
            {"/theme",              "/Theme"},
            {"/version",            "/Version"},
            // clang-format on
        };
        // rename items
        for (auto item : names) {
            if (settings.contains(item.first)) {
                auto value = settings.value(item.first);

                // remove before set, due to case-insensitive windows approach
                settings.remove(item.first);
                settings.setValue(item.second, value);
            }
        }

        // update /version
        settings.setValue("/Version", "0.3");
    }
}

bool Settings::getFirstRun() { return instance().FirstRun; }

QString Settings::getLanguage() { return instance().Language; }
void Settings::setLanguage(QString newLanguage) {
    instance().settings.setValue("/Language", newLanguage);
    instance().Language = newLanguage;
};

QString Settings::getTheme() { return instance().Theme; }
void Settings::setTheme(QString newTheme) {
    instance().settings.setValue("/Theme", newTheme);
    instance().Theme = newTheme;
};

QString Settings::getStats() { return instance().Stats; }
void Settings::setStats(QString newStats) {
    instance().settings.setValue("/Stats", newStats);
    instance().Stats = newStats;
};

bool Settings::getSoundState() { return instance().SoundState; }
void Settings::setSoundState(bool newSoundState) {
    instance().settings.setValue("/SoundState", newSoundState);
    instance().SoundState = newSoundState;
};

bool Settings::getMusicState() { return instance().MusicState; }
void Settings::setMusicState(bool newMusicState) {
    instance().settings.setValue("/MusicState", newMusicState);
    instance().MusicState = newMusicState;
};

bool Settings::getIntelMode() { return instance().IntelMode; }
void Settings::setIntelMode(bool newIntelMode) {
    instance().settings.setValue("/IntelMode", newIntelMode);
    instance().IntelMode = newIntelMode;
};

int Settings::getTableSide_From() { return instance().TableSide_From; }
void Settings::setTableSide_From(int newTableSide_From) {
    instance().settings.setValue("/TableSide_From", newTableSide_From);
    instance().TableSide_From = newTableSide_From;
};

int Settings::getTableSide_To() { return instance().TableSide_To; }
void Settings::setTableSide_To(int newTableSide_To) {
    instance().settings.setValue("/TableSide_To", newTableSide_To);
    instance().TableSide_To = newTableSide_To;
};

int Settings::getExerciseTimer() { return instance().ExerciseTimer; }
void Settings::setExerciseTimer(int newExerciseTimer) {
    instance().settings.setValue("/ExerciseTimer", newExerciseTimer);
    instance().ExerciseTimer = newExerciseTimer;
};

// Temporary settings
QSize Settings::getWindowSize() {
    static QWidget *mainWgt = nullptr;
    if (!mainWgt) {
        const QWidgetList topLevelWidgets = QApplication::topLevelWidgets();
        for (auto wgt : topLevelWidgets)
            if (wgt->objectName() == "MainWidget") {
                mainWgt = wgt;
                break;
            }
    }

    if (mainWgt)
        return mainWgt->size();
    else
        return QSize(600, 800);
}

QFont Settings::getFont1() { return instance().Font1; }
void Settings::setFont1(QFont newFont1) { instance().Font1 = newFont1; };
QFont Settings::getFont2() { return instance().Font2; }
void Settings::setFont2(QFont newFont2) { instance().Font2 = newFont2; };
QFont Settings::getFont3() { return instance().Font3; }
void Settings::setFont3(QFont newFont3) { instance().Font3 = newFont3; };
QFont Settings::getFont4() { return instance().Font4; }
void Settings::setFont4(QFont newFont4) { instance().Font4 = newFont4; };
double Settings::getLogicalDPI() { return instance().LogicalDPI; }
void Settings::setLogicalDPI(double newLogicalDPI) {
    instance().LogicalDPI = newLogicalDPI;
};
