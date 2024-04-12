#pragma once

#include "settings.h"
#include <QWidget>

class StatsWidget : public QWidget {
    Q_OBJECT

    QPointF handle1, handle2;
    bool handleGrabbed = false;
    int handleGrabbedNumber;
    QPointF shiftGrabbed;
    QPixmap handlePixmap;

    bool alterTable = false;

    double side;
    QPointF corner;
    double cellSide;

    void onThemeChange();
    void onLanguageChange();

  protected:
    virtual bool event(QEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

  public:
    StatsWidget(QWidget *wgt = nullptr);
};
