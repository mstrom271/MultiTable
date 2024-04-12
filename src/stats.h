#pragma once

#include <QVector>
#include <random>

class Stats {
  public:
    static constexpr int table_side = 12;
    static constexpr int tableSize(int tableSide = table_side) {
        return (((tableSide - 1) * (tableSide - 1)) + (tableSide - 1)) / 2;
    }

  private:
    QVector<int> table;
    std::default_random_engine generator;

    Stats();
    Stats(const Stats &) = delete;
    Stats &operator=(const Stats &) = delete;

  public:
    static Stats &getInstance();

    static std::pair<int, int>
    getRandomIndexes(int table_side_from, int table_side_to, bool intelMode);
    static bool updateProbability(int problemI, int problemJ, int decisec,
                                  bool promptShowed, bool intelMode);

    static int toIndex(int i, int j);
    static std::pair<int, int> fromIndex(int index);
    static int getValue(int i, int j);
    static void setValue(int i, int j, int value);
    static QVector<int> getShrinkedTable(int table_side_from, int table_side_to,
                                         bool intelMode);
    static void load();
    static void store();
    static void print();
    static void fill();
};
