#include "exercisewidget.h"
#include "language.h"
#include "settings.h"
#include "sound.h"
#include "stats.h"
#include "theme.h"
#include <QDebug>
#include <QKeyEvent>
#include <QPainter>
#include <chrono>

using namespace std::chrono;
typedef duration<long long, std::ratio<1, 10>> deciseconds;

void ExerciseWidget::generateProblem() {
    problemIndexes =
        Stats::getRandom(Settings::getTableSide_From(),
                         Settings::getTableSide_To(), Settings::getIntelMode());

    result.clear();
    showProblem();

    const auto now = steady_clock::now();
    timer_problem = duration_cast<deciseconds>(now.time_since_epoch()).count();

    promptShowed = false;
}

void ExerciseWidget::onThemeChange() {}

void ExerciseWidget::onLanguageChange() {
    answersText = tr("Answers");
    answerLabel->setText("<p>" + answersText + ":</p><p>" +
                         QString::number(answers) + "</p>");

    timeLabelText = tr("Time left");
    btnBS->setText(tr("Backspace"));
    btnPrompt->setText(tr("Prompt"));
    stopBtn->setText(tr("Stop"));
}

bool ExerciseWidget::event(QEvent *event) {
    if (event->type() == ThemeChangeEvent::type) {
        onThemeChange();
        return true;
    } else if (event->type() == LanguageChangeEvent::type) {
        onLanguageChange();
        return true;
    }

    return QWidget::event(event);
}

void ExerciseWidget::resizeEvent(QResizeEvent *event) {
    QFont font3(Settings::getFont3());
    QFont font1(Settings::getFont1());

    font3.setPixelSize(
        std::min(font3.pixelSize(), Settings::getWindowSize().height() / 22));
    font1.setPixelSize(
        std::min(font1.pixelSize(), Settings::getWindowSize().height() / 10));

    timeLabel->setMinimumHeight(font3.pixelSize() * 2.5);
    timeLabel->setMaximumHeight(font3.pixelSize() * 3);
    timeLabel->setFont(font3);

    answerLabel->setMinimumHeight(font3.pixelSize() * 2.5);
    answerLabel->setMaximumHeight(font3.pixelSize() * 3);
    answerLabel->setFont(font3);

    problemLabel->setMinimumHeight(font1.pixelSize() * 1);
    problemLabel->setMaximumHeight(font1.pixelSize() * 2);
    problemLabel->setFont(font1);

    for (auto btn :
         {btn1, btn2, btn3, btn4, btn5, btn6, btn7, btn8, btn9, btn0}) {
        btn->setMinimumHeight(font1.pixelSize() * 1);
        btn->setMaximumHeight(font1.pixelSize() * 1.2);
        btn->setFont(font1);
    }

    btnPrompt->setMinimumHeight(font1.pixelSize() * 1);
    btnPrompt->setMaximumHeight(font1.pixelSize() * 1.2);
    btnPrompt->setFont(font3);

    btnBS->setMinimumHeight(font1.pixelSize() * 1);
    btnBS->setMaximumHeight(font1.pixelSize() * 1.2);
    btnBS->setFont(font3);

    stopBtn->setMinimumHeight(Settings::getLogicalDPI() * 0.15);
    stopBtn->setMaximumHeight(Settings::getLogicalDPI() * 0.25);
    stopBtn->setFont(font3);

    QWidget::resizeEvent(event);
}

void ExerciseWidget::paintEvent(QPaintEvent *event) {

    QWidget::paintEvent(event);
}

void ExerciseWidget::showEvent(QShowEvent *event) {
    Sound::bgMusicPlayNext();

    startExercise();
    stopBtn->setFocus();

    QWidget::showEvent(event);
}

void ExerciseWidget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Back || event->key() == Qt::Key_Backspace)
        stopExercise();
}

