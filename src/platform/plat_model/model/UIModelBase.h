#pragma once
#include <QList>
#include <QObject>

#include "utils/UISubject.h"
#include "datafactory/UIBaseFactory.h"
#include "plat_model_global.h"
class PLAT_MODEL_EXPORT  UIModelBase : public QObject, public IUISubjectImp, public UIBaseFactory
{
    Q_OBJECT
public:
    UIModelBase();
    virtual ~UIModelBase();

public:
    virtual void Lazy_RemoveCache(QString type, QString strKey, QObject *info)             = 0;
    virtual void Lazy_RemoveCache(QString type, QString strKey, QList<QObject *> infoList) = 0;
    virtual bool IsCheckByKey(QString type, QString strKey, QObject *info)                 = 0;
    virtual void AsyncNotifyObjectChange(QString type, QString strKey, QObject *info);
    virtual void AsyncNotifyObjectListChange(QString type, QString strKey, QList<QObject *> infoList);
    virtual void AsyncNotifyObjectChange_Lazy(QString type, QString strKey, QObject *info);
    virtual void AsyncNotifyObjectListChange_Lazy(QString type, QString strKey, QList<QObject *> infoList);
};
