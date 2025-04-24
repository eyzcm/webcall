#include "UIYL_ManRobotSettingWidget_CountPrompt.h"
#include "UI_Widget_SystemPrompt.h"
#include "control/CommonSignalService.h"
#include "control/IUIController.h"
#include "service/DbUiWebcallService.h"
#include "model/UICommonDataModel.h"
#include "model/UIManRobotMatchRuleTipModel.h"
#include "protocol_biz/webcallprotocolservice.h"
#include "session/el_sessionservice.h"
#include "data/UIOpertorSettinginfo.h"

UIYL_ManRobotSettingWidget_CountPrompt::UIYL_ManRobotSettingWidget_CountPrompt(QWidget *parent)
    : QWidget(parent), m_strID(""), m_strRuleName(tr("机器人索联次数")), m_IsNetBroken(false), m_pMask(nullptr), m_colorDefault(255, 237, 237), m_pTimer_HidePreviewEffectWidget(nullptr), m_pWidget_PreviewEffect(nullptr)
{
    ui.setupUi(this);
	
	//QTimer::singleShot(2500, this, SLOT(R_Init()));

	
		ui.m_btn_save->hide();
		ui.m_comb_ruleState->setView(new QListView());

		//初始化执行动作
		ui.m_label_effect->installEventFilter(this);
		connect(ui.m_btn_color, SIGNAL(clicked()), this, SLOT(R_Btn_SelectColor_Clicked()));

		m_pTimer_HidePreviewEffectWidget = new QTimer(this);
		connect(m_pTimer_HidePreviewEffectWidget, SIGNAL(timeout()), this, SLOT(R_Timeout_HidePreviewEffectWidget()));

		m_pWidget_PreviewEffect = new UIYL_ManRobotRuleSetting_PreviewEffect_Widget(this);
		m_pWidget_PreviewEffect->SetWidgetType(2);
		m_pWidget_PreviewEffect->hide();
		m_pWidget_PreviewEffect->installEventFilter(this);

		//初始化保存、修改按钮
		connect(ui.m_btn_modify, SIGNAL(clicked()), this, SLOT(R_Btn_Modify_Clicked()));
		connect(ui.m_btn_save, SIGNAL(clicked()), this, SLOT(R_Btn_Save_Clicked()));

		//初始化遮罩层
		m_pMask = new QWidget(ui.widget_2_rule);
		m_pMask->setStyleSheet("background-color: rgba(255, 255, 255, 102);");
		this->ShowMask();

		//初始化网络状态监控
		connect(gSessionService->LoginSignal(), SIGNAL(S_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)), this, SLOT(R_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)));

		UIManRobotMatchRuleTipModel::GetInstance()->RegisterObserver(this);
		UIManRobotMatchRuleTipModel::GetInstance()->ReInitUserAskCountManRobotList();
	
}
UIYL_ManRobotSettingWidget_CountPrompt::~UIYL_ManRobotSettingWidget_CountPrompt()
{
    UIManRobotMatchRuleTipModel::GetInstance()->UnRegisterObserver(this);
}

void UIYL_ManRobotSettingWidget_CountPrompt::Update(QString type, QObject *obj)
{
    if (StringCompare(type, "UpdateUIUserAskCountManRobotRuleData"))
    {
        MUIManRobotRuleData *objData = (MUIManRobotRuleData *)obj;
        if (objData)
        {
            UIUserRuleInfo *info = UICommonDataModel::GetInstance()->FindUserRobotInfoById("askCount", objData->Id());
            if (info)
            {
                ClearUI();
                InitUI(info);
                m_strID = info->UserRuleId();

                delete info;
                info = nullptr;

                this->ShowMask();
            }
        }
    }
}

void UIYL_ManRobotSettingWidget_CountPrompt::RemoveUpdate(QString type, QString body)
{
}

void UIYL_ManRobotSettingWidget_CountPrompt::SetBrokenNet(int isBroken)
{
    m_IsNetBroken = isBroken;
}

void UIYL_ManRobotSettingWidget_CountPrompt::InitUI(UIUserRuleInfo *info)
{
    if (info)
    {
        m_strID = info->UserRuleId();

        //设置规则状态
        if (info->RuleState() == 1)
        {
            ui.m_comb_ruleState->setCurrentIndex(0);
        }
        else
        {
            ui.m_comb_ruleState->setCurrentIndex(1);
        }

        //设置机器人索联次数
        QString strAskCount = info->RuleValue();
        if (!strAskCount.isEmpty())
        {
            ui.m_sbox_robotContactCount->setValue(strAskCount.toInt());
        }

        //设置执行动作(声音、闪动、对话列表)
        if (info->VoiceState() == 1)
        {
            ui.m_cbox_soundPrompt->setChecked(true);
        }
        if (info->FlickerState() == 1)
        {
            ui.m_cbox_flickerPrompt->setChecked(true);
        }
        if (info->ChatListState() == 1)
        {
            ui.m_cbox_chatListPrompt->setChecked(true);
        }

        //设置预览颜色及预览窗口
        QString strColor = info->Color();
        if (!strColor.isEmpty())
        {
            m_colorDefault = QColor(strColor);

            QString strStyle = "QPushButton{border: 1px solid #DDDDDD;border-radius: 2px;background-color: " + strColor + ";}";
            ui.m_btn_color->setStyleSheet(strStyle);
            m_pWidget_PreviewEffect->SetChatItemColor(strColor);
        }
    }
}

