#pragma once
#include <QList>
#include <QObject>
#include <QString>

class IUIObserver
{
public:
    virtual void Update(QString type, QObject* obj)                = 0;
    virtual void RemoveUpdate(QString type, QString body)          = 0;
    virtual void UpdateList(QString type, QList<QObject*> objList) = 0;
};
