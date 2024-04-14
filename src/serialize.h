#pragma once

#include <QString>
#include <QVector>

QString serializeVector(QVector<int> Vector);
QVector<int> deSerializeVector(QString str);
