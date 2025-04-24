#include "UIEachBotMainWnd.h"
#include "UI_Widget_SystemPrompt.h"
#include "control/CommonSignalService.h"
#include "session/el_sessionservice.h"

UIEachBotMainWnd::UIEachBotMainWnd(QWidget* parent) : QWidget(parent)
{
    ui.setupUi(this);
    ui.stackedWidget->setCurrentIndex(0);

    InitSwitchButton(ui.m_robotSettingBtn, true);
    InitSwitchButton(ui.m_manRobotSettingBtn, false);
    InitSwitchButton(ui.m_btn_KeyPrompt, false);
    InitSwitchButton(ui.m_btn_CountPrompt, false);
    

    connect(ui.m_manRobotSettingBtn, SIGNAL(clicked()), this, SLOT(R_OnManRobotSettingBtnClick()));
    connect(ui.m_robotSettingBtn, SIGNAL(clicked()), this, SLOT(R_OnRobotSettingBtnClick()));
    connect(ui.m_btn_KeyPrompt, SIGNAL(clicked()), this, SLOT(R_Btn_KeyPrompt_Clicked()));
    connect(ui.m_btn_CountPrompt, SIGNAL(clicked()), this, SLOT(R_Btn_CountPrompt_Clicked()));
    

    connect(CommonSignalService::GetInstance(), SIGNAL(S_ShowToastMsg_AISettingCenter(QString, int, EYLUI_ToastShowPos, EYLUI_ToastType, QPoint)), this, SLOT(R_ShowToastMsg_AISettingCenter(QString, int, EYLUI_ToastShowPos, EYLUI_ToastType, QPoint)));

    
}

UIEachBotMainWnd::~UIEachBotMainWnd()
{
}

void UIEachBotMainWnd::R_OnRobotSettingBtnClick()
{
    ui.stackedWidget->setCurrentIndex(0);

    ui.m_robotSettingBtn->IsBtnChecked(true);
    ui.m_manRobotSettingBtn->IsBtnChecked(false);
    ui.m_btn_KeyPrompt->IsBtnChecked(false);
    ui.m_btn_CountPrompt->IsBtnChecked(false);
    
}

void UIEachBotMainWnd::R_OnManRobotSettingBtnClick()
{
    ui.stackedWidget->setCurrentIndex(1);

    ui.m_robotSettingBtn->IsBtnChecked(false);
    ui.m_manRobotSettingBtn->IsBtnChecked(true);
    ui.m_btn_KeyPrompt->IsBtnChecked(false);
    ui.m_btn_CountPrompt->IsBtnChecked(false);
    
}

void UIEachBotMainWnd::R_Btn_KeyPrompt_Clicked()
{
    ui.stackedWidget->setCurrentIndex(2);

    ui.m_robotSettingBtn->IsBtnChecked(false);
    ui.m_manRobotSettingBtn->IsBtnChecked(false);
    ui.m_btn_KeyPrompt->IsBtnChecked(true);
    ui.m_btn_CountPrompt->IsBtnChecked(false);
    
}

void UIEachBotMainWnd::R_Btn_CountPrompt_Clicked()
{
    ui.stackedWidget->setCurrentIndex(3);

    ui.m_robotSettingBtn->IsBtnChecked(false);
    ui.m_manRobotSettingBtn->IsBtnChecked(false);
    ui.m_btn_KeyPrompt->IsBtnChecked(false);
    ui.m_btn_CountPrompt->IsBtnChecked(true);
    
}

void UIEachBotMainWnd::R_Btn_AlternateRobot_Clicked()
{
    ui.stackedWidget->setCurrentIndex(4);

    ui.m_robotSettingBtn->IsBtnChecked(false);
    ui.m_manRobotSettingBtn->IsBtnChecked(false);
    ui.m_btn_KeyPrompt->IsBtnChecked(false);
    ui.m_btn_CountPrompt->IsBtnChecked(false);
    
}

void UIEachBotMainWnd::R_ShowToastMsg_AISettingCenter(QString msg, int showTime, EYLUI_ToastShowPos showPos, EYLUI_ToastType type, QPoint tarPos)
{
    UI_Widget_SystemPrompt* pWidget_SystemPtompt = new UI_Widget_SystemPrompt(this);
    pWidget_SystemPtompt->SetMsg(msg, type);
    pWidget_SystemPtompt->SetShowTime(showTime);
    pWidget_SystemPtompt->SetShowPos(this, showPos, this->mapFromGlobal(tarPos));
    pWidget_SystemPtompt->show();
}

void UIEachBotMainWnd::InitSwitchButton(SwitchButton* btn, bool isCheck)
{
    QString unCheckStyle = "QToolButton{border-image:url(:/Resources/Image/toolbtn_normal.png);padding-top:10px;padding-bottom:10px;color: #ffffff;}	\
                                                    QToolButton:hover,QToolButton:pressed{background:#464d51;}";
    QString checkStyle   = "QToolButton,QToolButton:hover,QToolButton:pressed{border-image:url(:/Resources/Image/toolbtn_normal.png);		\
                                                  padding-top:10px;background:#464d51;padding-bottom:10px;color: #ffffff;}";

    btn->SetCheckStyle(checkStyle);
    btn->SetUnCheckStyle(unCheckStyle);
    btn->IsBtnChecked(isCheck);
    btn->Init();
}
