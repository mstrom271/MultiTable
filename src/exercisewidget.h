#pragma once

#include "settings.h"
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QTimer>
#include <QWidget>
#include <random>

class ExerciseWidget : public QWidget {
    Q_OBJECT

    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout;
    QLabel *timeLabel;
    QLabel *answerLabel;
    QLabel *problemLabel;
    QGridLayout *gridLayout;
    QPushButton *btn1, *btn2, *btn3, *btn4, *btn5, *btn6, *btn7, *btn8, *btn9,
        *btn0;
    QPushButton *btnPrompt, *btnBS;
    QPushButton *stopBtn;

    int answers;
    QString answersText;
    QString timeLabelText;
    std::pair<int, int> problemIndexes;
    QString result;
    QTimer timer, promptTimer;
    bool promptShowed;
    int timer_problem;
    int timer_exercise;
    void generateProblem();

    void onThemeChange();
    void onLanguageChange();

  protected:
    virtual bool event(QEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;

  public:
    ExerciseWidget(QWidget *wgt = nullptr);

  public slots:
    void clicked_number();
    void clicked_backspace();
    void clicked_prompt();

    void startExercise();
    void updateTimer();
    void stopExercise();
    void showProblem();
    void showProblem(bool prompt);

  signals:
    void stop();
};
