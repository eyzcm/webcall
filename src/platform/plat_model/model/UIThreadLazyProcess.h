#pragma once

#include "UIModelBase.h"
#include <QList>
#include <QObject>
#include <QThread>

#include "plat_model_global.h"
class PLAT_MODEL_EXPORT UIThreadLazyProcess : public QThread
{
    Q_OBJECT

public:
    UIThreadLazyProcess();
    ~UIThreadLazyProcess();

signals:
    void S_Model_AsyncNotifyObjectChange_Lazy(UIModelBase *objData, QString type, QString strKey, QObject *info);
    void S_Model_AsyncNotifyObjectListChange_Lazy(UIModelBase *objData, QString type, QString strKey, QList<QObject *> infoList);

public slots:
    void R_Model_AsyncNotifyObjectChange(UIModelBase *objData, QString type, QString strKey, QObject *info);
    void R_Model_AsyncNotifyObjectListChange(UIModelBase *objData, QString type, QString strKey, QList<QObject *> infoList);

private:
    virtual void run();
};
