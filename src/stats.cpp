#include "stats.h"
#include "serialize.h"
#include "settings.h"
#include <QtCore>
#include <iostream>
#include <sstream>

Stats::Stats() : table(tableSize()) {
    generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
    load();
}

Stats &Stats::instance() {
    static Stats instance;
    return instance;
}

std::pair<int, int> Stats::getRandomIndexes(int table_side_from,
                                            int table_side_to, bool intelMode) {
    QVector<int> shrinkedTable =
        getShrinkedTable(table_side_from, table_side_to, intelMode);
    std::discrete_distribution dist(std::begin(shrinkedTable),
                                    std::end(shrinkedTable));
    int index = dist(instance().generator);
    std::pair<int, int> result = fromIndex(index);

    std::uniform_int_distribution<int> ud(0, 1);
    if (ud(instance().generator))
        std::swap(result.first, result.second);
    return result;
}

bool Stats::updateProbability(int problemI, int problemJ, int decisecs,
                              bool promptShowed, bool intelMode) {
    bool progress = false;

    if (intelMode) {
        int value = getValue(problemI, problemJ);

        if (promptShowed)
            decisecs = 149;

        if (decisecs > value) {
            value += std::min(decisecs - value, 25);
            progress = false;
        } else {
            value -= std::min(value - decisecs, 10);
            progress = true;
        }

        value = std::clamp(value, 1, 149);
        setValue(problemI, problemJ, value);
        store();
    }

    return progress;
}

void Stats::load() {
    table = deSerializeVector(Settings::getStats());
    if (table.size() != tableSize()) {
        qDebug() << "can't load stats";
        fill();
    }
}

void Stats::store() { Settings::setStats(serializeVector(instance().table)); }

void Stats::print() {
    qDebug() << "----------------------------";
    for (int j = 2; j <= table_side; j++) {
        for (int i = 2; i <= table_side; i++) {
            std::cout << getValue(i, j) << "\t";
        }
        std::cout << std::endl;
    }
    qDebug() << "----------------------------";
    for (int i = 0; i < tableSize(); i++) {
        std::cout << i << ":" << instance().table[i] << ", ";
    }
    std::cout << std::endl;
}

void Stats::fill() {
    std::fill(std::begin(table), std::end(table), 60);
    //    for (int i=0; i<table.size(); i++)
    //        table[i] = static_cast<double>(i)/table.size()*80+40;
}

int Stats::toIndex(int i, int j) {
    if ((i < 2 || i > table_side) | (j < 2 || j > table_side)) {
        qDebug() << "incorrect indexes" << i << j;
        return 0;
    }

    if (i > j)
        std::swap(i, j); // we work with bottom-left part of the table only. The
                         // top-right part is a mirror of bottom-left

    return tableSize(j) - (j - i + 1);
}

std::pair<int, int> Stats::fromIndex(int index) {
    static bool isInit = false;
    static QVector<std::pair<int, int>> table_index;
    if (!isInit) {
        for (int kj = 2; kj <= table_side; kj++)
            for (int ki = 2; ki <= kj; ki++)
                table_index.push_back(std::make_pair(ki, kj));
        isInit = true;
    }

    std::pair<int, int> result{2, 2};
    if (index > 0 || index < tableSize())
        result = table_index[index];
    else
        qDebug() << "incorrect index" << index;
    return result;
}

int Stats::getValue(int i, int j) { return instance().table[toIndex(i, j)]; }

void Stats::setValue(int i, int j, int value) {
    instance().table[toIndex(i, j)] = value;
}

QVector<int> Stats::getShrinkedTable(int table_side_from, int table_side_to,
                                     bool intelMode) {
    QVector<int> temp = instance().table;
    for (int j = 2; j <= table_side; j++)
        for (int i = j; i <= table_side; i++)
            if ((i < table_side_from && j < table_side_from) ||
                (i > table_side_to || j > table_side_to)) {
                temp[toIndex(i, j)] = 0;
            } else if (intelMode == false)
                temp[toIndex(i, j)] = 1;

    return temp;
}
