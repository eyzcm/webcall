#include "UIThreadLazyProcess.h"

UIThreadLazyProcess::UIThreadLazyProcess()
{
    moveToThread(this);
}

UIThreadLazyProcess::~UIThreadLazyProcess()
{
}

void UIThreadLazyProcess::run()
{
    exec();
}

void UIThreadLazyProcess::R_Model_AsyncNotifyObjectChange(UIModelBase *objData, QString type, QString strKey, QObject *info)
{
    msleep(1);
    Q_EMIT S_Model_AsyncNotifyObjectChange_Lazy(objData, type, strKey, info);
    msleep(5);
}

void UIThreadLazyProcess::R_Model_AsyncNotifyObjectListChange(UIModelBase *objData, QString type, QString strKey, QList<QObject *> infoList)
{
    msleep(1);
    Q_EMIT S_Model_AsyncNotifyObjectListChange_Lazy(objData, type, strKey, infoList);
    msleep(5);
}
