#include "serialize.h"
#include <QStringList>

QString serializeVector(QVector<int> vec) {
    QString s;
    for (auto &i : vec)
        s += QString::number(i) + ":";
    return s;
}

QVector<int> deSerializeVector(QString str) {
    QStringList numbers = str.split(":");

    QVector<int> result;
    for (const QString &number : numbers) {
        bool ok;
        int score = number.toInt(&ok);
        if (ok) {
            result.push_back(score);
        }
    }
    return result;
}
