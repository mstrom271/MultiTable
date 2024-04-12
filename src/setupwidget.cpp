#include "setupwidget.h"
#include "language.h"
#include "settings.h"
#include "sound.h"
#include "stats.h"
#include "theme.h"
#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>
#include <QStyle>

SetupWidget::SetupWidget(QWidget *wgt) : QWidget(wgt) {
    vLayout = new QVBoxLayout;
    hLayout1 = new QHBoxLayout;
    deleteStatsBtn = new QPushButton;
    connect(deleteStatsBtn, SIGNAL(clicked()), SLOT(deleteStats()));
    hLayout1->addWidget(deleteStatsBtn);

    settingsBtn = new QPushButton;
    connect(settingsBtn, SIGNAL(clicked()), SIGNAL(showSettings()));
    hLayout1->addWidget(settingsBtn);

    helpBtn = new QPushButton;
    connect(helpBtn, SIGNAL(clicked()), SIGNAL(showHelp()));
    hLayout1->addWidget(helpBtn);

    likeBtn = new QPushButton;
    connect(likeBtn, SIGNAL(clicked()), SLOT(likeApp()));
    hLayout1->addWidget(likeBtn);
    vLayout->addLayout(hLayout1);

    hLayout2 = new QHBoxLayout;
    QFontMetrics fm(Settings::getFont4());
    lblGreen = new QLabel;
    lblGreen->setFixedSize(fm.lineSpacing(), fm.lineSpacing());
    lblGreen->setStyleSheet("background-color: green");
    hLayout2->addWidget(lblGreen);

    lblTextGreen = new QLabel;
    lblTextGreen->setMaximumHeight(fm.lineSpacing());
    lblTextGreen->setFont(Settings::getFont4());
    lblTextGreen->setAttribute(Qt::WA_TranslucentBackground);
    hLayout2->addWidget(lblTextGreen);

    lblRed = new QLabel;
    lblRed->setFixedSize(fm.lineSpacing(), fm.lineSpacing());
    lblRed->setStyleSheet("background-color: red");
    hLayout2->addWidget(lblRed);

    lblTextRed = new QLabel;
    lblTextRed->setMaximumHeight(fm.lineSpacing());
    lblTextRed->setFont(Settings::getFont4());
    lblTextRed->setAttribute(Qt::WA_TranslucentBackground);
    hLayout2->addWidget(lblTextRed);
    vLayout->addLayout(hLayout2);

    tableWgt = new StatsWidget();
    vLayout->addWidget(tableWgt);

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spacer->setAttribute(Qt::WA_TranslucentBackground);
    vLayout->addWidget(spacer);

    startBtn = new QPushButton;
    startBtn->setFont(Settings::getFont3());
    connect(startBtn, SIGNAL(clicked()), SIGNAL(start()));
    vLayout->addWidget(startBtn);

    setLayout(vLayout);
}

void SetupWidget::deleteStats() {
    Sound::switching();
    MessageBox *dlg = new MessageBox();
    dlg->setText(msgDeleteAll);
    if (dlg->exec() == QDialog::DialogCode::Accepted) {
        Stats::fill();
        Stats::store();
        tableWgt->update();
    }
    Sound::switching();
}

void SetupWidget::likeApp() {
    Sound::switching();
    QDesktopServices::openUrl(QUrl("https://play.google.com/store/apps/"
                                   "details?id=org.tapmyapp.multitable_free"));
}

void SetupWidget::onThemeChange() {
    deleteStatsBtn->setIcon(
        QIcon(":/res/" + Theme::getEffectiveTheme() + "/delete_all_icon.png"));
    settingsBtn->setIcon(
        QIcon(":/res/" + Theme::getEffectiveTheme() + "/settings_icon.png"));
    helpBtn->setIcon(
        QIcon(":/res/" + Theme::getEffectiveTheme() + "/help_icon.png"));
    likeBtn->setIcon(
        QIcon(":/res/" + Theme::getEffectiveTheme() + "/like_icon.png"));
#ifdef FREE_VERSION
    proBtn->setIcon(
        QIcon(":/res/" + Theme::getEffectiveTheme() + "/pro_icon.png"));
#endif
}

void SetupWidget::onLanguageChange() {
    startBtn->setText(tr("Start"));

    lblTextGreen->setText(tr("Well learned pairs"));
    lblTextRed->setText(tr("Poorly learned pairs"));

    msgDeleteAll = tr("Delete all statistics?");
}

bool SetupWidget::event(QEvent *event) {
    if (event->type() == ThemeChangeEvent::type) {
        onThemeChange();
        return true;
    } else if (event->type() == LanguageChangeEvent::type) {
        onLanguageChange();
        return true;
    }

    return QWidget::event(event);
}

