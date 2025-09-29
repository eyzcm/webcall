#pragma once

#include "UIObserver.h"
#include <QList>
#include <QMutex>
#include <QObject>
#include <QString>

class IUISubject
{
public:
    virtual void RegisterObserver(IUIObserver* obj)                             = 0;
    virtual void UnRegisterObserver(IUIObserver* obj)                           = 0;
    virtual void NotifyObjectChange(QString type, QObject* info)                = 0;
    virtual void NotifyObjectListChange(QString type, QList<QObject*> infoList) = 0;

public:
    QMutex m_mutex;
};

class IUISubjectImp : public IUISubject
{
public:
    virtual void RegisterObserver(IUIObserver* obj)
    {
        QMutexLocker locker(&m_mutex);
        if (obj)
        {
            m_observerlist.append(obj);
        }
    }

    virtual void UnRegisterObserver(IUIObserver* obj)
    {
        QMutexLocker locker(&m_mutex);
        if (obj)
        {
            m_observerlist.removeOne(obj);
        }
    }

    virtual void NotifyRemoveObject(QString type, QString body)
    {
        QMutexLocker locker(&m_mutex);
        Q_FOREACH (IUIObserver* obj, m_observerlist)
        {
            if (obj)
            {
                obj->RemoveUpdate(type, body);
            }
        }
    }

    virtual void NotifyObjectChange(QString type, QObject* info)
    {
        QMutexLocker locker(&m_mutex);
        Q_FOREACH (IUIObserver* obj, m_observerlist)
        {
            if (obj)
            {
                obj->Update(type, info);
            }
        }
    }

    virtual void NotifyObjectListChange(QString type, QList<QObject*> infoList)
    {
        QMutexLocker locker(&m_mutex);
        Q_FOREACH (IUIObserver* obj, m_observerlist)
        {
            if (obj)
            {
                obj->UpdateList(type, infoList);
            }
        }
    }

private:
    QList<IUIObserver*> m_observerlist;
};
