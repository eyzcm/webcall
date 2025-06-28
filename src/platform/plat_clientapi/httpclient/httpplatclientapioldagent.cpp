#include "HttpPlatClientApiOldAgent.h"
#include "session/el_sessionservice.h"
#include "protocol/httpcmd.h"
#include <QFile>
#include "service/util_service.h"
#include "protocol/httpuploadfilecmd.h"
#include "protocol/httpuploaddumpfilecmd.h"
#include "utils/jsonhelper.h"
#include "httpplatcacheagent.h"

QString HttpPlatClientApiOldAgent::CURRCOLLEAGUEVERSION             = "2.0";

HttpPlatClientApiOldAgent::HttpPlatClientApiOldAgent(QObject *parent) : QObject(parent)
{
    m_uploaderrcount = 0;
    m_firstReqChatList = 0;

    m_timer = new QTimer;
    m_timer->setInterval(5 * 1000);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(ReReqTimeOut()));

}



//todo 转到平台层
void HttpPlatClientApiOldAgent::DoUploadDumpFile(QString fileName)
{
    HttpUploadDumpFileCmd *cmd = new HttpUploadDumpFileCmd;
    cmd->FileName(fileName);

    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_DoUploadDumpFileFinished(QString, QString, QString)));
    cmd->DumpUploadURL(gSessionService->GetGlobalConfig()->GetValueByKey("dumpUploadURL"));
    //cmd->DumpUploadURL("http://group-mgr.easyliao.com/webcall-collect-log/tool/api/dumpupload2");
    cmd->CompanyId(CompanyId());
    cmd->FileName(fileName);
    cmd->Upload();
}


void HttpPlatClientApiOldAgent::R_DoUploadDumpFileFinished(QString code, QString reason, QString body)
{
    HttpUploadDumpFileCmd *cmd      = (HttpUploadDumpFileCmd *)sender();
    QString                fileName = "";
    if (cmd)
    {
        fileName = cmd->FileName();
        cmd->deleteLater();
        cmd = NULL;
    }
    if (code != "200")
    {
        GLOG(QString("code:%1 reason:%2 Body:\r\n%3").arg(code).arg(reason).arg(body));
    }

    QString bakfilename = fileName;
    int     findex      = bakfilename.lastIndexOf("DUMP");
    if (findex >= 0)
    {
        bakfilename.remove(findex, 4);
        bakfilename.insert(findex, "DUMPBAK");
        QFile file(fileName);
        file.copy(bakfilename);
        file.remove();
    }

    Q_EMIT S_DoUploadDumpFileFinished(code, reason, body, fileName);
}


//todo 转到平台层
void HttpPlatClientApiOldAgent::DoUploadFile(const QString &uploadUrl, const QString &fileName, const QString &fileType, const QString &chatId, const QString &companyId, EUSERTYPE type)
{
    HttpUploadFileCmd *cmd = new HttpUploadFileCmd;
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_DoUploadFileFinished(QString, QString, QString)));
    if (fileType != "client") //上传表情不需要上传中状态
    {
        connect(cmd, SIGNAL(S_SendingStatus(QString, QString, QString)), this, SLOT(R_SendingStatus(QString, QString, QString)));
    }
    if (fileType == "noadvancenotice")
    {
        cmd->AdvanceNotice(fileType);
        cmd->FileType("file");
    }
    else
    {
        cmd->FileType(fileType);
    }
    cmd->FileUploadURL(uploadUrl);
    cmd->ChatId(chatId);
    cmd->CompanyId(companyId);
    cmd->FileName(fileName);
    cmd->SetUserType(type);
    cmd->Upload();
}


//todo 转到平台层
void HttpPlatClientApiOldAgent::GetChatDetail(const QString &userid, const QString &chatID)
{
    HttpCmd *cmd = new HttpCmd;
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_GetChatDetailFinished(QString, QString, QString)));

    QString ret = MakeUri("getChatDetail");
    ret.append(QString("&chatId=%1").arg(chatID));
    cmd->AddCmdData("chatid", chatID);
    cmd->AddCmdData("userid", userid);

    cmd->Uri(ret);
    cmd->Get();
}


