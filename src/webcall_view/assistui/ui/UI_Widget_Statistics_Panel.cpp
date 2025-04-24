#include "UI_Widget_Statistics_Panel.h"
#include "utils/EL_Util_Time.h"
#include "control/IUIController.h"
#include "control/UIMainController.h"
#include "service/DbUiWebcallService.h"
#include "model\YL_Model\UI_Model_Statistics.h"
#include "data/UIOpertorSettinginfo.h"
#include "utils/utilshelper.h"
#include "session/el_sessionservice.h"
#include "model/UICommonDataModel.h"
#include "protocol_biz/webcallprotocolservice.h"
#include "control/CommonSignalService.h"


UI_Widget_Statistics_Panel::UI_Widget_Statistics_Panel(QWidget *parent) : QWidget(parent)
{
    //过渡页显示进度
    gLoginTransitionsPage->SendSchedule("85", tr("初始化底部信息区..."));

    ET("UI_Widget_Statistics_Panel_setupUi");
    ui.setupUi(this);
    Init();
}

UI_Widget_Statistics_Panel::~UI_Widget_Statistics_Panel()
{
    if (m_pBrokenNet != nullptr)
    {
        delete m_pBrokenNet;
        m_pBrokenNet = nullptr;
    }
}

void UI_Widget_Statistics_Panel::Init()
{
    UI_Model_Statistics::GetInstance()->RegisterObserver(this);

    m_pBrokenNet = new UI_Widget_Statistics_BrokenNet_Panel(this);
    m_pBrokenNet->setGeometry(0, 0, this->width(), this->height());
    m_pBrokenNet->hide();
}

void UI_Widget_Statistics_Panel::SetBrokenNet(int isBroken)
{
    if (m_pBrokenNet)
    {
        UIOpertorSettingInfo *opeInfo = gUIPersonalConfigYLService->OpertorSettingInfo();
        if (isBroken)
        {
            if (isBroken == 1)
            {
                m_pBrokenNet->SetErrorFlag(0);
            }
            else if (isBroken == 2)
            {
                m_pBrokenNet->SetErrorFlag(1);
            }

            //断网
            m_pBrokenNet->show();
            QString onlinevoiceFileName = UICommonDataModel::GetInstance()->GetSystemConfigByKey("offlinevoiceFileName", "");
            if (!onlinevoiceFileName.isEmpty())
            {
                if (opeInfo && (opeInfo->OfflinesoundTip() == 1))
                {
                    IUIController::GetInstance()->OnPlayAudition("offlinevoice", onlinevoiceFileName.arg(UtilsHelper::GetRunAppDir()));
                }
            }
        }
        else
        {
            //正常
            m_pBrokenNet->hide();
            QString onlinevoiceFileName = UICommonDataModel::GetInstance()->GetSystemConfigByKey("onlinevoiceFileName", "");
            if (!onlinevoiceFileName.isEmpty())
            {
                if (opeInfo && (opeInfo->OnlinesoundTip() == 1))
                {
                    IUIController::GetInstance()->OnPlayAudition("onlinevoice", onlinevoiceFileName.arg(UtilsHelper::GetRunAppDir()));
                }
            }
        }

		if (opeInfo)
		{
			delete opeInfo;
			opeInfo = nullptr;
		}
    }
}

void UI_Widget_Statistics_Panel::Update(QString type, QObject *obj)
{
    if (StringCompare(type, "UpdateCompanyInfo"))
    {
        MCompanyInfoData *companyInfo = (MCompanyInfoData *)obj;
        if (companyInfo)
        {
            ui.label_custom_companyID->setText(companyInfo->CompanyID());
            ui.label_custom_seatCount->setText(companyInfo->LimitOnlineCount());
            ui.label_custom_expireTime->setText(companyInfo->ExpireTime());
            QString vt = companyInfo->Version();
            if (gUISystemConfigManager->istestVersion())
            {
                vt += QString("(测试版)");
            }
            ui.label_custom_versionNum->setText(vt);
        }
    }
    else if (StringCompare(type, "UpdateCompanyInfoStatistics"))
    {
        MCompanyInfoStatisticsData *companyInfoStatistics = (MCompanyInfoStatisticsData *)obj;
        if (companyInfoStatistics)
        {
            ui.label_custom_todayVisitCount->setText(companyInfoStatistics->ViewCount());
            ui.label_custom_currentLoginCount->setText(companyInfoStatistics->OnlineUserCount());
            ui.label_custom_todayLoginDuration->setText(companyInfoStatistics->TodayUuserOnlienCount());
            ui.label_custom_todayServeTotal->setText(companyInfoStatistics->TodayUserViewCount());
        }
    }
    else if (StringCompare(type, "INIT_DATA"))
    {
        MCompanyInfoData *companyInfo = (MCompanyInfoData *)obj;
        if (companyInfo)
        {
            UICompanyBaseInfo *compBaseInfo = gUIOtherServiceManager->CompanyBaseInfoInfoDup();

            ui.label_custom_companyID->setText(companyInfo->CompanyID());
            ui.label_custom_seatCount->setText(companyInfo->LimitOnlineCount());
            ui.label_custom_expireTime->setText(companyInfo->ExpireTime());

            //SXL 到期提醒
            if (QDateTime::currentDateTime() >= QDateTime::fromString(companyInfo->ExpireTime(), "yyyy-MM-dd").addDays(-7))
            {
                CommonSignalService::GetInstance()->DoExpirationReminder(companyInfo->ExpireTime());
            }
            QString vt = companyInfo->Version();
            if (gUISystemConfigManager->istestVersion())
            {
                vt += QString("(测试版)");
            }
            ui.label_custom_versionNum->setText(vt);
            QString title = QString(tr("公司ID:%1  公司名称:%2  版本号:%3")).arg(compBaseInfo->CompanyId(), compBaseInfo->CompanyName(), vt);
            UIMainController::GetInstance()->SetWindowTitle(title);
            compBaseInfo->deleteLater();
        }
    }
    else
    {
        Q_ASSERT(false);
    }
}

void UI_Widget_Statistics_Panel::RemoveUpdate(QString type, QString body)
{
    // if (StringCompare(type, "REMOVE_ITEM"))
    //{
    //	RemoveRowItem(body);
    // }
}

void UI_Widget_Statistics_Panel::enterEvent(QEvent *event)
{
    this->setStyleSheet("QWidget#widget_bg{background-color: rgba(0, 0, 0, 51);}");
}
void UI_Widget_Statistics_Panel::leaveEvent(QEvent *event)
{
    this->setStyleSheet("");
}

void UI_Widget_Statistics_Panel::resizeEvent(QResizeEvent *event)
{
    if (m_pBrokenNet)
    {
        m_pBrokenNet->setGeometry(0, 0, this->width(), this->height());
    }
}
