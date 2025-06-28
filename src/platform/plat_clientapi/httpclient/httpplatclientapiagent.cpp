#include "stdafx.h"
#include "HttpPlatClientApiAgent.h"
#include "session/el_sessionservice.h"
#include "protocol/httpcmd.h"
#include <QFile>
#include "service/util_service.h"
#include "protocol/httpuploadfilecmd.h"
#include "protocol/httpuploaddumpfilecmd.h"
#include "utils/jsonhelper.h"
#include "httpplatcacheagent.h"
#include "utils/utilshelper.h"

QString HttpPlatClientApiAgent::CURRCOLLEAGUEVERSION             = "2.0";

HttpPlatClientApiAgent::HttpPlatClientApiAgent(QObject *parent) : QObject(parent)
{
    m_uploaderrcount = 0;
    m_firstReqChatList = 0;

    m_timer = new QTimer;
    m_timer->setInterval(5 * 1000);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(ReReqTimeOut()));

}



//todo 转到平台层
void HttpPlatClientApiAgent::DoUploadDumpFile(QString fileName)
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


void HttpPlatClientApiAgent::R_DoUploadDumpFileFinished(QString code, QString reason, QString body)
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

    Q_EMIT S_DoUploadDumpFileFinished(ChangeCode(code), reason, body, fileName);
}


//todo 转到平台层
void HttpPlatClientApiAgent::DoUploadFile(const QString &uploadUrl, const QString &fileName, const QString &fileType, const QString &chatId, const QString &companyId, EUSERTYPE type)
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


void HttpPlatClientApiAgent::R_DoUploadFileFinished(QString code, QString reason, QString body) //发送文件完成 1
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
    Q_EMIT S_DoUploadFileFinished(ChangeCode(code), reason, body, type); //发送文件完成 2
}

void HttpPlatClientApiAgent::R_SendingStatus(QString code, QString reason, QString body)
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
    Q_EMIT S_SendingStatus(ChangeCode(code), reason, body, type); //发送文件完成 2
}


//todo 转到平台层
void HttpPlatClientApiAgent::DoGetChatFriendly(int apiType)
{
    HttpCmd *cmd = CreateHttpCmd(json_type);
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_GetChatFriendlyFinished(QString, QString, QString)));
    QString ret = MakeUri("/chat-friend/list");
    ret.append(QString("&apiType=%1").arg(apiType));
    qDebug()<<ret;
    cmd->Uri(ret);
    cmd->Post( QByteArray());
}

void HttpPlatClientApiAgent::GetColleagueList()
{

    m_firstReqChatList = 1;

    HttpCmd *cmd = CreateHttpCmd(defalt_Type);
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_GetColleagueListFinished(QString, QString, QString)));
    QString ret = MakeUri("/dept/all-dept-user");

    cmd->Uri(ret);
    cmd->Post(QByteArray());
}


//todo 转到平台层
void HttpPlatClientApiAgent::GetCurrentChat()
{
    DWORD currentTime = GetTickCount();

    if ((currentTime - m_currentchattime) > 7 * 1000)
    {
        HttpCmd *cmd = CreateHttpCmd(json_type);

        connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_GetCurrentChatFinished(QString, QString, QString)));

        cmd->Uri(MakeUri("getCurrentChat"));
        cmd->Get();
    }
}

//todo 转到平台层
void HttpPlatClientApiAgent::GetCurrentChatRecord(const QString &chatID,std::function<void(QString,QString,QString)> httpretfun)
{

    HttpCmd *cmd = CreateHttpCmd(defalt_Type);
    if( httpretfun != nullptr )
        connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_GetCurrentChatRecordFinished(QString, QString, QString)));
    else{
        connect(cmd, &HttpCmd::S_Finished,httpretfun);
    }


    QString ret = MakeUri("/chat/getChatRecord");
    ret.append(QString("&chatId=%1").arg(chatID));
    cmd->AddCmdData("chatid", chatID);
    cmd->Uri(ret);
    cmd->Get();
}


//todo 转到平台层
void HttpPlatClientApiAgent::GetWaitQueue()
{
    HttpCmd *cmd = CreateHttpCmd(defalt_Type);
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_GetWaitQueueFinished(QString, QString, QString)));

    cmd->Uri(MakeUri("/wait-queue/get"));
    cmd->Post(QByteArray());
}



//todo 转到平台层
void HttpPlatClientApiAgent::DoSaveFriend(const QStringList &friendList)
{
    const QString reqJsonTemp = R"({
                                "friends":{{friends}}
                              })";
    HttpCmd *cmd = CreateHttpCmd(json_type);
    cmd->SetHeader("Content-Type", "application/json;chatset=utf-8");
    cmd->SetHeader("Authorization",  m_token.isEmpty()?gSessionService->Token():m_token);
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_SaveFriendFinished(QString, QString, QString)));


    QString ret     = MakeUri("/chat-friend/add");
    QVariantMap map;
    map.insert("friends",friendList);
    QString body = UtilsHelper::replaceTemplate(reqJsonTemp,QJsonObject::fromVariantMap(map) ) ;
    cmd->Uri(ret);
    cmd->Post(body.toUtf8());

}

