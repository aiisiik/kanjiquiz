#include "mainwindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QRandomGenerator>
#include <algorithm>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Инициализация виджетов
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    kanjiLabel = new QLabel("Kanji Test", this);
    kanjiLabel->setStyleSheet("font:90px");
    layout->addWidget(kanjiLabel);

    pronunciationLabel = new QLabel("Выберите произношение:", this);
    layout->addWidget(pronunciationLabel);

    pronunciationGroup = new QButtonGroup(this);
    for (int i = 0; i < 4; ++i) {
        QPushButton *optionButton = new QPushButton(this);
        pronunciationGroup->addButton(optionButton);
        layout->addWidget(optionButton);
    }
    connect(pronunciationGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(on_pronunciationButton_clicked(QAbstractButton*)));

    meaningLabel = new QLabel("Выберите перевод:", this);
    layout->addWidget(meaningLabel);

    meaningGroup = new QButtonGroup(this);
    for (int i = 0; i < 4; ++i) {
        QPushButton *optionButton = new QPushButton(this);
        meaningGroup->addButton(optionButton);
        layout->addWidget(optionButton);
    }
    connect(meaningGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(on_meaningButton_clicked(QAbstractButton*)));

    resultLabel = new QLabel(this);
    layout->addWidget(resultLabel);

    showKanjiButton = new QPushButton("Начать квиз", this);
    layout->addWidget(showKanjiButton);
    connect(showKanjiButton, &QPushButton::clicked, this, &MainWindow::on_showKanjiButton_clicked);

    setCentralWidget(centralWidget);

    loadKanjiFromFile();
}

MainWindow::~MainWindow() { }

void MainWindow::displayRandomKanji() {
    if (kanjiList.empty()) {
        resultLabel->setText("Нет данных для отображения. Пожалуйста, проверьте файл kanji.txt.");
        showKanjiButton->setEnabled(false);
        return;
    }
    int index = QRandomGenerator::global()->bounded(kanjiList.size());
    currentKanji = kanjiList[index];
    kanjiLabel->setText(currentKanji.character);
}

void MainWindow::displayAnswerOptions() {
    if (kanjiList.empty()) {
        resultLabel->setText("Нет данных для отображения. Пожалуйста, проверьте файл kanji.txt.");
        showKanjiButton->setEnabled(false);
        return;
    }

    std::vector<QString> pronunciationOptions = {currentKanji.pronunciation};
    std::vector<QString> meaningOptions = {currentKanji.meaning};

    while (pronunciationOptions.size() < 4) {
        int index = QRandomGenerator::global()->bounded(kanjiList.size());
        QString option = kanjiList[index].pronunciation;
        if (std::find(pronunciationOptions.begin(), pronunciationOptions.end(), option) == pronunciationOptions.end()) {
            pronunciationOptions.push_back(option);
        }
    }

    while (meaningOptions.size() < 4) {
        int index = QRandomGenerator::global()->bounded(kanjiList.size());
        QString option = kanjiList[index].meaning;
        if (std::find(meaningOptions.begin(), meaningOptions.end(), option) == meaningOptions.end()) {
            meaningOptions.push_back(option);
        }
    }

    std::random_shuffle(pronunciationOptions.begin(), pronunciationOptions.end());
    std::random_shuffle(meaningOptions.begin(), meaningOptions.end());

    for (int i = 0; i < 4; ++i) {
        QPushButton *pronunciationButton = static_cast<QPushButton*>(pronunciationGroup->buttons()[i]);
        pronunciationButton->setText(pronunciationOptions[i]);
        pronunciationButton->setEnabled(true);
        pronunciationButton->setStyleSheet("");

        QPushButton *meaningButton = static_cast<QPushButton*>(meaningGroup->buttons()[i]);
        meaningButton->setText(meaningOptions[i]);
        meaningButton->setEnabled(true);
        meaningButton->setStyleSheet("");
    }
}

void MainWindow::checkAnswer(QPushButton *button, bool isPronunciation) {
    QButtonGroup *group = isPronunciation ? pronunciationGroup : meaningGroup;
    QString correctAnswer = isPronunciation ? currentKanji.pronunciation : currentKanji.meaning;

    if (button->text() == correctAnswer) {
        button->setStyleSheet("background-color: green; color: white;");
    } else {
        button->setStyleSheet("background-color: red; color: white;");
    }

    for (QAbstractButton *otherButton : group->buttons()) {
        if (otherButton != button) {
            otherButton->setEnabled(false);
        }
    }
}

void MainWindow::on_showKanjiButton_clicked() {
    showKanjiButton->setText("Следующий");
    displayRandomKanji();
    displayAnswerOptions();
    resultLabel->clear();
}

void MainWindow::on_pronunciationButton_clicked(QAbstractButton *button) {
    checkAnswer(static_cast<QPushButton*>(button), true);
}

void MainWindow::on_meaningButton_clicked(QAbstractButton *button) {
    checkAnswer(static_cast<QPushButton*>(button), false);
}

void MainWindow::loadKanjiFromFile() {
    QFile file("kanji.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        resultLabel->setText("Файл kanji.txt не найден. Пожалуйста, добавьте файл с данными.");
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(',');
        if (parts.size() == 3) {
            Kanji kanji = { parts[0], parts[1], parts[2] };
            kanjiList.push_back(kanji);
        }
    }
    file.close();
}
