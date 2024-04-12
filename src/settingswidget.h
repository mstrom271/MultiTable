#pragma once

#include "settings.h"
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QWidget>

class SettingsWidget : public QWidget {
    Q_OBJECT

    QVBoxLayout *vLayout;
    QScrollArea *scrollArea;
    QWidget *scrollWidget;
    QGridLayout *gridLayout;
    QLabel *lbl_caption;

    QLabel *lbl_language;
    QLabel *lbl_language_detailed;
    QComboBox *cmb_language;

    QLabel *lbl_clickSound;
    QLabel *lbl_clickSound_detailed;
    QCheckBox *ckb_clickSound;

    QLabel *lbl_music;
    QLabel *lbl_music_detailed;
    QCheckBox *ckb_music;

    QLabel *lbl_intelMode;
    QLabel *lbl_intelMode_detailed;
    QCheckBox *ckb_intelMode;

    QLabel *lbl_timer;
    QLabel *lbl_timer_detailed;
    QComboBox *cmb_timer;

    QLabel *lbl_theme;
    QLabel *lbl_theme_detailed;
    QComboBox *cmb_theme;
    QPushButton *okBtn;

    void onThemeChange();
    void onLanguageChange();

  protected:
    virtual bool event(QEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;

  public:
    SettingsWidget(QWidget *wgt = nullptr);

  public slots:
    void languageChange(QString s);
    void clickSoundChange(int state);
    void musicChange(int state);
    void intelModeChange(int state);
    void timerChange(QString s);
    void themeChange(QString s);

  signals:
    void ok();
};
