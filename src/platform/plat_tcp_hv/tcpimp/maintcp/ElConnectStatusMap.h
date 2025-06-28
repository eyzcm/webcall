#pragma once
#include <QList>
#include <QObject>
#include <QString>

class ElConnectStatusMap
{
    static QList<QString> statusDesc;

public:
    ElConnectStatusMap();
    ~ElConnectStatusMap();

    static QList<QString> InitData();
    static QString        GetStatusDesc(int status);
};
