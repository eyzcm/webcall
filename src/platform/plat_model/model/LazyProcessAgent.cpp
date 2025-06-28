#include "stdafx.h"

#include "LazyProcessAgent.h"


LazyProcessAgent::LazyProcessAgent()
{
    Init();
}

LazyProcessAgent::~LazyProcessAgent()
{
}

void LazyProcessAgent::Init()
{
    connect(this, SIGNAL(S_Model_AsyncNotifyObjectChange(UIModelBase*, QString, QString, QObject*)), &m_uiLazyProcessThread, SLOT(R_Model_AsyncNotifyObjectChange(UIModelBase*, QString, QString, QObject*)));

    connect(this, SIGNAL(S_Model_AsyncNotifyObjectListChange(UIModelBase*, QString, QString, QList<QObject*>)), &m_uiLazyProcessThread, SLOT(R_Model_AsyncNotifyObjectListChange(UIModelBase*, QString, QString, QList<QObject*>)));

    connect(&m_uiLazyProcessThread, SIGNAL(S_Model_AsyncNotifyObjectChange_Lazy(UIModelBase*, QString, QString, QObject*)), this, SLOT(R_Model_AsyncNotifyObjectChange_Lazy(UIModelBase*, QString, QString, QObject*)));

    connect(&m_uiLazyProcessThread, SIGNAL(S_Model_AsyncNotifyObjectListChange_Lazy(UIModelBase*, QString, QString, QList<QObject*>)), this, SLOT(R_Model_AsyncNotifyObjectListChange_Lazy(UIModelBase*, QString, QString, QList<QObject*>)));
    m_uiLazyProcessThread.start();
}

void LazyProcessAgent::R_Model_AsyncNotifyObjectChange_Lazy(UIModelBase* objData, QString type, QString strKey, QObject* info)
{
    if (objData)
    {
        objData->AsyncNotifyObjectChange_Lazy(type, strKey, info);
    }
}

void LazyProcessAgent::R_Model_AsyncNotifyObjectListChange_Lazy(UIModelBase* objData, QString type, QString strKey, QList<QObject*> infoList)
{
    if (objData)
    {
        objData->AsyncNotifyObjectListChange_Lazy(type, strKey, infoList);
    }
}

void LazyProcessAgent::Model_AsyncNotifyObjectChange(UIModelBase* objData, QString type, QString strKey, QObject* info)
{
    Q_EMIT S_Model_AsyncNotifyObjectChange(objData, type, strKey, info);
}

void LazyProcessAgent::Model_AsyncNotifyObjectListChange(UIModelBase* objData, QString type, QString strKey, QList<QObject*> info)
{
    Q_EMIT S_Model_AsyncNotifyObjectListChange(objData, type, strKey, info);
}