void HttpPlatClientApiAgent::R_SaveFriendFinished(QString code, QString reason, QString body)
{
    HttpCmd *cmd = (HttpCmd *)sender();

    if (cmd)
    {
        if (code != "200")
        {
            GLOG(QString("code:%1 reason:%2 Body:\r\n%3").arg(code).arg(reason).arg(body));
        }

        Q_EMIT S_SaveFriendFinished(ChangeCode(code), reason, body);
        cmd->deleteLater();
        cmd = NULL;
    }
}



//todo 转到平台层
void HttpPlatClientApiAgent::DoGetVerifyCurrentChatList(QString chatIds)
{
    const QString reqJsonTemp =
                R"({
                "chatIds":{{chatIds}}
              })";



    if (chatIds.isEmpty())
    {
        return; // SXL没有对话id时,不需要调用http
    }

    HttpCmd *cmd = CreateHttpCmd(json_type);
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_GetVerifyCurrentChatListFinished(QString, QString, QString)));

    QString ret     = MakeUri("/chat/verify-chat-exist");

    cmd->Uri(ret);

    QVariantMap map;
    map.insert("chatIds",chatIds.split(","));

    QString data = UtilsHelper::replaceTemplate(reqJsonTemp,QJsonObject::fromVariantMap(map) ) ;
    cmd->Post(data.toUtf8());


}

void HttpPlatClientApiAgent::R_GetVerifyCurrentChatListFinished(QString code, QString reason, QString body)
{
    HttpCmd *cmd = (HttpCmd *)sender();

    if (cmd)
    {
        if (code != "200")
        {
            GLOG(QString("code:%1 reason:%2 Body:\r\n%3").arg(code).arg(reason).arg(body));
        }

        Q_EMIT S_GetVerifyCurrentChatListFinished(ChangeCode(code), reason, body);
        cmd->deleteLater();
        cmd = NULL;
    }
}



QString HttpPlatClientApiAgent::MakeUri()
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

QString HttpPlatClientApiAgent::MakeUri(QString cmdTag)
{
    QString uri = m_clientUrl;
    return MakeUri(uri, cmdTag);
}

QString HttpPlatClientApiAgent::MakeUri(QString url, QString cmdTag)
{

       QString strUriFormat = "%1%2?u=%3&c=%4&version=%5&_t=%6";


       QString ret = "";
       ret         = strUriFormat.arg(url).arg(cmdTag).arg(UserId()).arg(CompanyId()).arg(CurrentVersion()).arg(QString("%1").arg(QDateTime::currentDateTime().toMSecsSinceEpoch()));
       return ret;
}


void HttpPlatClientApiAgent::R_GetColleagueListFinished(QString code, QString reason, QString body)
{
    const QString tbody = R"({"msg":"成功","code":200,"data":{"colleagueStatus":{},"colleague":[]},"apiVersions":"2.0","succeed":true,"dataVersions":"1710400039476"})";
    Q_EMIT S_GetColleagueListFinished("200","ok", tbody, " ");

#if 0
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
                    if (retapiVersions != HttpPlatClientApiAgent::CURRCOLLEAGUEVERSION)
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
#endif
}



void HttpPlatClientApiAgent::SetClientApiUrl(QString clientUrl)
{
    m_clientUrl = clientUrl;

}

void HttpPlatClientApiAgent::DoGetCurrentChatsRecordList(QString chatlists)
{
    const QString reqJsonTemp = R"({
                                "chatIds":{{chatIds}}
                              })";
    HttpCmd *cmd = CreateHttpCmd(json_type);
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_GetCurrentChatsRecordListFinished(QString, QString, QString)));

    QString ret = MakeUri("/chat/batch-get-chat-record");
    cmd->Uri(ret);

    QVariantMap map;
    map.insert("chatIds",chatlists.split(","));
    QString data = UtilsHelper::replaceTemplate(reqJsonTemp,QJsonObject::fromVariantMap(map) ) ;
    cmd->Post(data.toUtf8());
}

void HttpPlatClientApiAgent::R_GetCurrentChatsRecordListFinished(QString code, QString reason, QString body)
{
    HttpCmd *cmd = (HttpCmd *)sender();

    if (cmd)
    {
        if (code != "200")
        {
            GLOG(QString("code:%1 reason:%2 Body:\r\n%3").arg(code).arg(reason).arg(body));
        }

        Q_EMIT S_GetCurrentChatsRecordListFinished(ChangeCode(code), reason, body);
        cmd->deleteLater();
        cmd = NULL;
    }
}


void HttpPlatClientApiAgent::R_GetCurrentChatFinished(QString code, QString reason, QString body)
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

    Q_EMIT S_GetCurrentChatFinished(ChangeCode(code), reason, body);
}



void HttpPlatClientApiAgent::AddErrorReq(HttpCmd *cmd, int flag)
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

