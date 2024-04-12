#include "statswidget.h"
#include "language.h"
#include "settings.h"
#include "stats.h"
#include "theme.h"
#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>

StatsWidget::StatsWidget(QWidget *wgt) : QWidget(wgt) {
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}

void StatsWidget::onThemeChange() {
    handlePixmap.load(":/res/" + Theme::getEffectiveTheme() +
                      "/resize_icon.png");
}

void StatsWidget::onLanguageChange() {}

bool StatsWidget::event(QEvent *event) {
    if (event->type() == ThemeChangeEvent::type) {
        onThemeChange();
        return true;
    } else if (event->type() == LanguageChangeEvent::type) {
        onLanguageChange();
        return true;
    }

    return QWidget::event(event);
}

void StatsWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    // Border title
    QFont font_header(this->font());
    font_header.setPixelSize(cellSide / 2);
    painter.setFont(font_header);
    QFontMetrics fm_header(font_header);

    for (int i = 2; i <= Stats::table_side; i++) {
        QString text = QString::number(i);
        QPointF pointH((i - 1) * cellSide + cellSide / 2 -
                           fm_header.horizontalAdvance(text) / 2.0,
                       cellSide / 2 + fm_header.ascent() -
                           fm_header.lineSpacing() / 2.0);
        painter.drawText(pointH + corner, text);

        QPointF pointV(cellSide / 2 - fm_header.horizontalAdvance(text) / 2.0,
                       (i - 1) * cellSide + cellSide / 2 + fm_header.ascent() -
                           fm_header.lineSpacing() / 2.0);
        painter.drawText(pointV + corner, text);
    }

    // Drawing inside the table
    QFont font_regular(this->font());
    font_regular.setPixelSize(cellSide / 3);
    painter.setFont(font_regular);
    QFontMetrics fm_regular(font_regular);

    for (int i = 2; i <= Stats::table_side; i++) {
        for (int j = 2; j <= Stats::table_side; j++) {
            double color_inc = (Stats::getValue(i, j)) / 150.0;
            QColor color;
            if (color_inc < 0.5) {
                color = QColor(0, 255, 0,
                               std::fmin((1 - color_inc / 0.5) * 255, 255));
            } else {
                color = QColor(255, 0, 0,
                               std::fmin(((color_inc - 0.5) / 0.5) * 255, 255));
            }

            painter.fillRect((i - 1) * cellSide + cellSide * 0.1 + corner.x(),
                             (j - 1) * cellSide + cellSide * 0.1 + corner.y(),
                             cellSide * 0.8, cellSide * 0.8, color);

            QString text;
            if (alterTable)
                text = QString::number(i * j);
            else
                text = QString::number(i) + "x" + QString::number(j);

            painter.drawText(
                (i - 1) * cellSide + cellSide / 2 -
                    fm_regular.horizontalAdvance(text) / 2.0 + corner.x(),
                (j - 1) * cellSide + cellSide / 2 + fm_regular.ascent() -
                    fm_regular.lineSpacing() / 2.0 + corner.y(),
                text);
        }
    }

    // Cut of by shade the numbers which are not under study
    QBrush shadeBrush;
    QPen shadePen;
    if (Theme::getEffectiveTheme() == "DarkTheme") {
        shadeBrush = QColor(0, 0, 0, 210);
        shadePen = QColor(255, 255, 255, 70);
    } else if (Theme::getEffectiveTheme() == "GreyTheme") {
        shadeBrush = QColor(0x55, 0x55, 0x55, 170);
        shadePen = QColor(0, 0, 0, 70);
    } else {
        shadeBrush = QColor(0xDD, 0xDD, 0xDD, 170);
        shadePen = QColor(0, 0, 0, 70);
    }

    painter.fillRect(corner.x(), corner.y(), handle1.x() - corner.x(),
                     handle1.y() - corner.y(), shadeBrush);

    // floor is to avoid incorrect joint of shadows
    painter.fillRect(std::floor(handle2.x()), corner.y(),
                     side - handle2.x() + corner.x(), side, shadeBrush);

    painter.fillRect(std::floor(corner.x()), handle2.y(),
                     std::floor(handle2.x()) - std::floor(corner.x()),
                     side - handle2.y() + corner.y(), shadeBrush);

    painter.setPen(shadePen);
    painter.drawLine(handle1.x(), corner.y(), handle1.x(), handle1.y());

    painter.drawLine(corner.x(), handle1.y(), handle1.x(), handle1.y());

    painter.drawLine(handle2.x(), corner.y(), handle2.x(), handle2.y());

    painter.drawLine(corner.x(), handle2.y(), handle2.x(), handle2.y());

    painter.drawRect(corner.x() + cellSide, corner.y() + cellSide,
                     side - cellSide, side - cellSide);

    painter.drawPixmap(handle1.x() - cellSide / 2, handle1.y() - cellSide / 2,
                       cellSide, cellSide, handlePixmap);
    painter.drawPixmap(handle2.x() - cellSide / 2, handle2.y() - cellSide / 2,
                       cellSide, cellSide, handlePixmap);

    QWidget::paintEvent(event);
}