void HttpPlatClientApiOldAgent::R_DoUploadFileFinished(QString code, QString reason, QString body) //发送文件完成 1
{
    EUSERTYPE          type = E_USERTYPE_DEFAULT;
    HttpUploadFileCmd *cmd  = (HttpUploadFileCmd *)sender();

    if (code != "200")
    {
        GLOG(QString("code:%1 reason:%2 Body:\r\n%3").arg(code).arg(reason).arg(body));

        m_uploaderrcount++;
        if ((cmd) && m_uploaderrcount < 3)
        {
            cmd->Upload();
            return;
        }
    }
    else
    {
        Json::Value val;
        bool        isOk = JsonHelper::GetJsonValueFromBody(body, val);
        if (isOk)
        {
            QMap<QString, QString> tempMap;
            bool                   isParse = JsonHelper::ParseValueToMap(val, tempMap);
            if (isParse)
            {
                if (tempMap["result"] == "success")
                {
                    if (tempMap["type"] == "file")
                    {
                        if (tempMap["value"].isEmpty())
                        {
                            m_uploaderrcount++;
                            if ((cmd) && m_uploaderrcount < 3)
                            {
                                cmd->Upload();
                                return;
                            }
                        }
                        else
                        {
                            m_uploaderrcount = 0;
                        }
                    }
                }
            }
        }
    }
    if (cmd)
    {
        type = cmd->GetUserType();
        cmd->deleteLater();
        cmd = NULL;
    }
    Q_EMIT S_DoUploadFileFinished(code, reason, body, type); //发送文件完成 2
}

void HttpPlatClientApiOldAgent::R_SendingStatus(QString code, QString reason, QString body)
{
    EUSERTYPE          type = E_USERTYPE_DEFAULT;
    HttpUploadFileCmd *cmd  = (HttpUploadFileCmd *)sender();
    if (cmd)
    {
        type = cmd->GetUserType();
        if (code != "200")
        {
            cmd->deleteLater();
            cmd = NULL;
        }
    }
    Q_EMIT S_SendingStatus(code, reason, body, type); //发送文件完成 2
}

void HttpPlatClientApiOldAgent::R_GetChatDetailFinished(QString code, QString reason, QString body)
{
    QString  chatId = "";
    QString  userid = "";
    HttpCmd *cmd    = (HttpCmd *)sender();
    if (cmd)
    {
        chatId = cmd->GetCmdDataByKey("chatid");
        userid = cmd->GetCmdDataByKey("userid");
        cmd->deleteLater();
        cmd = NULL;
    }
    if (code != "200")
    {
        GLOG(QString("code:%1 reason:%2 Body:\r\n%3").arg(code).arg(reason).arg(body));
    }
    Q_EMIT S_GetChatDetailFinished(code, reason, body, chatId, userid);
}


void HttpPlatClientApiOldAgent::GetChatGroup()
{
    HttpCmd *cmd = new HttpCmd;
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_GetChatGroupFinished(QString, QString, QString)));

    cmd->Uri(MakeUri("getChatGroup"));
    cmd->Get();
}

//todo 转到平台层
void HttpPlatClientApiOldAgent::DoGetChatFriendly(int apiType)
{
    HttpCmd *cmd = new HttpCmd;
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_GetChatFriendlyFinished(QString, QString, QString)));
    QString ret = MakeUri("getFriend");
    ret.append(QString("&apiType=%1").arg(apiType));
    cmd->Uri(ret);
    cmd->Get();
}

void HttpPlatClientApiOldAgent::GetChatGroupRecord(const QString &groupid)
{
    HttpCmd *cmd = new HttpCmd;
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_GetChatGroupRecordFinished(QString, QString, QString)));

    QString ret = MakeUri("getChatGroupRecord");
    ret.append(QString("&groupId=%1").arg(groupid));
    cmd->AddCmdData("groupid", groupid);
    cmd->Uri(ret);
    cmd->Get();
}

void HttpPlatClientApiOldAgent::GetChatHistory(const QString &userid)
{
    HttpCmd *cmd = new HttpCmd;
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_GetChatHistoryFinished(QString, QString, QString)));

    QString ret = MakeUri("getChatHistory");
    ret.append(QString("&userId=%1").arg(userid));

    cmd->AddCmdData("userid", userid);
    cmd->Uri(ret);
    cmd->Get();
}