void UIYL_ManRobotSettingWidget_CountPrompt::ClearUI()
{
    //规则状态
    ui.m_comb_ruleState->setCurrentIndex(0);

    //索联次数
    ui.m_sbox_robotContactCount->setValue(1);

    //执行动作
    ui.m_cbox_soundPrompt->setChecked(false);
    ui.m_cbox_flickerPrompt->setChecked(false);
    ui.m_cbox_chatListPrompt->setChecked(false);

    //颜色块
    ui.m_btn_color->setStyleSheet("QPushButton{border: 1px solid #DDDDDD;border-radius: 2px;background-color: #FFEDED;}");
    m_pWidget_PreviewEffect->ResetChatItemColor();
}

void UIYL_ManRobotSettingWidget_CountPrompt::ShowMask()
{
    if (m_pMask)
    {
        // m_pMask->setGeometry(ui.widget_0_content->x(), ui.widget_0_content->y(), ui.widget_0_content->width(), ui.widget_0_content->height() - 50);
        m_pMask->setGeometry(0, 20, 709, 530 - 50);
        m_pMask->show();
    }
}

void UIYL_ManRobotSettingWidget_CountPrompt::R_Btn_SelectColor_Clicked()
{
    QColor color = QColorDialog::getColor(m_colorDefault, this, tr("颜色选择："));
    if (color.isValid())
    {
        m_colorDefault = color;

        if (m_pWidget_PreviewEffect)
        {
            m_pWidget_PreviewEffect->SetChatItemColor(color);
        }

        QString strColor = QString::number(color.red()) + ", " + QString::number(color.green()) + ", " + QString::number(color.blue()) + ", " + QString::number(color.alpha());
        QString strStyle = "QPushButton{border: 0px solid #DDDDDD;border-radius: 0px;background-color: rgba(" + strColor + ");}";
        ui.m_btn_color->setStyleSheet(strStyle);
    }
}

void UIYL_ManRobotSettingWidget_CountPrompt::R_Btn_Modify_Clicked()
{
    m_pMask->hide();

    ui.m_btn_save->show();
    ui.m_btn_modify->hide();
}

void UIYL_ManRobotSettingWidget_CountPrompt::R_Btn_Save_Clicked()
{
    //校验UI数据
    if (!IsCheckNormal())
    {
        return;
    }

    SaveByModify();
}

void UIYL_ManRobotSettingWidget_CountPrompt::R_Timeout_HidePreviewEffectWidget()
{
    m_pTimer_HidePreviewEffectWidget->stop();
    m_pWidget_PreviewEffect->hide();
}

void UIYL_ManRobotSettingWidget_CountPrompt::R_LoginStateChanged(ELOGINSTATUS oldStatus, ELOGINSTATUS newStatus)
{
    switch (newStatus)
    {
        case E_LOGIN_NONE:
            break;

        case E_LOGIN_PRE:
            break;

        case E_LOGIN_DOING:
            break;

        case E_LOGIN_OK_MAINTCP:
            if (oldStatus != E_LOGIN_DOING)
            {
                m_IsNetBroken = false;
            }
            break;

        case E_LOGIN_PRE_MONITORTCP:
            break;

        case E_LOGIN_ERROR:
            if (oldStatus != E_LOGIN_DOING)
            {
                QString isAutoReply = UICommonDataModel::GetInstance()->GetSystemConfigByKey("customeraitype", "0");
                if (isAutoReply == "1")
                {
                    m_IsNetBroken = true;
                }
                else
                {
                    if (gSessionService->RetryLoginTag())
                    {
                        m_IsNetBroken = true;
                    }
                    else
                    {
                        if (gSessionService->OKLoginTag())
                        {
                            m_IsNetBroken = true;
                        }
                        else
                        {
                            m_IsNetBroken = true;
                        }
                    }
                }
            }
            break;

        case E_LOGIN_ERROR_MAINTCP:
        case E_LOGIN_RETRY_MAINTCP:
            m_IsNetBroken = true;
            break;

        case E_LOGIN_ERROR_MONITORTCP:
        case E_LOGIN_RETRY_MONITORTCP:
            break;

        case E_LOGIN_ALLOK:
            m_IsNetBroken = false;
            break;

        case E_LOGIN_LOGOUT:
            break;
    }
}

