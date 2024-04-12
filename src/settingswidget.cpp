#include "settingswidget.h"
#include "language.h"
#include "settings.h"
#include "sound.h"
#include "theme.h"
#include <QLayout>
#include <QPushButton>
#include <QScrollBar>
#include <QScroller>
#include <QStyledItemDelegate>

SettingsWidget::SettingsWidget(QWidget *wgt) : QWidget(wgt) {
    vLayout = new QVBoxLayout;
    scrollArea = new QScrollArea;
    scrollArea->setFrameStyle(QFrame::NoFrame);
    scrollArea->setWidgetResizable(true);
    scrollArea->setAttribute(Qt::WA_TranslucentBackground);
#ifdef Q_OS_ANDROID
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QScroller::grabGesture(scrollArea, QScroller::TouchGesture);
#endif
    scrollWidget = new QWidget;
    scrollWidget->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout = new QGridLayout;
    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 1);
    gridLayout->setContentsMargins(
        Settings::getLogicalDPI() * 0.1, Settings::getLogicalDPI() * 0.1,
        Settings::getLogicalDPI() * 0.1, Settings::getLogicalDPI() * 0.1);
    int level = 0;
    lbl_caption = new QLabel;
    lbl_caption->setWordWrap(true);
    lbl_caption->setAttribute(Qt::WA_TranslucentBackground);
    lbl_caption->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    gridLayout->addWidget(lbl_caption, level, 0, 1, 2);
    level += 1;

    lbl_language = new QLabel;
    lbl_language->setWordWrap(true);
    lbl_language->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_language, level, 0,
                          Qt::AlignBottom | Qt::AlignLeft);
    lbl_language_detailed = new QLabel;
    lbl_language_detailed->setWordWrap(true);
    lbl_language_detailed->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_language_detailed, level + 1, 0,
                          Qt::AlignTop | Qt::AlignLeft);

    cmb_language = new QComboBox;
    cmb_language->addItems(Language::getLanguageList());
    cmb_language->setCurrentText(Settings::getLanguage());
    cmb_language->setMinimumContentsLength(4);
    cmb_language->setItemDelegate(
        new QStyledItemDelegate()); // to force stylesheets to work
    QObject::connect(cmb_language, SIGNAL(textActivated(QString)),
                     SLOT(languageChange(QString)));
    cmb_language->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(cmb_language, level, 1, 2, 1,
                          Qt::AlignVCenter | Qt::AlignLeft);
    level += 2;

    lbl_clickSound = new QLabel;
    lbl_clickSound->setWordWrap(true);
    lbl_clickSound->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_clickSound, level, 0,
                          Qt::AlignBottom | Qt::AlignLeft);
    lbl_clickSound_detailed = new QLabel;
    lbl_clickSound_detailed->setWordWrap(true);
    lbl_clickSound_detailed->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_clickSound_detailed, level + 1, 0,
                          Qt::AlignTop | Qt::AlignLeft);

    ckb_clickSound = new QCheckBox;
    ckb_clickSound->setCheckState(Settings::getSoundState()
                                      ? Qt::CheckState::Checked
                                      : Qt::CheckState::Unchecked);
    ckb_clickSound->setAttribute(Qt::WA_TranslucentBackground);
    QObject::connect(ckb_clickSound, SIGNAL(stateChanged(int)),
                     SLOT(clickSoundChange(int)));
    gridLayout->addWidget(ckb_clickSound, level, 1, 2, 1,
                          Qt::AlignVCenter | Qt::AlignLeft);
    level += 2;

    lbl_music = new QLabel;
    lbl_music->setWordWrap(true);
    lbl_music->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_music, level, 0, Qt::AlignBottom | Qt::AlignLeft);
    lbl_music_detailed = new QLabel;
    lbl_music_detailed->setWordWrap(true);
    lbl_music_detailed->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_music_detailed, level + 1, 0,
                          Qt::AlignTop | Qt::AlignLeft);

    ckb_music = new QCheckBox;
    ckb_music->setCheckState(Settings::getMusicState()
                                 ? Qt::CheckState::Checked
                                 : Qt::CheckState::Unchecked);
    ckb_music->setAttribute(Qt::WA_TranslucentBackground);
    QObject::connect(ckb_music, SIGNAL(stateChanged(int)),
                     SLOT(musicChange(int)));
    gridLayout->addWidget(ckb_music, level, 1, 2, 1,
                          Qt::AlignVCenter | Qt::AlignLeft);
    level += 2;

    lbl_intelMode = new QLabel;
    lbl_intelMode->setWordWrap(true);
    lbl_intelMode->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_intelMode, level, 0,
                          Qt::AlignBottom | Qt::AlignLeft);
    lbl_intelMode_detailed = new QLabel;
    lbl_intelMode_detailed->setWordWrap(true);
    lbl_intelMode_detailed->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_intelMode_detailed, level + 1, 0,
                          Qt::AlignTop | Qt::AlignLeft);

    ckb_intelMode = new QCheckBox;
    ckb_intelMode->setCheckState(Settings::getIntelMode()
                                     ? Qt::CheckState::Checked
                                     : Qt::CheckState::Unchecked);
    ckb_intelMode->setAttribute(Qt::WA_TranslucentBackground);
    QObject::connect(ckb_intelMode, SIGNAL(stateChanged(int)),
                     SLOT(intelModeChange(int)));
    gridLayout->addWidget(ckb_intelMode, level, 1, 2, 1,
                          Qt::AlignVCenter | Qt::AlignLeft);
    level += 2;

    lbl_timer = new QLabel;
    lbl_timer->setWordWrap(true);
    lbl_timer->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_timer, level, 0, Qt::AlignBottom | Qt::AlignLeft);
    lbl_timer_detailed = new QLabel;
    lbl_timer_detailed->setWordWrap(true);
    lbl_timer_detailed->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_timer_detailed, level + 1, 0,
                          Qt::AlignTop | Qt::AlignLeft);

    cmb_timer = new QComboBox;
    cmb_timer->addItems(
        QStringList({"0", "1", "2", "3", "5", "7", "10", "15", "20", "30"}));
    cmb_timer->setCurrentText(QString::number(Settings::getExerciseTimer()));
    cmb_timer->setMinimumContentsLength(4);
    cmb_timer->setItemDelegate(
        new QStyledItemDelegate()); // to force stylesheets to work
    cmb_timer->setAttribute(Qt::WA_TranslucentBackground);
    QObject::connect(cmb_timer, SIGNAL(textActivated(QString)),
                     SLOT(timerChange(QString)));
    gridLayout->addWidget(cmb_timer, level, 1, 2, 1,
                          Qt::AlignVCenter | Qt::AlignLeft);
    level += 2;

    lbl_theme = new QLabel;
    lbl_theme->setWordWrap(true);
    lbl_theme->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_theme, level, 0, Qt::AlignBottom | Qt::AlignLeft);
    lbl_theme_detailed = new QLabel;
    lbl_theme_detailed->setWordWrap(true);
    lbl_theme_detailed->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_theme_detailed, level + 1, 0,
                          Qt::AlignTop | Qt::AlignLeft);

    cmb_theme = new QComboBox;
    cmb_theme->addItems(Theme::getThemeList());
    cmb_theme->setCurrentText(Theme::getEffectiveTheme());
    cmb_theme->setMinimumContentsLength(10);
    cmb_theme->setItemDelegate(
        new QStyledItemDelegate()); // to force stylesheets to work
    QObject::connect(cmb_theme, SIGNAL(textActivated(QString)),
                     SLOT(themeChange(QString)));
    cmb_theme->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(cmb_theme, level, 1, 2, 1,
                          Qt::AlignVCenter | Qt::AlignLeft);
    level += 2;
    scrollWidget->setLayout(gridLayout);
    scrollWidget->setMinimumHeight(Settings::getLogicalDPI() * 0.5 * level);
    scrollArea->setWidget(scrollWidget);
    vLayout->addWidget(scrollArea, 17);

    okBtn = new QPushButton;
    okBtn->setFont(Settings::getFont3());
    connect(okBtn, SIGNAL(clicked()), SIGNAL(ok()));
    vLayout->addWidget(okBtn, 1);
    setLayout(vLayout);
}

