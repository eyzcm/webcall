#include "JsUserCheckHelper.h"
//#include "control/IUIController.h"

#include "include/json/value.h"
#include "utils/jsonhelper.h"


#include "ui/uimessagebox.h"

#include <service/dbuiservice.h>
#include "session/el_sessionservice.h"


#include <utils/UtilsHelper.h>


JsUserCheckHelper::JsUserCheckHelper(): BaseJsLoginHelper()
{

    m_token     = gSessionService->UserInfo()->Token();
    m_companyId = gSessionService->UserInfo()->CompanyId();
    m_userId    = gSessionService->UserInfo()->UserId();
//    m_version   = gUISystemConfigManager->GetCurrentVersion();

    m_isModify = 0;
}

JsUserCheckHelper::~JsUserCheckHelper()
{
}

void JsUserCheckHelper::SetInitData()
{
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
    val["language"]   = language.toStdString();
    val["token"]      = m_token.toStdString();
    val["companyid"]  = m_companyId.toStdString();
    val["userid"]     = m_userId.toStdString();
    val["version"]    = m_version.toStdString();
    val["mac"]        = UtilsHelper::GetClientMac().toStdString();
    QString host      = gSessionService->GetSystemConfigByKey("accountloginurl", "https://auth.easyliao.com");

    val["host"]       = host.toStdString();

    val["isPwdSafed"] = m_isPwdSafed;

    QString jsonStr = QString::fromStdString(val.toStyledString());
    QString jsVal   = QString("InitData(%1)").arg(jsonStr);
    CallJs(jsVal);
}

void JsUserCheckHelper::SetIsPwdSafed(int isPwdSafed)
{
    m_isPwdSafed = isPwdSafed;
}

void JsUserCheckHelper::SetUrl(const QString& url)
{
    if (GetCurrentView())
    {
        GetCurrentView()->setUrl(QUrl(url));
    }
}

void JsUserCheckHelper::JsSetClientParam(QString key, QString val)
{
    if (key == "modify")
    {
        m_isModify = val.toInt();
    }
}

int JsUserCheckHelper::CheckModify()
{
    return m_isModify;
}

QString JsUserCheckHelper::JsGetClientParam(QString key)
{
    return "";
}

void JsUserCheckHelper::SetClientParam(QString key, QString val)
{
    if (key == "modify")
    {
        m_isModify = val.toInt();
    }
}

void JsUserCheckHelper::LoadFinished()
{
    m_isLoadOk = true;
    SetInitData();
}

void JsUserCheckHelper::JsQuitSys()
{
    Q_EMIT S_quitSys();
}
