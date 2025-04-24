#include "JsPanelAIHelper.h"
#include "control/CommonSignalService.h"
#include "control/IUIController.h"
#include "include/json\value.h"
#include "session/el_sessionservice.h"
#include "service/DbUiwebcallService.h"
#include "utils/jsonhelper.h"
#include "model\YL_Model\UIYL_ChatRecordModel.h"
#include "protocol_biz/webcallprotocolservice.h"

#include "ui/uimessagebox.h"
#include "utils/utilshelper.h"

JsPanelAIHelper::JsPanelAIHelper() : BaseJsCommonHelper()
{

    m_isModify = 0;
    m_aiInit   = 0;
    connect(this, SIGNAL(S_ReturnAIData(QString, QString)), this, SLOT(R_ReturnAIData(QString, QString)));
}

JsPanelAIHelper::~JsPanelAIHelper()
{
}

void JsPanelAIHelper::SetInitData(QString data)
{
    m_initData = data;
    if ((m_isLoadOk) && (m_aiInit == 0))
    {
        Json::Value val;
        val["token"]     = gSessionService->UserInfo()->Token().toStdString();
        val["companyid"] = gSessionService->UserInfo()->CompanyId().toStdString();
        val["userid"]    = gSessionService->UserInfo()->UserId().toStdString();
        val["version"]   = gUISystemConfigManager->GetCurrentVersion().toStdString();

        QString jsonStr = QString::fromStdString(val.toStyledString());
        QString jsVal   = QString("InitData(%1)").arg(jsonStr);
        CallJs(jsVal);
        m_aiInit = 1;
    }
    if (m_aiInit == 1)
    {
        Json::Value val;
        val["token"] = gSessionService->UserInfo()->Token().toStdString();
        val["data"]  = m_initData.toStdString();
        ;

        QString jsonStr = QString::fromStdString(val.toStyledString());
        QString jsVal   = QString("LoadAiData(%1)").arg(jsonStr);
        CallJs(jsVal);
    }
}

void JsPanelAIHelper::SetUrl(const QString& url)
{
    if (GetCurrentView())
    {
        if (m_isLoadOk == false)
            GetCurrentView()->setUrl(QUrl(url));
    }
}

void JsPanelAIHelper::JsSetClientParam(QString key, QString val)
{
    if (key == "modify")
    {
        m_isModify = val.toInt();
    }
}

int JsPanelAIHelper::CheckModify()
{
    return m_isModify;
}

QString JsPanelAIHelper::JsGetClientParam(QString key)
{
    return "";
}

void JsPanelAIHelper::SetClientParam(QString key, QString val)
{
    if (key == "modify")
    {
        m_isModify = val.toInt();
    }
}

void JsPanelAIHelper::LoadFinished()
{
    m_isLoadOk = true;

    SetInitData(m_initData);
}

void JsPanelAIHelper::JsOpenAIWindow(QString key, QString jsonval)
{
    CommonSignalService::GetInstance()->DoOpenAIWindow(key, jsonval);
}

void JsPanelAIHelper::R_ReturnAIData(QString key, QString json)
{
    QString jsVal = QString("jsReturnData(%1)").arg(json);
    CallJs(jsVal);
}
