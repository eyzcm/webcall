#ifndef HttpPlatClientApiOldAgent_H
#define HttpPlatClientApiOldAgent_H

#include <QObject>
#include "utils/UISingleton.h"
#include "utils/Plat_EnumDefine.h"
#include "plat_clientapi_global.h"
#include "protocol/httpcmd.h"
#include <windows.h>



class PLAT_CLIENTAPI_EXPORT HttpPlatClientApiOldAgent : public QObject,public UISingleton<HttpPlatClientApiOldAgent>
{

    Q_OBJECT



    friend class UISingleton<HttpPlatClientApiOldAgent>;

    static QString CURRCOLLEAGUEVERSION;

public:
    explicit HttpPlatClientApiOldAgent(QObject *parent = nullptr);

    void DoUploadDumpFile(QString fileName);
    void DoUploadFile(const QString &uploadUrl, const QString &fileName, const QString &fileType, const QString &chatId, const QString &companyId, EUSERTYPE type);

    void GetChatDetail(const QString &userid, const QString &chatID);
    void GetChatGroup();
    void DoGetChatFriendly(int apiType);
    void GetChatGroupRecord(const QString &groupid);
    void GetChatHistory(const QString &userid);

    void GetColleagueList();
    void GetCompanyInfo();

    void DoGetBatchUserCurrentChatList(QString type, QStringList list);
    void GetCurrentChat();
    void GetCurrentChatRecord(const QString &chatID);


    void DoGetChatRecordByNum();
    void DoSaveFriend(const QStringList &friendList);
    void DoGetNewCurrentChatRecordList(QString chatID);
    void DoGetVerifyCurrentChatList(QString chatIds);
    void DoGetCurrentChatsRecordList(QString chatlists);

    void GetWaitQueue();
    void DoAllocationChat(QString visitorId, QString groupId, QString customerId);


    void SetClientApiUrl(QString clientUrl);

    QString CompanyId(){ return m_companyId;}
    void CompanyId(QString cid){ m_companyId = cid;}
    QString MakeUri();
    QString MakeUri(QString cmdTag);
    QString MakeUri(QString url, QString cmdTag);

    QString UserId()const {return m_userId;}
    void UserId(const QString uid){m_userId = uid;}

    QString CurrentVersion()const {return m_vesion;}
    void CurrentVersion(const QString version){m_vesion = version;}

public slots:
    void R_DoUploadDumpFileFinished(QString code, QString reason, QString body);
    void R_DoUploadFileFinished(QString code, QString reason, QString body);
    void R_SendingStatus(QString code, QString reason, QString body);
    void R_GetChatDetailFinished(QString code, QString reason, QString body);
    void R_DoGetChatRecordByNumFinished(QString code, QString reason, QString body);
    void R_SaveFriendFinished(QString code, QString reason, QString body);
    void R_GetNewCurrentChatRecordListFinished(QString code, QString reason, QString body);
    void R_GetVerifyCurrentChatListFinished(QString code, QString reason, QString body);
    void R_GetColleagueListFinished(QString code, QString reason, QString body);
    void R_GetCurrentChatsRecordListFinished(QString code, QString reason, QString body);
    void R_GetCurrentChatFinished(QString code, QString reason, QString body);
    void R_GetChatHistoryFinished(QString code, QString reason, QString body);
    void R_GetChatGroupRecordFinished(QString code, QString reason, QString body);
    void R_GetWaitQueueFinished(QString code, QString reason, QString body);
    void R_GetChatGroupFinished(QString code, QString reason, QString body);

    void R_GetChatFriendlyFinished(QString code, QString reason, QString body);
    void R_GetCurrentChatRecordFinished(QString code, QString reason, QString body);
    void R_AllocationChatFinished(QString code, QString reason, QString body);
    void R_GetBatchUserCurrentChatListFinished(QString code, QString reason, QString body);


private:
    void ReReqTimeOut();


signals:
    void  S_DoUploadDumpFileFinished(QString code, QString reason, QString body, QString fileName);
    void  S_DoUploadFileFinished(QString code, QString reason, QString body, EUSERTYPE  type); //发送文件完成 2
    void  S_SendingStatus(QString code, QString reason, QString body, EUSERTYPE  type); //发送文件完成 2
    void  S_GetChatDetailFinished(QString code, QString reason, QString body,  QString chatId,  QString userid);
    void  S_DoGetChatRecordByNumFinished(QString code, QString reason, QString body);
    void  S_SaveFriendFinished(QString code, QString reason, QString body);
    void  S_GetNewCurrentChatRecordListFinished(QString code, QString reason, QString body,  QString chatId);
    void  S_GetVerifyCurrentChatListFinished(QString code, QString reason, QString body);
    void S_GetColleagueListFinished(QString code, QString reason, QString body, QString customStatus);
    void S_GetCurrentChatsRecordListFinished(QString code, QString reason, QString body);
    void S_GetCurrentChatFinished(QString code, QString reason, QString body);
    void S_GetChatHistoryFinished(QString code, QString reason, QString body, QString userid);
    void S_GetChatGroupRecordFinished(QString code, QString reason, QString body, QString chatid);
    void S_GetWaitQueueFinished(QString code, QString reason, QString body);
    void S_GetChatGroupFinished(QString code, QString reason, QString body);
    void S_GetChatFriendlyFinished(QString code, QString reason, QString body);
    void S_GetCurrentChatRecordFinished(QString code, QString reason, QString body, QString chatid);

    void S_AllocationChatFinished(QString code, QString reason, const QString &body);
    void S_GetBatchUserCurrentChatListFinished(QString code, QString reason, const QString &body, QString chatIds);



private:
    void AddErrorReq(HttpCmd *cmd, int flag);

private:
    QString m_companyId;
    int m_uploaderrcount;
    ECLIENTTYPE m_clentType;
    QString m_clientUrl;
    QString m_userId;
    QString m_vesion;
    int     m_firstReqChatList;
    DWORD  m_currentchattime;

    QMap<HttpCmd *, int> m_errorReqs;

    QTimer              *m_timer;

#ifdef _TEST
public:
#else
private:
#endif
    QString m_token;



};

#endif // HttpPlatClientApiOldAgent_H