void SetupWidget::resizeEvent(QResizeEvent *event) {
    // Min and Max sizes of widgets
    for (auto wgt : std::initializer_list<QWidget *>{
             deleteStatsBtn, settingsBtn, helpBtn, likeBtn,
#ifdef FREE_VERSION
             proBtn,
#endif
             startBtn}) {
        wgt->setMinimumHeight(Settings::getLogicalDPI() * 0.15);
        wgt->setMaximumHeight(Settings::getLogicalDPI() * 0.25);
    }

    QWidget::resizeEvent(event);
}

void SetupWidget::showEvent(QShowEvent *event) {
    startBtn->setFocus();

    QWidget::showEvent(event);
}

void SetupWidget::paintEvent(QPaintEvent *event) {
    // icon sizes
    double iconScale = 0.5;
    for (auto btn : {
             deleteStatsBtn,
             settingsBtn,
             helpBtn,
             likeBtn,
#ifdef FREE_VERSION
             proBtn,
#endif
         }) {
        btn->setIconSize(QSize(btn->size().height() * iconScale,
                               btn->size().height() * iconScale));
    }

    QWidget::paintEvent(event);
}

void MessageBox::onThemeChange() {
    QString style;
    if (Theme::getEffectiveTheme() == "DarkTheme") {
        style += "QWidget {background-color: black; color: white;}";
        style += "QPushButton {\
                    border: 1px solid #6f6f71;\
                    border-radius: 10px;\
                    background-color: qlineargradient(x1: 0.5, y1: 1, x2: 0.5, y2: 0,\
                                                      stop: 0 #111111, stop: 1 #222222);\
                 }";
        style += "QDialog{border:1px solid gray}";
    } else if (Theme::getEffectiveTheme() == "GreyTheme") {
        style +=
            "QWidget {background-color: qlineargradient(x1: 0.5, y1: 1, x2: 0.5, y2: 0,\
                                                            stop: 0 #999999, stop: 1 #777777);\
                           color: black;}";
        style += "QPushButton {\
                    border: 1px solid #666666;\
                    border-radius: 10px;\
                    background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1,\
                                                      stop: 0 #AAAAAA, stop: 1 #BBBBBB);\
                 }";
        style += "QDialog{border:1px solid black}";
    } else if (Theme::getEffectiveTheme() == "LightTheme") {
        style += "QWidget {background-color: #EEEEEE; color: black;}";
        style += "QPushButton {\
                    border: 1px solid #AAAAAA;\
                    border-radius: 10px;\
                    background-color: qlineargradient(x1: 0.5, y1: 0, x2: 0.5, y2: 1,\
                                                      stop: 0 #CCCCCC, stop: 1 #DDDDDD);\
                 }";
        style += "QDialog{border:1px solid gray}";
    }

    setStyleSheet(style);
}

void MessageBox::onLanguageChange() {
    okBtn->setText(tr("Ok"));
    cancelBtn->setText(tr("Cancel"));
}

bool MessageBox::event(QEvent *event) {
    if (event->type() == ThemeChangeEvent::type) {
        onThemeChange();
        return true;
    } else if (event->type() == LanguageChangeEvent::type) {
        onLanguageChange();
        return true;
    }

    return QDialog::event(event);
}

void MessageBox::resizeEvent(QResizeEvent *event) {
#ifdef Q_OS_ANDROID
    setMinimumWidth(QApplication::primaryScreen()->size().width() * 0.8);
#endif

    okBtn->setMinimumHeight(Settings::getLogicalDPI() * 0.25);
    okBtn->setMaximumHeight(Settings::getLogicalDPI() * 0.30);

    cancelBtn->setMinimumHeight(Settings::getLogicalDPI() * 0.25);
    cancelBtn->setMaximumHeight(Settings::getLogicalDPI() * 0.30);

    QDialog::resizeEvent(event);
}

MessageBox::MessageBox(QWidget *wgt) : QDialog(wgt) {
    vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(
        Settings::getLogicalDPI() * 0.05, Settings::getLogicalDPI() * 0.05,
        Settings::getLogicalDPI() * 0.05, Settings::getLogicalDPI() * 0.05);
    lbl = new QLabel;
    lbl->setFont(Settings::getFont3());
    lbl->setWordWrap(true);
    lbl->setAttribute(Qt::WA_TranslucentBackground);
    lbl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    vLayout->addWidget(lbl);

    vLayout->addSpacing(Settings::getLogicalDPI() * 0.20);

    hLayout = new QHBoxLayout;
    okBtn = new QPushButton;
    okBtn->setFont(Settings::getFont3());
    connect(okBtn, SIGNAL(clicked()), SLOT(accept()));
    hLayout->addWidget(okBtn);

    cancelBtn = new QPushButton;
    cancelBtn->setFont(Settings::getFont3());
    connect(cancelBtn, SIGNAL(clicked()), SLOT(reject()));
    hLayout->addWidget(cancelBtn);
    vLayout->addLayout(hLayout);
    setLayout(vLayout);

    onThemeChange();
    onLanguageChange();
}

void MessageBox::setText(QString text) { lbl->setText(text); }