void HttpPlatClientApiOldAgent::GetColleagueList()
{
    m_firstReqChatList = 1;

    HttpCmd *cmd = new HttpCmd;
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_GetColleagueListFinished(QString, QString, QString)));
    QString ret = MakeUri("colleague");
    ret.append(QString("&apiVersions=%1&dataVersions=%2").arg(HttpPlatClientApiOldAgent::CURRCOLLEAGUEVERSION).arg(HttpPlatCacheAgent::GetInstance()->DataColleagueVersion()));
    cmd->Uri(ret);
    cmd->Get();
}

void HttpPlatClientApiOldAgent::GetCompanyInfo()
{
    HttpCmd *cmd = new HttpCmd;
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_GetCompanyInfoFinished(QString, QString, QString)));
    cmd->Uri(MakeUri("getCompanyInfo"));
    cmd->Get();
}


//todo 转到平台层
void HttpPlatClientApiOldAgent::GetCurrentChat()
{
    DWORD currentTime = GetTickCount();

    if ((currentTime - m_currentchattime) > 7 * 1000)
    {
        HttpCmd *cmd = new HttpCmd;

        connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_GetCurrentChatFinished(QString, QString, QString)));

        cmd->Uri(MakeUri("getCurrentChat"));
        cmd->Get();
    }
}

//todo 转到平台层
void HttpPlatClientApiOldAgent::GetCurrentChatRecord(const QString &chatID)
{
    HttpCmd *cmd = new HttpCmd;
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_GetCurrentChatRecordFinished(QString, QString, QString)));

    QString ret = MakeUri("getCurrentChatRecord");
    ret.append(QString("&chatId=%1").arg(chatID));
    cmd->AddCmdData("chatid", chatID);
    cmd->Uri(ret);
    cmd->Get();
}


//todo 转到平台层
void HttpPlatClientApiOldAgent::GetWaitQueue()
{
    HttpCmd *cmd = new HttpCmd;
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_GetWaitQueueFinished(QString, QString, QString)));

    cmd->Uri(MakeUri("getWaitQueue"));
    cmd->Get();
}


//todo 转到平台层
void HttpPlatClientApiOldAgent::DoGetChatRecordByNum()
{
    static int chatCount = 20;

    HttpCmd *cmd = new HttpCmd;
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_DoGetChatRecordByNumFinished(QString, QString, QString)));

    QString uri    = MakeUri("getChatRecordByNum");
    QString format = "%1&chatCount=%2";
    QString val    = format.arg(uri).arg(chatCount);
    cmd->Uri(val);
    cmd->Get();
}

void HttpPlatClientApiOldAgent::R_DoGetChatRecordByNumFinished(QString code, QString reason, QString body)
{
    HttpCmd *cmd = (HttpCmd *)sender();
    if (cmd)
    {
        cmd->deleteLater();
        cmd = NULL;
    }
    if (code != "200")
    {
        GLOG(QString("code:%1 reason:%2 Body:\r\n%3").arg(code).arg(reason).arg(body));
    }

    Q_EMIT S_DoGetChatRecordByNumFinished(code, reason, body);
}


//todo 转到平台层
void HttpPlatClientApiOldAgent::DoSaveFriend(const QStringList &friendList)
{
    HttpCmd *cmd = new HttpCmd;
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_SaveFriendFinished(QString, QString, QString)));

    QString ret     = MakeUri("saveFriend");
    QString splitId = friendList.join(",");
    ret.append(QString("&users=%1").arg(splitId));
    cmd->Uri(ret);
    cmd->Get();
}

void HttpPlatClientApiOldAgent::R_SaveFriendFinished(QString code, QString reason, QString body)
{
    HttpCmd *cmd = (HttpCmd *)sender();

    if (cmd)
    {
        if (code != "200")
        {
            GLOG(QString("code:%1 reason:%2 Body:\r\n%3").arg(code).arg(reason).arg(body));
        }

        Q_EMIT S_SaveFriendFinished(code, reason, body);
        cmd->deleteLater();
        cmd = NULL;
    }
}


