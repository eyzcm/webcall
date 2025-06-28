#pragma once
#include "model/UIModelBase.h"
#include "utils/UISingleton.h"
#include "UIThreadLazyProcess.h"
#include <QObject>

#include "plat_model_global.h"
class PLAT_MODEL_EXPORT LazyProcessAgent : public QObject, public UISingleton<LazyProcessAgent>
{
    Q_OBJECT

    friend class UISingleton<LazyProcessAgent>;

private:
    LazyProcessAgent();
    ~LazyProcessAgent();

private:
    void Init();

public:
    void Model_AsyncNotifyObjectChange(UIModelBase* objData, QString type, QString strKey, QObject* info);
    void Model_AsyncNotifyObjectListChange(UIModelBase* objData, QString type, QString strKey, QList<QObject*> info);

signals:
    void S_Model_AsyncNotifyObjectChange(UIModelBase* objData, QString type, QString strKey, QObject* info);
    void S_Model_AsyncNotifyObjectListChange(UIModelBase* objData, QString type, QString strKey, QList<QObject*> info);

private slots:
    void R_Model_AsyncNotifyObjectChange_Lazy(UIModelBase* objData, QString type, QString strKey, QObject* info);
    void R_Model_AsyncNotifyObjectListChange_Lazy(UIModelBase* objData, QString type, QString strKey, QList<QObject*> infoList);

private:
    UIThreadLazyProcess m_uiLazyProcessThread;

private:
    UIThreadLazyProcess m_thread;
};

#define gLazyProcessAgent LazyProcessAgent::GetInstance()
