#ifndef KANJI_DB_H
#define KANJI_DB_H

#include <QString>
#include <vector>

// Структура для представления кандзи
struct Kanji {
    QString character;
    QString pronunciation;
    QString meaning;
};

// Функция для получения списка кандзи
std::vector<Kanji> getKanjiList();

#endif // KANJI_DB_H
