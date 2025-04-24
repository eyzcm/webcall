#include "UIYL_ManRobotSettingWidget_AlternateRobot.h"
#include "utils/ConstStringHelper.h"
#include "utils/UISingleton.h"
#include "UI_Widget_SystemPrompt.h"
#include "control/CommonSignalService.h"
#include "service/DbUiWebcallService.h"
#include "utils/jsonhelper.h"
#include "model/UICommonDataModel.h"
#include "model/UIManRobotMatchRuleTipModel.h"
#include "protocol_biz/webcallprotocolservice.h"

#include "data/UIReserveRobotInfo.h"
#include <QRadioButton>

UIYL_ManRobotSettingWidget_AlternateRobot::UIYL_ManRobotSettingWidget_AlternateRobot(QWidget* parent) : QWidget(parent), m_pMask(nullptr), m_bIsResult(false)
{
    ui.setupUi(this);

    connect(ui.m_rbtn_open, SIGNAL(clicked()), this, SLOT(R_RBtn_Clicked()));
    connect(ui.m_rbtn_close, SIGNAL(clicked()), this, SLOT(R_RBtn_Clicked()));
    connect(ui.m_btn_save, SIGNAL(clicked()), this, SLOT(R_Btn_Save_Clicked()));

    connect(CommonSignalService::GetInstance(), SIGNAL(S_AddOrUpdateReserveRobotRelationResult(QString, QString, QString)), this, SLOT(R_AddOrUpdateReserveRobotRelation_Result(QString, QString, QString)));

    //初始化遮罩层
    m_pMask = new QWidget(this);
    m_pMask->setStyleSheet("background-color: rgba(255, 255, 255, 102);");

    m_pTimer = new QTimer();
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(R_Timeout()));

   
    {
        ui.m_rbtn_close->setChecked(true);
        ui.m_sbox_overTime->setValue(10);
        ShowMask();
    }
}

UIYL_ManRobotSettingWidget_AlternateRobot::~UIYL_ManRobotSettingWidget_AlternateRobot()
{
    if (m_pTimer)
    {
        m_pTimer->deleteLater();
        m_pTimer = nullptr;
    }
}

void UIYL_ManRobotSettingWidget_AlternateRobot::ShowMask()
{
    if (m_pMask)
    {
        m_pMask->setGeometry(0, 90, this->width(), this->height() - 90 - 60);
        m_pMask->show();
    }
}

void UIYL_ManRobotSettingWidget_AlternateRobot::ShowToastMsg(QString msg, int showTime, EYLUI_ToastShowPos showPos, EYLUI_ToastType type, QPoint tarPos)
{
    UI_Widget_SystemPrompt* pWidget_SystemPtompt = new UI_Widget_SystemPrompt(this);
    pWidget_SystemPtompt->SetMsg(msg, type);
    pWidget_SystemPtompt->SetShowTime(showTime);
    pWidget_SystemPtompt->SetShowPos(this, showPos, this->mapFromGlobal(tarPos));
    pWidget_SystemPtompt->show();
}

void UIYL_ManRobotSettingWidget_AlternateRobot::R_RBtn_Clicked()
{
    QRadioButton* rbtn = (QRadioButton*)sender();
    if (rbtn == ui.m_rbtn_open)
    {
        m_pMask->hide();
    }
    else
    {
        ShowMask();
    }
}

void UIYL_ManRobotSettingWidget_AlternateRobot::R_Btn_Save_Clicked()
{
    QString strRobotId = ui.m_lineEdit_robotId->text().trimmed();
    if (strRobotId.isEmpty())
    {
        ShowToastMsg(tr("保存失败，请检查候补机器人客服ID是否正确!"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Common, QPoint());
    }
    else
    {
       
        {
            ShowToastMsg(tr("数据异常，未检测到候补机器人数据!"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Common, QPoint());
        }
    }
}

void UIYL_ManRobotSettingWidget_AlternateRobot::R_AddOrUpdateReserveRobotRelation_Result(QString code, QString reason, QString body)
{
    m_bIsResult = true;
    m_pTimer->stop();

    if (code == "200")
    {
        Json::Value val;
        bool        isOk = JsonHelper::GetJsonValueFromBody(body, val);
        if (isOk)
        {
            QString strCode = JsonHelper::GetValueByKey("code", val);
            if (strCode == "200")
            {
                ShowToastMsg(tr("保存成功！"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Common, QPoint());
            }
            else
            {
                ShowToastMsg(JsonHelper::GetValueByKey("msg", val), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Common, QPoint());
            }
        }
    }
    else
    {
        ShowToastMsg(tr("保存失败，请再次尝试!"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Common, QPoint());
    }
}

void UIYL_ManRobotSettingWidget_AlternateRobot::R_Timeout()
{
    m_pTimer->stop();
    if (!m_bIsResult)
    {
        ShowToastMsg(tr("网络超时，请重试！"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Common, QPoint());
    }
}

void UIYL_ManRobotSettingWidget_AlternateRobot::resizeEvent(QResizeEvent* event)
{
    m_pMask->setGeometry(0, 90, this->width(), this->height() - 90 - 60);
}