void StatsWidget::resizeEvent(QResizeEvent *event) {
    setMaximumHeight(Settings::getWindowSize().width());

    side = std::min(width() - 10, height() - 10);
    corner = QPointF(((width() - 10) - side) / 2.0 + 5, 5);
    cellSide = side / Stats::table_side;

    handle1 =
        QPointF(cellSide * (Settings::getTableSide_From() - 1) + corner.x(),
                cellSide * (Settings::getTableSide_From() - 1) + corner.y());
    handle2 = QPointF(cellSide * (Settings::getTableSide_To()) + corner.x(),
                      cellSide * (Settings::getTableSide_To()) + corner.y());

    QWidget::resizeEvent(event);
}

double length(QPointF point) {
    return std::sqrt(point.x() * point.x() + point.y() * point.y());
}

void StatsWidget::mousePressEvent(QMouseEvent *event) {
    double length1 = length(handle1 - event->pos());
    double length2 = length(handle2 - event->pos());
    if (length1 < length2) {
        if (length1 < cellSide * 1.5) {
            handleGrabbed = true;
            handleGrabbedNumber = 1;
            shiftGrabbed = event->pos() - handle1;
        }
    } else {
        if (length2 < cellSide * 1.5) {
            handleGrabbed = true;
            handleGrabbedNumber = 2;
            shiftGrabbed = event->pos() - handle2;
        }
    }

    if (!handleGrabbed) {
        alterTable = true;
        update();
    }

    QWidget::mousePressEvent(event);
}

void StatsWidget::mouseReleaseEvent(QMouseEvent *event) {
    handleGrabbed = false;
    alterTable = false;

    Settings::setTableSide_From(
        std::round((handle1.x() - corner.x()) / cellSide + 1));
    Settings::setTableSide_To(
        std::round((handle2.x() - corner.x()) / cellSide));

    handle1 =
        QPointF(cellSide * (Settings::getTableSide_From() - 1) + corner.x(),
                cellSide * (Settings::getTableSide_From() - 1) + corner.y());
    handle2 = QPointF(cellSide * (Settings::getTableSide_To()) + corner.x(),
                      cellSide * (Settings::getTableSide_To()) + corner.y());

    update();

    QWidget::mouseReleaseEvent(event);
}

void StatsWidget::mouseMoveEvent(QMouseEvent *event) {
    if (handleGrabbed == true) {
        QPointF mousePos = event->pos() - shiftGrabbed;
        if (handleGrabbedNumber == 1) {
            if ((mousePos.x() - corner.x()) > (mousePos.y() - corner.y())) {
                handle1.setX(std::clamp<double>(mousePos.x(),
                                                corner.x() + cellSide,
                                                handle2.x() - cellSide));
                handle1.setY(handle1.x() - corner.x() + corner.y());
            } else {
                handle1.setY(std::clamp<double>(mousePos.y(),
                                                corner.y() + cellSide,
                                                handle2.y() - cellSide));
                handle1.setX(handle1.y() - corner.y() + corner.x());
            }
        } else if (handleGrabbedNumber == 2) {
            if ((mousePos.x() - corner.x()) > (mousePos.y() - corner.y())) {
                handle2.setX(std::clamp<double>(
                    mousePos.x(), handle1.x() + cellSide, corner.x() + side));
                handle2.setY(handle2.x() - corner.x() + corner.y());
            } else {
                handle2.setY(std::clamp<double>(
                    mousePos.y(), handle1.y() + cellSide, corner.y() + side));
                handle2.setX(handle2.y() - corner.y() + corner.x());
            }
        }
        update();
    }

    QWidget::mouseMoveEvent(event);
}
