#ifndef PlatCommonModelService_H
#define PlatCommonModelService_H

#include <QObject>
#include "utils/Plat_EnumDefine.h"

#include <utils/UISingleton.h>
#include "plat_model_global.h"
#include <data/CommInfo.h>
#include "data/UISystemTipData.h"

//#include <data/UISystemTipData.h>

class PLAT_MODEL_EXPORT PlatCommonModelService : public QObject, public UISingleton<PlatCommonModelService>
{

    Q_OBJECT

     friend class UISingleton<PlatCommonModelService>; //因为基类在getInstance()中 new T() 时会调用到A的构造函数

private:
     PlatCommonModelService(QObject *parent=nullptr);
     virtual ~PlatCommonModelService(void);

public:
   static PlatCommonModelService * Instance()
   {
       return PlatCommonModelService::GetInstance();
   }

    bool HasRecordHistoryByChatId(const QString chatid, const QString vid);
    UISystemTipData* GetSystemTipByKey(QString type);

    ECLIENTTYPE  GetClientType() const;
    //QString          GetSystemConfigByKey(const QString& key, const QString& defaultString);
    void     SendFileMsg(const QString &fileName, const QString &msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, int callbackId, QMap<QString, QString> map);
    void DoTransChatForMe(const QString &vid, const QString &chatid, const QString &fromUserid);

    void DoSendMonitorChatById(const QString &id);

    //发送文字消息
    void SendRichTextMsg(const QString &msgBody, const QString &msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, QString recordId, CommInfo aiOtherInfo, QString actionId, int callbackId, QMap<QString, QString> map);
    //发送文字消息
    void SendMsg(const QString &msgBody, const QString &msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, QString recordId, int callbackId, QMap<QString, QString> map);
    //自动关闭对话
    void DoAutoQuitChatByChatId(QString chatId);





signals:

private slots:
	void  R_LoginReturnData(QString body);


private:
    ECLIENTTYPE             m_clientType;
};

#endif // PlatCommonModelService_H