void HttpPlatClientApiAgent::ReReqTimeOut()
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



void HttpPlatClientApiAgent::R_GetWaitQueueFinished(QString code, QString reason, QString body)
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
    }else
    {

        Json::Value val;
        if( JsonHelper::GetJsonValueFromBody(body,val) )
        {
            Q_EMIT S_GetWaitQueueFinished(ChangeCode(code), reason, QString::fromStdString(val["data"].toStyledString()));
        }

    }
}


void HttpPlatClientApiAgent::R_GetChatFriendlyFinished(QString code, QString reason, QString body)
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
    Q_EMIT S_GetChatFriendlyFinished(ChangeCode(code), reason, body);
}


void HttpPlatClientApiAgent::R_GetCurrentChatRecordFinished(QString code, QString reason, QString body)
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
        Q_EMIT S_GetCurrentChatRecordFinished(ChangeCode(code), reason, body, chatId);
    }else
    {
        Json::Value val;
        if( JsonHelper::GetJsonValueFromBody(body,val) )
        {
            Q_EMIT S_GetCurrentChatRecordFinished(ChangeCode(code), reason, QString::fromStdString(val["data"].toStyledString()), chatId);
        }
    }

}


void HttpPlatClientApiAgent::DoAllocationChat(QString visitorId, QString groupId, QString customerId)
{
    HttpCmd *cmd = CreateHttpCmd(json_type);
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_AllocationChatFinished(QString, QString, QString)));

    QString ret = MakeUri("allocationChat");
    ret.append(QString("&visitorId=%1&groupId=%2&customerId=%3").arg(visitorId).arg(groupId).arg(customerId));
    cmd->Uri(ret);
    cmd->Get();
}

void HttpPlatClientApiAgent::R_AllocationChatFinished(QString code, QString reason, QString body)
{
    HttpCmd *cmd = (HttpCmd *)sender();

    if (cmd)
    {
        if (code != "200")
        {
            GLOG(QString("code:%1 reason:%2 Body:\r\n%3").arg(code).arg(reason).arg(body));
        }

        Q_EMIT S_AllocationChatFinished(ChangeCode(code), reason, body);
        cmd->deleteLater();
        cmd = NULL;
    }
}


void HttpPlatClientApiAgent::DoGetBatchUserCurrentChatList(QString type, QStringList list,std::function<void(QString,QString,QString)> httpretfun)
{
    const QString reqJsonTemp =
            R"({
            "chats":{{chats}},
            "firstLogin": 0,
            "pageSize": 10,
            "type": {{type}}
            })";
    HttpCmd *cmd = CreateHttpCmd(json_type);
    if( httpretfun == nullptr)
        connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_GetBatchUserCurrentChatListFinished(QString, QString, QString)));
    else
    {
        connect(cmd, &HttpCmd::S_Finished,  httpretfun);

    }
    QString ret     = MakeUri("/chat/list");
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

    QVariantMap map;
    map.insert("chats",list.join(","));
    map.insert("type",type);
    QString data = UtilsHelper::replaceTemplate(reqJsonTemp,QJsonObject::fromVariantMap(map) ) ;
    cmd->Post(data.toUtf8());
}

void HttpPlatClientApiAgent::R_GetBatchUserCurrentChatListFinished(QString code, QString reason, QString body)
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
        Q_EMIT S_GetBatchUserCurrentChatListFinished(ChangeCode(code), reason, body, chatIds);


    }

    if (cmd)
    {
        cmd->deleteLater();
        cmd = NULL;
    }
}

HttpCmd * HttpPlatClientApiAgent::CreateHttpCmd(ContentType contentType)
{

    HttpCmd *cmd = new HttpCmd;
    switch( contentType ){

    case ContentType::defalt_Type:
        cmd->SetHeader("Content-Type", "application/x-www-form-urlencoded;chatset=utf-8");
        break;
    case ContentType::json_type:
        cmd->SetHeader("Content-Type", "application/json;chatset=utf-8");

        break;
    }
    cmd->SetHeader("Authorization",  m_token.isEmpty()?gSessionService->Token():m_token);
    return cmd;

}


void HttpPlatClientApiAgent::DoLogoutSystem(QString userid)
{
    const QString reqJsonTemp =
                R"({
                "userId":{{userId}}
              })";



    HttpCmd *cmd = CreateHttpCmd(json_type);
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_DoLogoutSystemFinished(QString, QString, QString)));

    QString ret = MakeUri("/user/force-login-out");
    QVariantMap map;
    map.insert("userId",userid);
    QString data = UtilsHelper::replaceTemplate(reqJsonTemp,QJsonObject::fromVariantMap(map) ) ;
    cmd->Post(data.toUtf8());


}

void HttpPlatClientApiAgent::R_DoLogoutSystemFinished(QString code, QString reason, QString body)
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
}

QString HttpPlatClientApiAgent::ChangeCode(const QString & code)
{
    if( code == "0") return "200";
    else return code;
}
