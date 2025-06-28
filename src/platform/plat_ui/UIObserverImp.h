#pragma once
#include "utils/Plat_EnumDefine.h"

#include <QList>
#include <QObject>
#include <QString>

#include <utils/UIObserver.h>

class IUIObserverImp : public IUIObserver
{
public:
    IUIObserverImp()
    {
        m_type        = E_TAB_ALL_PAGE;
        m_currentType = E_TAB_ALL_PAGE;
    }

public:
    void InitTabPage(E_TABTYPE type)
    {
        m_type = type;
    }
    E_TABTYPE CurrentType() const
    {
        return m_currentType;
    }
    void CurrentType(E_TABTYPE val)
    {
        m_currentType = val;
    }
    E_TABTYPE TabType() const
    {
        return m_type;
    }
    void TabType(E_TABTYPE val)
    {
        m_type = val;
    }
    bool IsDoExec()
    {
        bool isOk = false;
        do
        {
            if (m_type == E_TAB_ALL_PAGE)
            {
                isOk = true;
                break;
            }
            if (m_type == m_currentType)
            {
                isOk = true;
                break;
            }
        } while (false);
        return isOk;
    }
    virtual void Update(QString type, QObject *obj)
    {
        Q_UNUSED(type);
        Q_UNUSED(obj);
    }
    virtual void RemoveUpdate(QString type, QString body)
    {
        Q_UNUSED(type);
        Q_UNUSED(body);
    }
    virtual void UpdateList(QString type, QList<QObject *> objList)
    {
        Q_UNUSED(type);
        Q_UNUSED(objList);
    }

protected:
    E_TABTYPE m_type;
    E_TABTYPE m_currentType;
};
