#include "UIYL_TopMenuFunPanel.h"
#include "utils/ConstStringHelper.h"
#include "UIYL_PersonMainWidget.h"
#include "baseMainWnd.h"
#include "control/CommonSignalService.h"
#include "control/IUIController.h"
#include "control/UIMainController.h"
#include "data/CommInfo.h"
#include "service/DbUiWebcallService.h"
#include "model/UICommonDataModel.h"

#include "protocol_biz/webcallprotocolservice.h"
#include "ui/Common/BaseMainWnd.h"
#include "data/UIOpertorSettinginfo.h"

UIYL_TopMenuFunPanel::UIYL_TopMenuFunPanel(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    Init();
    m_initMenu = 0;
}

UIYL_TopMenuFunPanel::~UIYL_TopMenuFunPanel()
{
}

void UIYL_TopMenuFunPanel::Init()
{
    connect(ui.m_personinfo_btn, SIGNAL(clicked()), this, SLOT(R_personinfo_Clicked()));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_InitTopMenu()), this, SLOT(R_InitTopMenu()));
}

void UIYL_TopMenuFunPanel::InitTopMenu()
{
    QStringList data = gUIPersonalConfigManager->GetTopMenuKeys();
    if (m_initMenu == 0)
    {
        if (data.size() > 0)
            m_initMenu = 1;
        foreach (QString key, data)
        {
            CommInfo *commInfo = UICommonDataModel::GetInstance()->GetTopMenuByKey(key);
            if (commInfo)
            {
                QPushButton *pBtn = new QPushButton(this);
                pBtn->setText(commInfo->GetValueByKey("text"));

                QLocale locale;
                if (locale.language() == QLocale::Chinese && gUISystemConfigManager->GetSystemConfigByKey("language", "0") == "0")
                {
                    pBtn->setText(commInfo->GetValueByKey("text"));
                }
                else
                {
                    pBtn->setText(commInfo->GetValueByKey("menuId"));
                }


                ui.m_top_menu_hori->addWidget(pBtn);
                connect(pBtn, SIGNAL(clicked()), this, SLOT(R_otherMenu_Clicked()));
            }
        }
    }
}

void UIYL_TopMenuFunPanel::R_InitTopMenu()
{
    this->InitTopMenu();
}

void UIYL_TopMenuFunPanel::R_personinfo_Clicked()
{
    gIProtocolAgent->DoUploadOperatorLog("online_client_conversation_39");
    BaseMainWnd *baseWnd = new BaseMainWnd;
    baseWnd->HideMaxBtn();
    baseWnd->HideMinBtn();
    baseWnd->SetDragEnable(true);
    baseWnd->SetWndTitle(tr("个人设置"));
    baseWnd->SetTitlePanelIcon(":/YL/Resources/YL/person/icon_personSetting.png");
    baseWnd->resize(770, 610);

    UIYL_PersonMainWidget *newWnd = new UIYL_PersonMainWidget;
    baseWnd->AddContentWidget(newWnd);
    baseWnd->ShowCenter_Model();
}

void UIYL_TopMenuFunPanel::R_otherMenu_Clicked()
{
    QPushButton *pBtn = (QPushButton *)sender();
    if (pBtn)
    {
        QStringList data = gUIPersonalConfigManager->GetTopMenuKeys();
        foreach (QString key, data)
        {
            CommInfo *commInfo = UICommonDataModel::GetInstance()->GetTopMenuByKey(key);
            if (pBtn->text() == commInfo->GetValueByKey("text"))
            {
                QString     authstr = commInfo->GetValueByKey("auth");
                std::string tt      = authstr.toStdString();
                const char *url     = tt.c_str();
                const char *p       = strstr(url, "&pPath");
                if (p)
					CommonSignalService::GetInstance()->DoShowIeWndByUri(E_Uri_CenterManager, 0, p + 1);
                else
					CommonSignalService::GetInstance()->DoShowIeWndByUri(E_Uri_CenterManager, 0, authstr);
            }
            if (key == "styleConfig")
            {
                //网页风格设置
                gIProtocolAgent->DoUploadOperatorLog("online_client_conversation_42");
            }
            else if (key == "customerGroup")
            {
                //客服分组管理
                gIProtocolAgent->DoUploadOperatorLog("online_client_conversation_41");
            }
            else if (key == "visitor")
            {
                //访客屏蔽管理
                gIProtocolAgent->DoUploadOperatorLog("online_client_conversation_43");
            }
            else if (key == "customerAccount")
            {
                //客服账号管理
                gIProtocolAgent->DoUploadOperatorLog("online_client_conversation_40");
            }
        }
    }
}
