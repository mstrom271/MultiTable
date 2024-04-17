#pragma once

#include <QVector>
#include <random>

class Stats {
    // only top-right part of the table is used
    // The other part is mirroring values
    // 2x2 3x2 4x2 5x2
    // 	   3x3 4x3 5x3
    // 	       4x4 5x4
    //             5x5
    static constexpr int tableMin = 2;
    static constexpr int tableMax = 12;
    static constexpr int tableSize = tableMax + 1 - tableMin;

    static constexpr int upperLevel = 999;
    static constexpr int lowerLevel = 1;
    static constexpr int rangeLevel = upperLevel - lowerLevel;

    QVector<int> tableStats; // table of knowledge
    std::default_random_engine generator;

    Stats();
    Stats(const Stats &) = delete;
    Stats &operator=(const Stats &) = delete;

    void load();
    void store();
    int &operator()(int x, int y);

  public:
    static Stats &instance();

    static std::pair<int, int> getRandom(int subTableMin, int subTableMax,
                                         bool intelMode);
    static bool update(int x, int y, int timeAnswer, bool hintShowed);

    static int min();
    static int max();
    static int side();
    static int toIndex(int x, int y);
    static std::pair<int, int> fromIndex(int index);
    static int getValue(int x, int y);
    static void setValue(int x, int y, int value);
    static void print();
    static void deleteStats();
};
