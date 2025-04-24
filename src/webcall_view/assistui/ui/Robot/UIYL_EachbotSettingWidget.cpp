#include "UIYL_EachbotSettingWidget.h"
#include "control/CommonSignalService.h"
#include "control/UIMainController.h"
#include "data/CommInfo.h"
#include "service/DbUiWebcallService.h"
#include "protocol_biz/webcallprotocolservice.h"

UIYL_EachbotSettingWidget::UIYL_EachbotSettingWidget(QWidget* parent) : QWidget(parent)
{
    ui.setupUi(this);
    connect(ui.m_okBtn, SIGNAL(clicked()), this, SLOT(R_OnOkClick()));
    connect(ui.m_defaultBtn, SIGNAL(clicked()), this, SLOT(R_OnDefaultClick()));
    connect(ui.m_cancelBtn, SIGNAL(clicked()), this, SLOT(R_OnCancelClick()));
    connect(this, SIGNAL(S_ShowToastMsg_AISettingCenter(QString, int, EYLUI_ToastShowPos, EYLUI_ToastType, QPoint)), CommonSignalService::GetInstance(), SIGNAL(S_ShowToastMsg_AISettingCenter(QString, int, EYLUI_ToastShowPos, EYLUI_ToastType, QPoint)));

    ui.m_aiurl_comb->addItem(tr("AI3引擎"), "0");
    ui.m_aiurl_comb->addItem(tr("AI6引擎"), "1");




    CommInfo* info = gUIPersonalConfigManager->GetUserOtherSetting();
    if (info)
    {
        if (info->GetValueByKey("customeraitype") == "1")
        {
            ui.m_autoRobotRBtn->setChecked(true);
        }
        else if (info->GetValueByKey("customeraitype") == "2")
        {
            ui.m_manJoinBtn->setChecked(true);
        }
        else
            ui.m_manBtn->setChecked(true);
        if (info->GetValueByKey("autojoinchat") == "1")
        {
            ui.m_joinInBtn->setChecked(true);
        }
        else
        {
            ui.m_noJoinInBtn->setChecked(true);
        }
        //人机结合提醒条件
        if (info->GetValueByKey("promptCondition") == "1")
        {
            ui.m_rBtn_currCust->setChecked(true);
        }
        else
        {
            ui.m_rBtn_currCustAndMoniCust->setChecked(true);
        }

        if (info->GetValueByKey("aiurl") == "1")
        {
            ui.m_aiurl_comb->setCurrentIndex(1);
        }
        else
        {
            ui.m_aiurl_comb->setCurrentIndex(0);
        }
    }
    else
    {
        QString isEachbot = gUISystemConfigManager->GetSystemConfigByKey("iseachbot", "0");
        if (isEachbot == "1")
        {
            ui.m_autoRobotRBtn->setChecked(true);
            ui.m_noJoinInBtn->setChecked(true);
        }
        else
        {
            ui.m_manBtn->setChecked(true);
            ui.m_noJoinInBtn->setChecked(true);
        }
        ui.m_rBtn_currCustAndMoniCust->setChecked(true);
    }
    ui.m_cancelBtn->hide();
}

UIYL_EachbotSettingWidget::~UIYL_EachbotSettingWidget()
{
}

void UIYL_EachbotSettingWidget::R_OnOkClick()
{
    CommInfo info(*gUIPersonalConfigManager->GetUserOtherSetting());
    if (ui.m_manJoinBtn->isChecked())
    {
        info.SetValueByKey("customeraitype", "2");
    }
    else if (ui.m_autoRobotRBtn->isChecked())
    {
        info.SetValueByKey("customeraitype", "1");
    }
    else
    {
        info.SetValueByKey("customeraitype", "0");
    }
    if (ui.m_joinInBtn->isChecked())
    {
        info.SetValueByKey("autojoinchat", "1");
    }
    else
    {
        info.SetValueByKey("autojoinchat", "0");
    }
    //人机结合提醒条件
    if (ui.m_rBtn_currCustAndMoniCust->isChecked())
    {
        info.SetValueByKey("promptCondition", "0");
    }
    else
    {
        info.SetValueByKey("promptCondition", "1");
    }

    if (ui.m_aiurl_comb->currentIndex() == 1)
    {
        info.SetValueByKey("aiurl", "1");
        QString ai6uri = gUISystemConfigManager->GetSystemConfigByKey("autoreplymessageurl_ai6", "https://ai6.eachbot.cn/api/robot/companyid");
        gIProtocolAgent->UpdateAIUrl(ai6uri);
    }
    else
    {
        info.SetValueByKey("aiurl", "0");
        QString ai3uri = gUISystemConfigManager->GetSystemConfigByKey("autoreplymessageurl", "https://eachbot.easyliao.com/api/robot/companyid");
        gIProtocolAgent->UpdateAIUrl(ai3uri);
    }
    gUIPersonalConfigManager->SetUserOtherSetting(info);
    gIProtocolAgent->CreateGetCurrentChatListTimer();
    Q_EMIT S_Closed();
    Q_EMIT S_ShowToastMsg_AISettingCenter(tr("机器人设置保存成功！"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Common, QPoint());
}

void UIYL_EachbotSettingWidget::R_OnCancelClick()
{
    Q_EMIT S_Closed();
}

void UIYL_EachbotSettingWidget::R_OnDefaultClick()
{
    QString isEachbot = gUISystemConfigManager->GetSystemConfigByKey("iseachbot", "0");
    if (isEachbot == "1")
    {
        ui.m_autoRobotRBtn->setChecked(true);
        ui.m_noJoinInBtn->setChecked(true);
        ui.m_rBtn_currCustAndMoniCust->setChecked(true); //人机结合提醒条件
        CommInfo info(*gUIPersonalConfigManager->GetUserOtherSetting());
        info.SetValueByKey("customeraitype", "1");
        info.SetValueByKey("isautoreplayremind", "0");
        info.SetValueByKey("pushrobotmsg", "0");
        info.SetValueByKey("autojoinchat", "0");
        info.SetValueByKey("promptCondition", "0"); //人机结合提醒条件
        gUIPersonalConfigManager->SetUserOtherSetting(info);
    }
    else
    {
        ui.m_manBtn->setChecked(true);
        ui.m_noJoinInBtn->setChecked(true);
        ui.m_rBtn_currCustAndMoniCust->setChecked(true); //人机结合提醒条件

        CommInfo info(*gUIPersonalConfigManager->GetUserOtherSetting());
        info.SetValueByKey("customeraitype", "0");
        info.SetValueByKey("isautoreplayremind", "0");
        info.SetValueByKey("pushrobotmsg", "0");
        info.SetValueByKey("autojoinchat", "0");
        info.SetValueByKey("promptCondition", "0"); //人机结合提醒条件
        gUIPersonalConfigManager->SetUserOtherSetting(info);
    }
    gIProtocolAgent->CreateGetCurrentChatListTimer();
    Q_EMIT S_Closed();
}