void SettingsWidget::onThemeChange() {
#ifdef Q_OS_ANDROID
    QColor bgColor;
    QColor inversebgColor;
    if (Theme::getEffectiveTheme() == "DarkTheme") {
        bgColor = Qt::black;
        inversebgColor = Qt::white;
    } else if (Theme::getEffectiveTheme() == "GreyTheme") {
        bgColor = QColor("#777777");
        inversebgColor = Qt::black;
    } else {
        bgColor = QColor("#EEEEEE");
        inversebgColor = Qt::black;
    }

    QString ckbStyle =
        "QCheckBox {background-color: transparent; border: 1px solid " +
        inversebgColor.name() +
        "; }"
        "QCheckBox::indicator {\
                                width: " +
        QString::number(std::round(Settings::getLogicalDPI() * 0.15)) + "px;\
                                height: " +
        QString::number(std::round(Settings::getLogicalDPI() * 0.15)) + "px;\
                           }";
    ckb_clickSound->setStyleSheet(ckbStyle);
    ckb_music->setStyleSheet(ckbStyle);
    ckb_intelMode->setStyleSheet(ckbStyle);

    QString cmbStyle =
        "QComboBox QAbstractItemView::item { min-height: " +
        QString::number(std::round(Settings::getLogicalDPI() * 0.50)) + "px;}" +
        "QComboBox {background-color: " + bgColor.name() + ";}";
    cmb_language->setStyleSheet(cmbStyle);
    cmb_timer->setStyleSheet(cmbStyle);
    cmb_theme->setStyleSheet(cmbStyle);
#endif

    cmb_theme->setCurrentText(Settings::getTheme());
}

