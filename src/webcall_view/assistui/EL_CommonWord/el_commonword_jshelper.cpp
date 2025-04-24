#include "el_commonword_jshelper.h"
#include "control/CommonSignalService.h"
#include "control/IUIController.h"
#include "service/DbUiwebcallService.h"
#include "include/json/value.h"
#include "utils/jsonhelper.h"
#include "utils/utilshelper.h"
#include "session/el_sessionservice.h"
#include "service/util_service.h"


EL_CommonWord_JsHelper::EL_CommonWord_JsHelper(int type)
{
    m_showType = type;
    connect(CommonSignalService::GetInstance(), SIGNAL(S_Commonword_UIRefresh_complete()), this, SLOT(R_GetCommonWordComplete()));
}

void EL_CommonWord_JsHelper::SetUrl(const QString& url)
{
    if (GetCurrentView())
    {
        GetCurrentView()->setUrl(QUrl(url));
    }
}

void EL_CommonWord_JsHelper::LoadFinished(bool isl)
{
    if (isl && GetCurrentView())
    {
        GetCurrentView()->page()->setBackgroundColor(Qt::transparent); //网页透明背景
        Json::Value val;
        val["userid"]    = gSessionService->UserInfo()->UserId().toStdString();
        val["token"]     = gSessionService->UserInfo()->Token().toStdString();
        val["companyid"] = gSessionService->UserInfo()->CompanyId().toStdString();
        val["version"]   = gUISystemConfigManager->GetCurrentVersion().toStdString();
        val["showType"]  = m_showType;
        if (gSessionService->HasPermissionByKey("c_company_commonword_batch_import")) //批量导入
        {
            val["c_company_commonword_batch_import"] = 1;
        }
        else
        {
            val["c_company_commonword_batch_import"] = 0;
        }
        if (gSessionService->HasPermissionByKey("c_company_commonword_batch_export")) //批量导出
        {
            val["c_company_commonword_batch_export"] = 1;
        }
        else
        {
            val["c_company_commonword_batch_export"] = 0;
        }

        QString jsonStr  = QString::fromStdString(val.toStyledString());
        QString jscall   = QString("InitData(%1)").arg(jsonStr);
        GSCRIPTLOG(jscall);
        GetCurrentView()->page()->runJavaScript(jscall);
    }
}

void EL_CommonWord_JsHelper::R_Import_complete(int affiliation)
{
    QString id = ""; //默认获取公司
    if (affiliation == 1)
    {
        id = gSessionService->UserInfo()->UserId(); //获取个人
    }
    IUIController::GetInstance()->GetCustomCommonWord(id);
}

void EL_CommonWord_JsHelper::R_GetCommonWordComplete() //通知JS获取常用语完成
{
    if (GetCurrentView())
    {
        QString jscall = QString("Js_GetCommonWordComplete()");
        GSCRIPTLOG(jscall);
        GetCurrentView()->page()->runJavaScript(jscall);
    }
}