//todo 转到平台层
void HttpPlatClientApiOldAgent::DoGetNewCurrentChatRecordList(QString chatID)
{
    HttpCmd *cmd = new HttpCmd;
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_GetNewCurrentChatRecordListFinished(QString, QString, QString)));

    QString ret = MakeUri("getNewCurrentChatRecord");
    ret.append(QString("&chatId=%1").arg(chatID));
    cmd->AddCmdData("chatid", chatID);
    cmd->Uri(ret);
    cmd->Get();
}

void HttpPlatClientApiOldAgent::R_GetNewCurrentChatRecordListFinished(QString code, QString reason, QString body)
{
    QString  chatId;
    HttpCmd *cmd = (HttpCmd *)sender();

    if (cmd)
    {
        chatId = cmd->GetCmdDataByKey("chatid");

        if (code != "200")
        {
            GLOG(QString("code:%1 reason:%2 Body:\r\n%3").arg(code).arg(reason).arg(body));
        }

        Q_EMIT S_GetNewCurrentChatRecordListFinished(code, reason, body, chatId);
        cmd->deleteLater();
        cmd = NULL;
    }
}


//todo 转到平台层
void HttpPlatClientApiOldAgent::DoGetVerifyCurrentChatList(QString chatIds)
{

    if (chatIds.isEmpty())
    {
        return; // SXL没有对话id时,不需要调用http
    }

    HttpCmd *cmd = new HttpCmd;
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_GetVerifyCurrentChatListFinished(QString, QString, QString)));

    QString ret     = MakeUri("verifyCurrentChat");

    cmd->Uri(ret);
    QString data = QString("chats=%2").arg(chatIds);
    cmd->Post(data.toUtf8());
}

void HttpPlatClientApiOldAgent::R_GetVerifyCurrentChatListFinished(QString code, QString reason, QString body)
{
    HttpCmd *cmd = (HttpCmd *)sender();

    if (cmd)
    {
        if (code != "200")
        {
            GLOG(QString("code:%1 reason:%2 Body:\r\n%3").arg(code).arg(reason).arg(body));
        }

        Q_EMIT S_GetVerifyCurrentChatListFinished(code, reason, body);
        cmd->deleteLater();
        cmd = NULL;
    }
}



QString HttpPlatClientApiOldAgent::MakeUri()
{
    QString strUriFormat = "u=%1&c=%2&token=%3&version=%4&_t=%5";
    switch (m_clentType)
    {
        case E_CLIENT_EDUEDITION:
            strUriFormat.append("&clt=JY");
            break;
        case E_CLIENT_NEWMEDICAL:
            strUriFormat.append("&clt=NM");
            break;
    }
    QString token;
    token = m_token.isEmpty()?gSessionService->Token():m_token;

    QString ret = "";
    ret         = strUriFormat.arg(UserId()).arg(CompanyId()).arg(token).arg(CurrentVersion()).arg(QString("%1").arg(QDateTime::currentDateTime().toMSecsSinceEpoch()));
    return ret;
}

QString HttpPlatClientApiOldAgent::MakeUri(QString cmdTag)
{
    QString uri = m_clientUrl;
    return MakeUri(uri, cmdTag);
}

QString HttpPlatClientApiOldAgent::MakeUri(QString url, QString cmdTag)
{
    QString cmdstr = "";
       if (!cmdTag.isEmpty())
           cmdstr = QString("cmd=%1&").arg(cmdTag);
       QString strUriFormat = "%1?%2u=%3&c=%4&check=%5&version=%6&_t=%7";

       switch (m_clentType)
       {
           case E_CLIENT_EDUEDITION:
               strUriFormat.append("&clt=JY");
               break;
           case E_CLIENT_NEWMEDICAL:
               strUriFormat.append("&clt=NM");
               break;
       }
       QString token;
       token = m_token.isEmpty()?gSessionService->Token():m_token;

       QString ret = "";
       ret         = strUriFormat.arg(url).arg(cmdstr).arg(UserId()).arg(CompanyId()).arg(token).arg(CurrentVersion()).arg(QString("%1").arg(QDateTime::currentDateTime().toMSecsSinceEpoch()));
       return ret;
}


