#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>
#include <vector>
#include "kanji_db.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_showKanjiButton_clicked();
    void on_pronunciationButton_clicked(QAbstractButton *button);
    void on_meaningButton_clicked(QAbstractButton *button);

private:
    QLabel *kanjiLabel;
    QLabel *pronunciationLabel;
    QLabel *meaningLabel;
    QLabel *resultLabel;
    QButtonGroup *pronunciationGroup;
    QButtonGroup *meaningGroup;
    QPushButton *showKanjiButton;
    Kanji currentKanji;
    std::vector<Kanji> kanjiList;
    void displayRandomKanji();
    void displayAnswerOptions();
    void checkAnswer(QPushButton *button, bool isPronunciation);
    void loadKanjiFromFile();
};

#endif // MAINWINDOW_H