ExerciseWidget::ExerciseWidget(QWidget *wgt) : QWidget(wgt) {
    vLayout = new QVBoxLayout;
    hLayout = new QHBoxLayout;
    timeLabel = new QLabel;
    timeLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
    timeLabel->setWordWrap(true);
    timeLabel->setAttribute(Qt::WA_TranslucentBackground);
    hLayout->addWidget(timeLabel);

    answerLabel = new QLabel;
    answerLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
    answerLabel->setWordWrap(true);
    answerLabel->setAttribute(Qt::WA_TranslucentBackground);
    hLayout->addWidget(answerLabel);
    vLayout->addLayout(hLayout, 3);

    QWidget *spacer1 = new QWidget;
    spacer1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spacer1->setAttribute(Qt::WA_TranslucentBackground);
    vLayout->addWidget(spacer1);

    problemLabel = new QLabel;
    problemLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
    problemLabel->setWordWrap(true);
    problemLabel->setAttribute(Qt::WA_TranslucentBackground);
    vLayout->addWidget(problemLabel, 2);

    QWidget *spacer2 = new QWidget;
    spacer2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spacer2->setAttribute(Qt::WA_TranslucentBackground);
    vLayout->addWidget(spacer2);

    gridLayout = new QGridLayout;
    btn7 = new QPushButton("7");
    connect(btn7, SIGNAL(clicked()), SLOT(clicked_number()));
    gridLayout->addWidget(btn7, 0, 0);

    btn8 = new QPushButton("8");
    connect(btn8, SIGNAL(clicked()), SLOT(clicked_number()));
    gridLayout->addWidget(btn8, 0, 1);

    btn9 = new QPushButton("9");
    connect(btn9, SIGNAL(clicked()), SLOT(clicked_number()));
    gridLayout->addWidget(btn9, 0, 2);

    btn4 = new QPushButton("4");
    connect(btn4, SIGNAL(clicked()), SLOT(clicked_number()));
    gridLayout->addWidget(btn4, 1, 0);

    btn5 = new QPushButton("5");
    connect(btn5, SIGNAL(clicked()), SLOT(clicked_number()));
    gridLayout->addWidget(btn5, 1, 1);

    btn6 = new QPushButton("6");
    connect(btn6, SIGNAL(clicked()), SLOT(clicked_number()));
    gridLayout->addWidget(btn6, 1, 2);

    btn1 = new QPushButton("1");
    connect(btn1, SIGNAL(clicked()), SLOT(clicked_number()));
    gridLayout->addWidget(btn1, 2, 0);

    btn2 = new QPushButton("2");
    connect(btn2, SIGNAL(clicked()), SLOT(clicked_number()));
    gridLayout->addWidget(btn2, 2, 1);

    btn3 = new QPushButton("3");
    connect(btn3, SIGNAL(clicked()), SLOT(clicked_number()));
    gridLayout->addWidget(btn3, 2, 2);

    btn0 = new QPushButton("0");
    connect(btn0, SIGNAL(clicked()), SLOT(clicked_number()));
    gridLayout->addWidget(btn0, 3, 1);

    btnBS = new QPushButton;
    connect(btnBS, SIGNAL(clicked()), SLOT(clicked_backspace()));
    gridLayout->addWidget(btnBS, 3, 2);

    btnPrompt = new QPushButton;
    connect(btnPrompt, SIGNAL(clicked()), SLOT(clicked_prompt()));
    gridLayout->addWidget(btnPrompt, 3, 0);
    vLayout->addLayout(gridLayout, 12);

    // vLayout->addSpacing(Settings::getLogicalDPI()*0.2);
    QWidget *spacer3 = new QWidget;
    spacer3->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spacer3->setAttribute(Qt::WA_TranslucentBackground);
    vLayout->addWidget(spacer3);

    stopBtn = new QPushButton;
    connect(stopBtn, SIGNAL(clicked()), SLOT(stopExercise()));
    vLayout->addWidget(stopBtn, 1);

    setLayout(vLayout);

    QObject::connect(&timer, SIGNAL(timeout()), SLOT(updateTimer()));
}

void ExerciseWidget::clicked_number() {
    Sound::click();

    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    if (btn) {
        bool isDigit;
        btn->text().toInt(&isDigit);
        if (isDigit == true && result.size() < 3)
            result += btn->text();

        if (result.toInt() == problemIndexes.first * problemIndexes.second) {
            const auto now = steady_clock::now();
            int decisecs =
                duration_cast<deciseconds>(now.time_since_epoch()).count() -
                timer_problem;

            Stats::updateProbability(problemIndexes.first,
                                     problemIndexes.second, decisecs,
                                     promptShowed);

            answerLabel->setText("<p>" + answersText + ":</p><p>" +
                                 QString::number(++answers) + "</p>");
            generateProblem();
        }

        showProblem();
    }
}

void ExerciseWidget::clicked_backspace() {
    Sound::click();

    if (result.size() > 0)
        result.resize(result.size() - 1);

    showProblem();
}

void ExerciseWidget::clicked_prompt() {
    Sound::click();

    promptTimer.singleShot(500, this, SLOT(showProblem()));
    promptShowed = true;
    showProblem(true);
}

void ExerciseWidget::startExercise() {
    answers = 0;
    answerLabel->setText("<p>" + answersText + ":</p><p>" +
                         QString::number(answers) + "</p>");

    generateProblem();

    if (Settings::getExerciseTimer() != 0) {
        timer.start(50);

        const auto now = steady_clock::now();
        timer_exercise =
            duration_cast<deciseconds>(now.time_since_epoch()).count();
    } else
        timeLabel->setText("");
}

void ExerciseWidget::updateTimer() {
    if (Settings::getExerciseTimer() != 0) {
        const auto now = steady_clock::now();
        int decisecs =
            duration_cast<deciseconds>(now.time_since_epoch()).count() -
            timer_exercise;
        int remain_sec = (Settings::getExerciseTimer() * 60 - decisecs / 10);

        QString begin = "<p>", end = "</p>";
        if (remain_sec <= 5) {
            begin = "<p style=\"color:red\">";
        }
        timeLabel->setText(begin + timeLabelText + ":" + end + begin +
                           QString::number(remain_sec / 60) + ":" +
                           QString::number(remain_sec % 60) + end);

        if (remain_sec <= 0) {
            stopExercise();
        }
    }
}

void ExerciseWidget::stopExercise() {
    timer.stop();
    promptTimer.stop();
    emit stop();

    Sound::bgMusicStop();
    Sound::switching();
}

void ExerciseWidget::showProblem(bool prompt) {
    if (prompt)
        problemLabel->setText(
            QString::number(problemIndexes.first) + " x " +
            QString::number(problemIndexes.second) + " = " +
            QString::number(problemIndexes.first * problemIndexes.second));
    else
        problemLabel->setText(QString::number(problemIndexes.first) + " x " +
                              QString::number(problemIndexes.second) + " = " +
                              result);
}

void ExerciseWidget::showProblem() { showProblem(false); }