void HttpPlatClientApiOldAgent::R_GetColleagueListFinished(QString code, QString reason, QString body)
{
    QString  customstatus = "";
    HttpCmd *cmd          = (HttpCmd *)sender();
    if (cmd)
    {
        cmd->deleteLater();
        cmd = NULL;
    }
    if (code != "200")
    {
        GLOG(QString("code:%1 reason:%2 Body:\r\n%3").arg(code).arg(reason).arg(body));
    }
    else
    {
        Json::Value val;
        bool        isOk = JsonHelper::GetJsonValueFromBody(body, val);
        if (isOk)
        {
            if (val.isObject())
            {
                QString succeed = JsonHelper::GetValueByKey("succeed", val);
                if (succeed == "1")
                {
                    QString msg            = JsonHelper::GetValueByKey("msg", val);
                    QString retcode        = JsonHelper::GetValueByKey("code", val);
                    QString retapiVersions = JsonHelper::GetValueByKey("apiVersions", val);
                    QString dataVersions   = JsonHelper::GetValueByKey("dataVersions", val);
                    if (retapiVersions != HttpPlatClientApiOldAgent::CURRCOLLEAGUEVERSION)
                    {

                        // api版本不一致
                    }
                    else if (dataVersions == HttpPlatCacheAgent::GetInstance()->DataColleagueVersion())
                    {
                        if ((val["data"].isObject()) && (val["data"]["colleagueStatus"].isObject()))
                            customstatus = QString::fromUtf8(val["data"]["colleagueStatus"].toStyledString().c_str());
                        if (customstatus.isEmpty())
                            customstatus = "  ";
                        Q_EMIT S_GetColleagueListFinished(code, reason, HttpPlatCacheAgent::GetInstance()->BodyColleague(), customstatus);
                        return;
                    }
                    else
                    {
                        HttpPlatCacheAgent::GetInstance()->ApiColleagueVersion(retapiVersions);
                        HttpPlatCacheAgent::GetInstance()->DataColleagueVersion(dataVersions);
                        HttpPlatCacheAgent::GetInstance()->SaveCacheDataByKey("colleaguelist", body);
                        QString newbody = QString::fromUtf8(val["data"].toStyledString().c_str());
                        HttpPlatCacheAgent::GetInstance()->BodyColleague(newbody);

                        {
                            Q_EMIT S_GetColleagueListFinished(code, reason, newbody, customstatus);
                            return;
                        }
                    }
                }
            }
            else if (val.isArray())
            {
                Q_EMIT S_GetColleagueListFinished(code, reason, body, customstatus);
                return;
            }
        }
    }

    QString newcode   = code;
    QString newreason = reason;
    if (newcode == "200")
    {
        newcode   = "701";
        newreason = "api数据解释或版本不一致";
    }
    Q_EMIT S_GetColleagueListFinished(newcode, newreason, body, customstatus);
}



void HttpPlatClientApiOldAgent::SetClientApiUrl(QString clientUrl)
{
    m_clientUrl = clientUrl;

}

void HttpPlatClientApiOldAgent::DoGetCurrentChatsRecordList(QString chatlists)
{
    HttpCmd *cmd = new HttpCmd;
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_GetCurrentChatsRecordListFinished(QString, QString, QString)));

    QString ret = MakeUri("getCurrentChatRecordMap");
    cmd->Uri(ret);
    QString data = QString("chatIds=%1").arg(chatlists);
    cmd->Post(data.toUtf8());
}

void HttpPlatClientApiOldAgent::R_GetCurrentChatsRecordListFinished(QString code, QString reason, QString body)
{
    HttpCmd *cmd = (HttpCmd *)sender();

    if (cmd)
    {
        if (code != "200")
        {
            GLOG(QString("code:%1 reason:%2 Body:\r\n%3").arg(code).arg(reason).arg(body));
        }

        Q_EMIT S_GetCurrentChatsRecordListFinished(code, reason, body);
        cmd->deleteLater();
        cmd = NULL;
    }
}


