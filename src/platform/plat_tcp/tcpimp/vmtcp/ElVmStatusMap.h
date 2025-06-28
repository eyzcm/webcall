#pragma once
#include <QList>
#include <QObject>
#include <QString>

class ElVmStatusMap
{
    static QList<QString> statusDesc;

public:
    ElVmStatusMap();
    ~ElVmStatusMap();

    static QList<QString> InitData();
    static QString        GetStatusDesc(int status);
};