bool UIYL_ManRobotSettingWidget_CountPrompt::IsCheckNormal()
{
    //判断网络是否异常
    if (m_IsNetBroken)
    {
        ShowToastMsg(tr("网络异常/服务器超时，保存失败！"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Warning, QPoint());
        return false;
    }

    //校验索联次数
    int nCount = ui.m_sbox_robotContactCount->value();
    if ((nCount < 1) || (nCount > 200))
    {
        ShowToastMsg(tr("请输入机器人索联次数！"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Warning, QPoint());
        return false;
    }

    //校验执行动作
    if (!ui.m_cbox_soundPrompt->isChecked() && !ui.m_cbox_flickerPrompt->isChecked() && !ui.m_cbox_chatListPrompt->isChecked())
    {
        ShowToastMsg(tr("请选择执行动作！"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Warning, QPoint());
        return false;
    }

    return true;
}

void UIYL_ManRobotSettingWidget_CountPrompt::SaveByModify()
{
    UIUserRuleInfo *info = new UIUserRuleInfo();

    //保存规则信息
    info->UserRuleId(m_strID);
    info->RuleName(m_strRuleName);
    info->RuleType("askCount");
    info->RuleAlgorithm(2);
    info->RuleValue(QString::number(ui.m_sbox_robotContactCount->value()));

    int nIndex = ui.m_comb_ruleState->currentIndex();
    if (nIndex == 0)
    {
        info->RuleState(1);
    }
    else
    {
        info->RuleState(0);
    }

    //保存执行动作
    if (!ui.m_cbox_soundPrompt->isChecked())
    {
        info->VoiceState(0);
    }
    else
    {
        info->VoiceState(1);
    }

    if (!ui.m_cbox_flickerPrompt->isChecked())
    {
        info->FlickerState(0);
    }
    else
    {
        info->FlickerState(1);
    }

    if (!ui.m_cbox_chatListPrompt->isChecked())
    {
        info->ChatListState(0);
    }
    else
    {
        info->ChatListState(1);
    }

    //保存预览颜色
    QRgb    mRgb    = qRgb(m_colorDefault.red(), m_colorDefault.green(), m_colorDefault.blue());
    QString mRgbStr = "#" + QString::number(mRgb, 16).mid(2, 6);
    info->Color(mRgbStr);

    //保存到底层
    gIProtocolAgent->DoSaveUserAiRule(info);
    ShowToastMsg(tr("保存成功！"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Common, QPoint());

    //清理指针
    delete info;
    info = nullptr;

    //界面设置
    m_pMask->show();
    ui.m_btn_save->hide();
    ui.m_btn_modify->show();
}

void UIYL_ManRobotSettingWidget_CountPrompt::ShowToastMsg(QString msg, int showTime, EYLUI_ToastShowPos showPos, EYLUI_ToastType type, QPoint tarPos)
{
    UI_Widget_SystemPrompt *pWidget_SystemPtompt = new UI_Widget_SystemPrompt(this);
    pWidget_SystemPtompt->SetMsg(msg, type);
    pWidget_SystemPtompt->SetShowTime(showTime);
    pWidget_SystemPtompt->SetShowPos(this, showPos, this->mapFromGlobal(tarPos));
    pWidget_SystemPtompt->show();
}

bool UIYL_ManRobotSettingWidget_CountPrompt::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui.m_label_effect)
    {
        if (event->type() == QEvent::Enter)
        {
            int nX = ui.widget_2_rule->x() + ui.scrollAreaWidgetContents->x() + ui.widget_6_performAction->x() + ui.widget_2->x() - m_pWidget_PreviewEffect->width();
            int nY = ui.widget_2_rule->y() + ui.scrollAreaWidgetContents->y() + ui.widget_6_performAction->y() + ui.widget_2->y() - m_pWidget_PreviewEffect->height() + 127;
            m_pWidget_PreviewEffect->move(nX, nY);
            m_pWidget_PreviewEffect->show();
        }
        else if (event->type() == QEvent::Leave)
        {
            m_pTimer_HidePreviewEffectWidget->start(500);
        }
    }

    if (obj == m_pWidget_PreviewEffect)
    {
        if (event->type() == QEvent::Enter)
        {
            m_pTimer_HidePreviewEffectWidget->stop();
            m_pWidget_PreviewEffect->show();
        }
        else if (event->type() == QEvent::Leave)
        {
            m_pWidget_PreviewEffect->hide();
        }
    }

    return false;
}
