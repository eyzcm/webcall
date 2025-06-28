#include "JsWxLoginHelper.h"
#include "QClipboard"
#include "QFileDialog"
#include "utils/UICommUtilsHelper.h"
#include "session/el_sessionservice.h"

#include "service/dbuiservice.h"
#include "include/json/value.h"
#include "utils/jsonhelper.h"
#include "protocol/serviceaddressmanager.h"
#include "protocol/platprotocolservice.h"
#include "session/el_sessionservice.h"
#include "ui/uimessagebox.h"
#include "utils/utilshelper.h"
#include "service/util_service.h"

JsWxLoginHelper::JsWxLoginHelper() : BaseJsLoginHelper()
{
    m_isCheck    = 0;
    m_isPwdSafed = 0;
}

JsWxLoginHelper::~JsWxLoginHelper()
{
}

void JsWxLoginHelper::SetUrl(const QString &url)
{
    if (GetCurrentView())
    {

        QString path = "file:///";
        GetCurrentView()->setUrl(QUrl(path + url));
    }
}

void JsWxLoginHelper::SetInitData()
{
    if (GetCurrentView())
    {


        QString host = gSessionService->GetSystemConfigByKey("accountloginurl", "https://auth.easyliao.com");

        QString language = "0";
        QLocale locale;
        if (locale.language() == QLocale::Chinese && gSessionService->GetSystemConfigByKey("language", "0") == "0")
        {
            language = "0";
        }
        else
        {
            language = "1";
        }
        Json::Value val;
        val["language"] = language.toStdString();
        val["host"]     = host.toStdString();
        val["mac"]      = UtilsHelper::GetClientMac().toStdString();
        QString jsonStr = QString::fromStdString(val.toStyledString());
        QString jscall  = QString("SetInitData(%1)").arg(jsonStr);
        GSCRIPTLOG(jscall);
        GetCurrentView()->page()->runJavaScript(jscall);

    }
}

void JsWxLoginHelper::SetAccountWayData(QString userId, QString password)
{
    if (GetCurrentView())
    {

        QString host =  gSessionService->GetSystemConfigByKey("accountloginurl", "https://auth.easyliao.com");

        Json::Value val;
        val["host"]     = host.toStdString();
        val["userId"]   = userId.toStdString();
        val["password"] = password.toStdString();

        QString jsonStr = QString::fromStdString(val.toStyledString());
        QString jscall  = QString("SetAccountWayData(%1)").arg(jsonStr);
        GSCRIPTLOG(jscall);
        GetCurrentView()->page()->runJavaScript(jscall);

    }
}

void JsWxLoginHelper::HideWxLogin()
{
    QString jscall = QString("JsSetClientParam('wxlogin','%1')").arg(0);
    GSCRIPTLOG(jscall);
    GetCurrentView()->page()->runJavaScript(jscall);
    jscall = QString("JsSetClientParam('phonelogin','%1')").arg(0);
    GetCurrentView()->page()->runJavaScript(jscall);
}

void JsWxLoginHelper::SetLoginType(int type)
{
    QString jscall = QString("JsSetClientParam('wxlogin','%1')").arg(1);
    if (type == 1)
        jscall = QString("JsSetClientParam('phonelogin','%1')").arg(1);
    GSCRIPTLOG(jscall);
    GetCurrentView()->page()->runJavaScript(jscall);
}

void JsWxLoginHelper::LoadFinished()
{
    SetInitData();
}

void JsWxLoginHelper::Login(QString uid, QString token)
{
    QString loginUrl =  gSessionService->GetSystemConfigByKey("loginurl", "im.easyliao.com");
    if (!loginUrl.isEmpty())
    {
        Q_EMIT S_Login(uid, token, loginUrl);
    }
}

void JsWxLoginHelper::LoginError(QString code, QString msg)
{
    UICommUtilsHelper::ShowCommonBox(tr("登录"), msg);
    if (code == "201")
    {
        Q_EMIT S_AuthorizationLoginDenied();
    }
}

void JsWxLoginHelper::JsSetClientParam(QString key, QString val)
{
    if (key == "checkUrl")
    {
#ifdef Q_OS_WIN
        m_checkUrl = val;
#else
        if( (val.indexOf("http") == 0)||(val.indexOf("file://") == 0))  m_checkUrl = val;
        else
        {
            m_checkUrl = QString("file:///%1/%2").arg(UtilsHelper::GetRunAppDir()).arg(val);
        }
#endif
    }
    else if (key == "isCheck")
    {
        m_isCheck = val.toInt();
    }
    else if (key == "backAccount")
    {
        Q_EMIT S_BackAccountLogin();
    }
    else if (key == "isPwdSafed")
    {
        m_isPwdSafed = val.toInt();
    }
    else if (key == "token")
    {
        gUIUserInfoManager->SetNewToken(val);
        gSessionService->SetUserPass("", val, 1);
        gIProtocolLoginAgent->DoUpdateLoginToken(val);
        Q_EMIT S_UpdateToken(val);
    }
    else if (key == "phone")
    {
        gSessionService->SetValueByKey("phone", val);
    }
}

QString JsWxLoginHelper::JsGetClientParam(QString key)
{
    return "";
}

void JsWxLoginHelper::JsLoginexception(QString error)
{
    UIMessageBox *box = new UIMessageBox(0, 1);
    //    box->SetContentTip(error);
    Q_EMIT S_JsLoginexception(box->exec());
}

void JsWxLoginHelper::JsLoginjump(int i)
{
    Q_EMIT S_JsLoginexception(i);
}

int JsWxLoginHelper::IsPwdSafed()
{
    return m_isPwdSafed;
}

QString JsWxLoginHelper::CheckUrl()
{
    return m_checkUrl;
}

int JsWxLoginHelper::IsCheck()
{
    return m_isCheck;
}
