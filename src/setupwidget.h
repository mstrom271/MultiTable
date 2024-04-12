#pragma once

#include "settings.h"
#include "statswidget.h"
#include <QDialog>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QWidget>

class MessageBox : public QDialog {
    Q_OBJECT

    QVBoxLayout *vLayout;
    QLabel *lbl;
    QHBoxLayout *hLayout;
    QPushButton *okBtn, *cancelBtn;

    void onThemeChange();
    void onLanguageChange();

  protected:
    virtual bool event(QEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

  public:
    MessageBox(QWidget *wgt = nullptr);
    void setText(QString text);
};

class SetupWidget : public QWidget {
    Q_OBJECT

    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout1;
    QPushButton *deleteStatsBtn, *settingsBtn, *helpBtn, *likeBtn;
#ifdef FREE_VERSION
    QPushButton *proBtn;
#endif
    QHBoxLayout *hLayout2;
    QLabel *lblGreen, *lblTextGreen, *lblRed, *lblTextRed;
    StatsWidget *tableWgt;
    QPushButton *startBtn;

    QString msgDeleteAll;

    void onThemeChange();
    void onLanguageChange();

  protected:
    virtual bool event(QEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

  public:
    SetupWidget(QWidget *wgt = nullptr);

  public slots:
    void deleteStats();
    void likeApp();

  signals:
    void showSettings();
    void showHelp();
    void start();
};
