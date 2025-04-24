#include "YLJS_LemonLovesBeauty_EditPanel_Helper.h"
#include "control/IUIController.h"
#include "include/json\value.h"
#include "session/el_sessionservice.h"
#include "service/DbUiwebcallService.h"
#include "utils/jsonhelper.h"
#include "protocol_biz/webcallprotocolservice.h"
#include "ui/uimessagebox.h"
#include "utils/utilshelper.h"

YLJS_LemonLovesBeauty_EditPanel_Helper::YLJS_LemonLovesBeauty_EditPanel_Helper() : BaseJsCommonHelper()
{
    m_token     = gSessionService->UserInfo()->Token();
    m_companyId = gSessionService->UserInfo()->CompanyId();
    m_userId    = gSessionService->UserInfo()->UserId();
    m_version   = gUISystemConfigManager->GetCurrentVersion();
}

YLJS_LemonLovesBeauty_EditPanel_Helper::~YLJS_LemonLovesBeauty_EditPanel_Helper()
{
}

void YLJS_LemonLovesBeauty_EditPanel_Helper::SetUrl(const QString& url)
{
    if (GetCurrentView())
    {
        GetCurrentView()->setUrl(QUrl(url));
    }
}

void YLJS_LemonLovesBeauty_EditPanel_Helper::SetInitData()
{
    if (GetCurrentView())
    {
        Json::Value val;
        val["token"]     = m_token.toStdString();
        val["companyid"] = m_companyId.toStdString();
        val["userid"]    = m_userId.toStdString();
        val["version"]   = m_version.toStdString();

        QString jsonStr = QString::fromStdString(val.toStyledString());
        QString jsVal   = QString("InitData(%1)").arg(jsonStr);
        CallJs(jsVal);
    }
}

void YLJS_LemonLovesBeauty_EditPanel_Helper::R_RedEnvelopesLinks_Checked(QString databody, QString links)
{
    Q_EMIT S_RedEnvelopesLinks_Checked(databody, links);
}