void HttpPlatClientApiOldAgent::R_GetCurrentChatFinished(QString code, QString reason, QString body)
{
    HttpCmd *cmd = (HttpCmd *)sender();
    if (cmd)
    {
        if (code == "200")
        {
            AddErrorReq(cmd, 1);
            cmd->deleteLater();
            cmd = NULL;
        }
        else
        {
            AddErrorReq(cmd, 0);
        }
    }
    if (code != "200")
    {
        GLOG(QString("code:%1 reason:%2 Body:\r\n%3").arg(code).arg(reason).arg(body));
    }

    m_currentchattime = GetTickCount();

    Q_EMIT S_GetCurrentChatFinished(code, reason, body);
}


void HttpPlatClientApiOldAgent::R_GetChatHistoryFinished(QString code, QString reason, QString body)
{
    QString  cmdUserId = "";
    HttpCmd *cmd       = (HttpCmd *)sender();
    if (cmd)
    {
        cmdUserId = cmd->GetCmdDataByKey("userid");
        cmd->deleteLater();
        cmd = NULL;
    }
    if (code != "200")
    {
        GLOG(QString("code:%1 reason:%2 Body:\r\n%3").arg(code).arg(reason).arg(body));
    }
    Q_EMIT S_GetChatHistoryFinished(code, reason, body, cmdUserId);
}


void HttpPlatClientApiOldAgent::AddErrorReq(HttpCmd *cmd, int flag)
{
    if (m_errorReqs.find(cmd) != m_errorReqs.end())
    {
        if (flag == 1)
            m_errorReqs[cmd] = 100; //SXL 在ReReqTimeOut中删除,列表中HttpCmd
    }
    else
    {
        if (flag == 0)
            m_errorReqs.insert(cmd, 1);
    }
}

void HttpPlatClientApiOldAgent::ReReqTimeOut()
{
    if (gSessionService->Status() <= E_LOGIN_OK_MAINTCP)
            return;
        QList<HttpCmd *> lists;
        QList<HttpCmd *> deletelist;
        for (auto iter = m_errorReqs.begin(); iter != m_errorReqs.end(); iter++)
        {
            HttpCmd *cmd   = iter.key();
            int      count = iter.value();
            if (count == 100) // SXL 重试后成功的,此时cmd已经是空指针了
            {
                lists.append(cmd);
            }
            else if (count * 5 > (15 + 30 + 60)) //超出重试次数,此时cmd需要在此处delete
            {
                lists.append(cmd);
                deletelist.append(cmd);
            }
            else
            {
                m_errorReqs[cmd] = count + 1;
                if (((count * 5) == (15)) ||         // 15秒发一次
                    ((count * 5) == (15 + 30)) ||    // 30秒发第二次
                    ((count * 5) == (15 + 30 + 60))) // 60秒发第三次
                {
                    if (cmd)
                    {
                        cmd->ReReqData();
                    }
                }
            }
        }

        foreach (HttpCmd *cmd, lists)
        {
            m_errorReqs.remove(cmd);
        }
        qDeleteAll(deletelist);
}


void HttpPlatClientApiOldAgent::R_GetChatGroupRecordFinished(QString code, QString reason, QString body)
{
    QString  chatId = "";
    HttpCmd *cmd    = (HttpCmd *)sender();
    if (cmd)
    {
        chatId = cmd->GetCmdDataByKey("groupid");
        cmd->deleteLater();
        cmd = NULL;
    }
    if (code != "200")
    {
        GLOG(QString("code:%1 reason:%2 Body:\r\n%3").arg(code).arg(reason).arg(body));
    }
    Q_EMIT S_GetChatGroupRecordFinished(code, reason, body, chatId);
}


void HttpPlatClientApiOldAgent::R_GetWaitQueueFinished(QString code, QString reason, QString body)
{
    HttpCmd *cmd = (HttpCmd *)sender();
    if (cmd)
    {
        cmd->deleteLater();
        cmd = NULL;
    }
    if (code != "200")
    {
        GLOG(QString("code:%1 reason:%2 Body:\r\n%3").arg(code).arg(reason).arg(body));
        return;
    }

    Q_EMIT S_GetWaitQueueFinished(code, reason, body);
}

