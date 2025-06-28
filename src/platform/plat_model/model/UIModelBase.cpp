#include "stdafx.h"
#include "model/UIModelBase.h"
#include "LazyProcessAgent.h"

UIModelBase::UIModelBase()
{
}

UIModelBase::~UIModelBase()
{
}

void UIModelBase::AsyncNotifyObjectChange(QString type, QString strKey, QObject *info)
{
    gLazyProcessAgent->Model_AsyncNotifyObjectChange(this, type, strKey, info);
}

void UIModelBase::AsyncNotifyObjectListChange(QString type, QString strKey, QList<QObject *> infoList)
{
    gLazyProcessAgent->Model_AsyncNotifyObjectListChange(this, type, strKey, infoList);
}

void UIModelBase::AsyncNotifyObjectChange_Lazy(QString type, QString strKey, QObject *info)
{
    IUISubjectImp::NotifyObjectChange(type, info);
    Lazy_RemoveCache(type, strKey, info);
}

void UIModelBase::AsyncNotifyObjectListChange_Lazy(QString type, QString strKey, QList<QObject *> infoList)
{
    IUISubjectImp::NotifyObjectListChange(type, infoList);
    Lazy_RemoveCache(type, strKey, infoList);
}
