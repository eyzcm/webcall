#ifndef HttpPlatClientApiAgent_H
#define HttpPlatClientApiAgent_H

#include <QObject>
#include "utils/UISingleton.h"
#include "utils/Plat_EnumDefine.h"
#include "plat_clientapi_global.h"
#include "protocol/httpcmd.h"
#include <windows.h>



class PLAT_CLIENTAPI_EXPORT HttpPlatClientApiAgent : public QObject,public UISingleton<HttpPlatClientApiAgent>
{

    Q_OBJECT

    typedef enum{
        defalt_Type=0,
        json_type
    }ContentType;

    friend class UISingleton<HttpPlatClientApiAgent>;

    static QString CURRCOLLEAGUEVERSION;

public:
    explicit HttpPlatClientApiAgent(QObject *parent = nullptr);

    void DoLogoutSystem(QString userid);
    void DoUploadDumpFile(QString fileName);
    void DoUploadFile(const QString &uploadUrl, const QString &fileName, const QString &fileType, const QString &chatId, const QString &companyId, EUSERTYPE type);



    void DoGetChatFriendly(int apiType);
    void GetColleagueList();
    void DoGetBatchUserCurrentChatList(QString type, QStringList list,std::function<void(QString,QString,QString)> httpretfun = nullptr);
    void GetCurrentChat();
    void GetCurrentChatRecord(const QString &chatID,std::function<void(QString,QString,QString)> httpretfun = nullptr);



    void DoSaveFriend(const QStringList &friendList);
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

    QString ChangeCode(const QString & code);

public slots:
    void R_DoLogoutSystemFinished(QString code, QString reason, QString body);
    void R_DoUploadDumpFileFinished(QString code, QString reason, QString body);
    void R_DoUploadFileFinished(QString code, QString reason, QString body);
    void R_SendingStatus(QString code, QString reason, QString body);


    void R_SaveFriendFinished(QString code, QString reason, QString body);
    void R_GetVerifyCurrentChatListFinished(QString code, QString reason, QString body);
    void R_GetColleagueListFinished(QString code, QString reason, QString body);
    void R_GetCurrentChatsRecordListFinished(QString code, QString reason, QString body);
    void R_GetCurrentChatFinished(QString code, QString reason, QString body);


    void R_GetWaitQueueFinished(QString code, QString reason, QString body);


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


    void  S_SaveFriendFinished(QString code, QString reason, QString body);
    void  S_GetNewCurrentChatRecordListFinished(QString code, QString reason, QString body,  QString chatId);
    void  S_GetVerifyCurrentChatListFinished(QString code, QString reason, QString body);
    void S_GetColleagueListFinished(QString code, QString reason, QString body, QString customStatus);
    void S_GetCurrentChatsRecordListFinished(QString code, QString reason, QString body);
    void S_GetCurrentChatFinished(QString code, QString reason, QString body);


    void S_GetWaitQueueFinished(QString code, QString reason, QString body);

    void S_GetChatFriendlyFinished(QString code, QString reason, QString body);
    void S_GetCurrentChatRecordFinished(QString code, QString reason, QString body, QString chatid);

    void S_AllocationChatFinished(QString code, QString reason, const QString &body);
    void S_GetBatchUserCurrentChatListFinished(QString code, QString reason, const QString &body, QString chatIds);



private:
    void AddErrorReq(HttpCmd *cmd, int flag);
    HttpCmd * CreateHttpCmd(ContentType contentType);


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

#endif // HttpPlatClientApiAgent_H