void HttpPlatClientApiOldAgent::R_GetChatGroupFinished(QString code, QString reason, QString body)
{
    HttpCmd *cmd = (HttpCmd *)sender();
    if (cmd)
    {
        cmd->deleteLater();
        cmd = NULL;
    }
    if (code != "200")
    {
        GLOG(QString("code:%1 reason:%2 Body:\r\n%3").arg(code).arg(reason).arg(body));
    }
    Q_EMIT S_GetChatGroupFinished(code, reason, body);
}


void HttpPlatClientApiOldAgent::R_GetChatFriendlyFinished(QString code, QString reason, QString body)
{
    HttpCmd *cmd = (HttpCmd *)sender();
    if (cmd)
    {
        cmd->deleteLater();
        cmd = NULL;
    }
    if (code != "200")
    {
        GLOG(QString("code:%1 reason:%2 Body:\r\n%3").arg(code).arg(reason).arg(body));
    }
    Q_EMIT S_GetChatFriendlyFinished(code, reason, body);
}


void HttpPlatClientApiOldAgent::R_GetCurrentChatRecordFinished(QString code, QString reason, QString body)
{
    QString  chatId = "";
    HttpCmd *cmd    = (HttpCmd *)sender();
    if (cmd)
    {
        chatId = cmd->GetCmdDataByKey("chatid");
        cmd->deleteLater();
        cmd = NULL;
    }
    if (code != "200")
    {
        GLOG(QString("code:%1 reason:%2 Body:\r\n%3").arg(code).arg(reason).arg(body));
    }
    Q_EMIT S_GetCurrentChatRecordFinished(code, reason, body, chatId);
}


void HttpPlatClientApiOldAgent::DoAllocationChat(QString visitorId, QString groupId, QString customerId)
{
    HttpCmd *cmd = new HttpCmd;
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_AllocationChatFinished(QString, QString, QString)));

    QString ret = MakeUri("allocationChat");
    ret.append(QString("&visitorId=%1&groupId=%2&customerId=%3").arg(visitorId).arg(groupId).arg(customerId));
    cmd->Uri(ret);
    cmd->Get();
}

void HttpPlatClientApiOldAgent::R_AllocationChatFinished(QString code, QString reason, QString body)
{
    HttpCmd *cmd = (HttpCmd *)sender();

    if (cmd)
    {
        if (code != "200")
        {
            GLOG(QString("code:%1 reason:%2 Body:\r\n%3").arg(code).arg(reason).arg(body));
        }

        Q_EMIT S_AllocationChatFinished(code, reason, body);
        cmd->deleteLater();
        cmd = NULL;
    }
}


void HttpPlatClientApiOldAgent::DoGetBatchUserCurrentChatList(QString type, QStringList list)
{
    HttpCmd *cmd = new HttpCmd;
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_GetBatchUserCurrentChatListFinished(QString, QString, QString)));
    QString ret     = MakeUri("getBatchUserCurrentChat");
    QString chatIds    ;
    if (!list.isEmpty())
    {
        chatIds += ",";
        chatIds += list.join(",");
    }

    QStringList mlist = chatIds.split(",");
    mlist             = mlist.toSet().toList();
    qSort(mlist.begin(), mlist.end());
    chatIds = mlist.join(",");
    cmd->AddCmdData("chatIds", chatIds);
    cmd->Uri(ret);
    QString data = QString("firstLogin=%3&type=%1&chats=%2&maxcount=10").arg(type).arg(chatIds).arg(m_firstReqChatList); // maxcount=0 使用默认值
    cmd->Post(data.toUtf8());
}

void HttpPlatClientApiOldAgent::R_GetBatchUserCurrentChatListFinished(QString code, QString reason, QString body)
{
    HttpCmd *cmd = (HttpCmd *)sender();
    QString  chatIds;

    if (code != "200")
    {
        GLOG(QString("code:%1 reason:%2 Body:\r\n%3").arg(code).arg(reason).arg(body));
    }
    else
    { //防止接口报错重复拉取数据
        chatIds = cmd->GetCmdDataByKey("chatIds");
        Q_EMIT S_GetBatchUserCurrentChatListFinished(code, reason, body, chatIds);


    }

    if (cmd)
    {
        cmd->deleteLater();
        cmd = NULL;
    }
}
