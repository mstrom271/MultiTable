#include "stats.h"
#include "serialize.h"
#include "settings.h"
#include <QtCore>
#include <cmath>
#include <iostream>
#include <sstream>

void Stats::load() {
    tableStats.resize(tableSize * tableSize);
    tableStats.fill(0);

    QVector<int> reducedTable = deSerializeVector(Settings::getStats());
    constexpr int reducedTableSize = tableSize * (tableSize + 1) / 2;
    if (reducedTable.size() != reducedTableSize) {
        reducedTable.resize(reducedTableSize);
        reducedTable.fill(lowerLevel);
    }

    int it = 0;
    for (int y = tableMin; y <= tableMax; y++)
        for (int x = y; x <= tableMax; x++)
            operator()(x, y) = reducedTable[it++];
}

void Stats::store() {
    QVector<int> reducedTable;
    for (int y = tableMin; y <= tableMax; y++)
        for (int x = y; x <= tableMax; x++)
            reducedTable.push_back(operator()(x, y));
    Settings::setStats(serializeVector(reducedTable));
}

Stats::Stats() {
    load();
    generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

Stats &Stats::instance() {
    static Stats instance;
    return instance;
}

int &Stats::operator()(int x, int y) {
    if (x < y)
        std::swap(x, y);
    return tableStats[toIndex(x, y)];
}

int Stats::min() { return instance().tableMin; }

int Stats::max() { return instance().tableMax; }

int Stats::side() { return instance().tableMax - instance().tableMin; }

int Stats::toIndex(int x, int y) {
    return (y - tableMin) * tableSize + (x - tableMin);
}

QPair<int, int> Stats::fromIndex(int index) {
    // convert index to x, y pair in main table
    int x = index % tableSize;
    int y = index / tableSize;
    return QPair<int, int>(x + tableMin, y + tableMin);
}

int Stats::getValue(int x, int y) { return instance().operator()(x, y); }

void Stats::setValue(int x, int y, int value) {
    instance().operator()(x, y) = value;
    instance().store();
}

void Stats::print() {
    std::cout << "------------------------" << std::endl;
    for (int y = tableMin; y <= tableMax; y++) {
        for (int x = tableMin; x <= tableMax; x++) {
            std::cout << std::setw(4) << getValue(x, y);
        }
        std::cout << std::endl;
    }
}

void Stats::deleteStats() {
    // TODO separate fill func
    QVector<int> reducedTable;
    constexpr int reducedTableSize = tableSize * (tableSize + 1) / 2;
    if (reducedTable.size() != reducedTableSize) {
        reducedTable.resize(reducedTableSize);
        constexpr int lowerLevel = 1;
        reducedTable.fill(lowerLevel);
    }

    instance().store();
}

// choose random element in subtable
QPair<int, int> Stats::getRandom(int subTableMin, int subTableMax,
                                 bool intelMode) {
    QVector<int> subTable(instance().tableStats);

    // fill cells outside of subtable with 0 probability
    for (int y = tableMin; y <= tableMax; y++) {
        for (int x = y; x <= tableMax; x++) {
            if ((x < subTableMin || x > subTableMax) ||
                (y < subTableMin || y > subTableMax))
                subTable[toIndex(x, y)] = 0;
            else if (!intelMode)
                subTable[toIndex(x, y)] = lowerLevel;
            else
                // turn upside down to get probability
                subTable[toIndex(x, y)] =
                    upperLevel - subTable[toIndex(x, y)] + lowerLevel;
        }
    }

    // get index of random cell
    std::discrete_distribution desc_dist(std::begin(subTable),
                                         std::end(subTable));
    int index = desc_dist(instance().generator);
    auto coord = fromIndex(index);

    // x always bigger than y due to top-right table. Randomize it!
    std::uniform_int_distribution<int> uniform_dist(0, 1);
    if (uniform_dist(instance().generator))
        std::swap(coord.first, coord.second);

    return coord;
}

bool Stats::update(int x, int y, int timeAnswer, bool hintShowed) {
    // level of knowladge
    int relativeLevel = getValue(x, y) - lowerLevel;

    // time limit in decisec
    constexpr int upperTimeLimit = 50;
    constexpr int lowerTimeLimit = 10;
    constexpr int rangeTimeLimit = upperTimeLimit - lowerTimeLimit;
    double timeLimitCoef = -1.0 / rangeLevel * relativeLevel + 1.0;
    int timeLimitCur = lowerTimeLimit + timeLimitCoef * rangeTimeLimit;

    // rise power (in case of timeAnswer <= timeLimitCur)
    constexpr int upperRise = 50;
    constexpr int lowerRise = 5;
    constexpr int rangeRise = upperRise - lowerRise;
    double riseCoef = -log(relativeLevel + 1) / log(rangeLevel + 1) + 1;
    int riseCur = lowerRise + riseCoef * rangeRise;

    // fall power (in case of timeAnswer > timeLimitCur)
    constexpr int upperFall = -50;
    constexpr int lowerFall = -5;
    constexpr int rangeFall = upperFall - lowerFall;
    double fallCoef =
        1.0 / (rangeLevel * rangeLevel) * (relativeLevel * relativeLevel);
    int fallCur = lowerFall + fallCoef * rangeFall;

    // multiply time of answer if hint have been used
    constexpr double hintShowedCoef = 2.0;
    if (hintShowed)
        timeAnswer *= hintShowedCoef;

    // update level in the stats table
    int newLevel;
    bool progress;
    if (timeAnswer <= timeLimitCur) {
        newLevel = lowerLevel + relativeLevel + riseCur;
        progress = true;
    } else {
        newLevel = lowerLevel + relativeLevel + fallCur;
        progress = false;
    }
    newLevel = std::clamp(newLevel, lowerLevel, upperLevel);
    setValue(x, y, newLevel);

    qDebug() << "progress: " << progress << ", timeAnswer: " << timeAnswer
             << ", newLevel: " << newLevel;
    return progress;
}