void SettingsWidget::onLanguageChange() {
    lbl_caption->setText(tr("Settings"));

    lbl_language->setText(tr("Language"));
    lbl_language_detailed->setText(
        tr("To choose a language of the application"));

    lbl_clickSound->setText(tr("Sound"));
    lbl_clickSound_detailed->setText(tr("Sound of button clicks"));

    lbl_music->setText(tr("Music"));
    lbl_music_detailed->setText(tr("Background music"));

    lbl_intelMode->setText(tr("Intelligent Mode"));
    lbl_intelMode_detailed->setText(
        tr("Algorithm will select poorly studied combination of numbers"));

    lbl_timer->setText(tr("Exercise time"));
    lbl_timer_detailed->setText(tr("In minutes. 0 is infinity"));

    lbl_theme->setText(tr("Themes"));
    lbl_theme_detailed->setText(tr("Color of interface elements"));

    okBtn->setText(tr("Ok"));

    cmb_language->setCurrentText(Settings::getLanguage());
}

bool SettingsWidget::event(QEvent *event) {
    if (event->type() == ThemeChangeEvent::type) {
        onThemeChange();
        return true;
    } else if (event->type() == LanguageChangeEvent::type) {
        onLanguageChange();
        return true;
    }

    return QWidget::event(event);
}

void SettingsWidget::resizeEvent(QResizeEvent *event) {
    lbl_caption->setFont(Settings::getFont2());

    lbl_language->setFont(Settings::getFont3());
    lbl_language_detailed->setFont(Settings::getFont4());
    cmb_language->setFont(Settings::getFont3());

    lbl_clickSound->setFont(Settings::getFont3());
    lbl_clickSound_detailed->setFont(Settings::getFont4());
    ckb_clickSound->setFont(Settings::getFont3());

    lbl_music->setFont(Settings::getFont3());
    lbl_music_detailed->setFont(Settings::getFont4());
    ckb_music->setFont(Settings::getFont3());

    lbl_intelMode->setFont(Settings::getFont3());
    lbl_intelMode_detailed->setFont(Settings::getFont4());
    ckb_intelMode->setFont(Settings::getFont3());

    lbl_timer->setFont(Settings::getFont3());
    lbl_timer_detailed->setFont(Settings::getFont4());
    cmb_timer->setFont(Settings::getFont3());

    lbl_theme->setFont(Settings::getFont3());
    lbl_theme_detailed->setFont(Settings::getFont4());
    cmb_theme->setFont(Settings::getFont3());

    okBtn->setMinimumHeight(Settings::getLogicalDPI() * 0.15);
    okBtn->setMaximumHeight(Settings::getLogicalDPI() * 0.25);

    QWidget::resizeEvent(event);
}

void SettingsWidget::showEvent(QShowEvent *event) {
    okBtn->setFocus();
    scrollArea->verticalScrollBar()->setValue(0);

    QWidget::showEvent(event);
}

void SettingsWidget::languageChange(QString s) {
    Settings::setLanguage(s);
    Language::applyLanguage();
}

void SettingsWidget::clickSoundChange(int state) {
    if (state == Qt::Checked)
        Settings::setSoundState(true);
    else if (state == Qt::Unchecked)
        Settings::setSoundState(false);
}

void SettingsWidget::musicChange(int state) {
    if (state == Qt::Checked)
        Settings::setMusicState(true);
    else if (state == Qt::Unchecked)
        Settings::setMusicState(false);
}

void SettingsWidget::intelModeChange(int state) {
    if (state == Qt::Checked)
        Settings::setIntelMode(true);
    else if (state == Qt::Unchecked)
        Settings::setIntelMode(false);
}

void SettingsWidget::timerChange(QString s) {
    Settings::setExerciseTimer(s.toInt());
}

void SettingsWidget::themeChange(QString s) {
    Settings::setTheme(s);
    Theme::applyTheme();
}
