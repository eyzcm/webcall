#include <QColor>
#include <QColorDialog>
#include <QListView>
#include <QWheelEvent>

#include "UIYL_ManRobotRuleSettingWidget_KeyPrompt.h"
#include "UI_Widget_SystemPrompt.h"
#include "control/CommonSignalService.h"
#include "control/IUIController.h"
#include "service/DbUiWebcallService.h"
#include "model/UICommonDataModel.h"
#include "protocol_biz/webcallprotocolservice.h"
#include "session/el_sessionservice.h"
#include "data/UIOpertorSettinginfo.h"
#include <QEvent>
#include <QScrollBar>

UIYL_ManRobotRuleSettingWidget_KeyPrompt::UIYL_ManRobotRuleSettingWidget_KeyPrompt(QWidget *parent)
    : QWidget(parent), m_nWidgetType(1), m_strID("-1"), m_strModifyName(""), m_IsNetBroken(false), m_pMask(nullptr), m_colorDefault(255, 237, 237), m_pTimer_HidePreviewEffectWidget(nullptr), m_pWidget_PreviewEffect(nullptr)
{
    ui.setupUi(this);

    ui.m_comb_ruleState->setView(new QListView());
    ui.m_comb_containAny->setView(new QListView());

    //初始化网络状态监控
    connect(gSessionService->LoginSignal(), SIGNAL(S_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)), this, SLOT(R_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)));

    //初始化遮罩层
    m_pMask = new QWidget(ui.widget_content);
    m_pMask->setStyleSheet("background-color: rgba(255, 255, 255, 102);");
    m_pMask->hide();
    m_pMask->installEventFilter(this);

    //初始化规则名称、状态
    connect(ui.m_edit_ruleName, SIGNAL(textEdited(const QString &)), this, SLOT(R_TextEdited(const QString &)));

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

    //获取规则的数量，设置到提示框中
    int     nCount       = gUIManRobotService->GetUserRobotRuleCount("keyword");
    QString strRuleCount = tr("目前还可新增") + QString::number(100 - nCount) + tr("个");
    ui.m_label_addCount->setText(strRuleCount);

    //初始化规则列表、搜索列表item点击
    connect(CommonSignalService::GetInstance(), SIGNAL(S_ManRule_KeyWordItem_Clicked(QString)), this, SLOT(R_RuleItemClicked(QString)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_ManRule_KeyWordSearchItem_Clicked(QString)), this, SLOT(R_RuleItemClicked(QString)));

    // item删除信号
    connect(CommonSignalService::GetInstance(), SIGNAL(S_DelManRobotRule(QString)), this, SLOT(R_DelManRobotRule(QString)));
}

UIYL_ManRobotRuleSettingWidget_KeyPrompt::~UIYL_ManRobotRuleSettingWidget_KeyPrompt()
{
}

void UIYL_ManRobotRuleSettingWidget_KeyPrompt::SetWidgetType(int type)
{
    m_nWidgetType = type;
    if (m_nWidgetType == 0)
    {
        //设置新增时，控件的显隐
        ui.m_widget_1_addConut->show();
        ui.m_btn_modify->hide();
        ui.m_btn_save->show();
    }
    else
    {
        //设置修改类型时，控件的显隐
        ui.m_widget_1_addConut->hide();
        ui.m_btn_modify->show();
        ui.m_btn_save->hide();
    }
}

void UIYL_ManRobotRuleSettingWidget_KeyPrompt::SetBrokenNet(int isBroken)
{
    m_IsNetBroken = isBroken;
}

void UIYL_ManRobotRuleSettingWidget_KeyPrompt::InitUI(UIUserRuleInfo *info)
{
    if (info)
    {
        //设置规则名称和状态
        ui.m_edit_ruleName->setText(info->RuleName());
        ui.m_edit_key->setText(info->RuleValue());

        if (info->RuleState() == 1)
        {
            ui.m_comb_ruleState->setCurrentIndex(0);
        }
        else
        {
            ui.m_comb_ruleState->setCurrentIndex(1);
        }

        int nAlgorithm = info->RuleAlgorithm();
        if (nAlgorithm == 0)
        {
            ui.m_comb_containAny->setCurrentIndex(0);
        }
        else if (nAlgorithm == 1)
        {
            ui.m_comb_containAny->setCurrentIndex(1);
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

void UIYL_ManRobotRuleSettingWidget_KeyPrompt::ClearUI()
{
    //清空规则名称
    ui.m_edit_ruleName->setText("");
    ui.m_comb_ruleState->setCurrentIndex(0);

    //清理条件一：搜索关键词
    ui.m_comb_containAny->setCurrentIndex(0);
    ui.m_edit_key->setText("");

    //清理执行动作
    ui.m_cbox_soundPrompt->setChecked(false);
    ui.m_cbox_flickerPrompt->setChecked(false);
    ui.m_cbox_chatListPrompt->setChecked(false);

    ui.m_btn_color->setStyleSheet("QPushButton{border: 1px solid #DDDDDD;border-radius: 2px;background-color: #FFEDED;}");
    m_pWidget_PreviewEffect->ResetChatItemColor();
}

void UIYL_ManRobotRuleSettingWidget_KeyPrompt::ShowMask()
{
    if (m_pMask)
    {
        m_pMask->setGeometry(ui.widget_content->x(), ui.widget_content->y(), ui.widget_content->width() - 10, ui.widget_content->height());
        m_pMask->show();
        ui.m_btn_save->hide();
        ui.m_btn_modify->show();
    }
}

void UIYL_ManRobotRuleSettingWidget_KeyPrompt::R_Btn_SelectColor_Clicked()
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

void UIYL_ManRobotRuleSettingWidget_KeyPrompt::R_Btn_Modify_Clicked()
{
    m_pMask->hide();

    ui.m_btn_save->show();
    ui.m_btn_modify->hide();
}

void UIYL_ManRobotRuleSettingWidget_KeyPrompt::R_Btn_Save_Clicked()
{
    //校验UI数据
    if (!IsCheckNormal())
    {
        return;
    }

    if (m_nWidgetType == 1)
    {
        //修改类型的保存
        SaveByModify();
    }
    else
    {
        //新增类型的保存
        SaveByAdd();
    }
}

void UIYL_ManRobotRuleSettingWidget_KeyPrompt::R_Timeout_HidePreviewEffectWidget()
{
    m_pTimer_HidePreviewEffectWidget->stop();
    m_pWidget_PreviewEffect->hide();
}

void UIYL_ManRobotRuleSettingWidget_KeyPrompt::R_TextEdited(const QString &value)
{
    if (value.size() > 20)
    {
        QString str = value.mid(0, 20);
        ui.m_edit_ruleName->setText(str);
    }
}

void UIYL_ManRobotRuleSettingWidget_KeyPrompt::R_RuleItemClicked(QString id)
{
    if (m_nWidgetType == 0)
    {
        //新增窗口
        // m_pMask->hide();
        // Q_EMIT S_HideNewAddWidget();
    }
    else
    {
        //修改窗口
        if (!id.isEmpty())
        {
            UIUserRuleInfo *info = UICommonDataModel::GetInstance()->FindUserRobotInfoById("keyword", id);
            if (info)
            {
                this->show();

                ClearUI();
                InitUI(info);
                m_strID         = info->UserRuleId();
                m_strModifyName = info->RuleName();

                delete info;
                info = nullptr;

                // 0, 0, 709, 530
                // m_pMask->setGeometry(ui.widget_content->x(), ui.widget_content->y(), ui.widget_content->width() - 10, ui.widget_content->height());
                m_pMask->setGeometry(0, 0, 709 - 148 - 10, 530 - 50);
                m_pMask->show();
                m_pMask->setFocus();
                ui.m_btn_save->hide();
                ui.m_btn_modify->show();
                SetWidgetType(1);
            }
        }
        else
        {
            this->hide();
        }
    }
}

void UIYL_ManRobotRuleSettingWidget_KeyPrompt::R_DelManRobotRule(QString id)
{
    if (m_strID == id)
    {
        ClearUI();
        ShowMask();
    }
}

void UIYL_ManRobotRuleSettingWidget_KeyPrompt::R_LoginStateChanged(ELOGINSTATUS oldStatus, ELOGINSTATUS newStatus)
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

bool UIYL_ManRobotRuleSettingWidget_KeyPrompt::IsCheckNormal()
{
    //判断网络是否异常
    if (m_IsNetBroken)
    {
        ShowToastMsg(tr("网络异常/服务器超时，保存失败！"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Warning, QPoint());
        return false;
    }

    //新增
    if (m_nWidgetType == 0)
    {
        //如果规则数量达到100条，弹出Toast提示
        int nCount = gUIManRobotService->GetUserRobotRuleCount("keyword");
        if (nCount >= 100)
        {
            ShowToastMsg(tr("规则数量已达上限！"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Warning, QPoint());
            return false;
        }
    }

    //规则名称校验（空值、重复）
    QString strRuleName = ui.m_edit_ruleName->text();
    if (strRuleName.isEmpty())
    {
        ShowToastMsg(tr("请输入规则名称！"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Warning, QPoint());
        return false;
    }
    bool hasSame_Connect = gUIManRobotService->CheckRobotName(strRuleName);
    bool hasSame_KeyWord = gUIManRobotService->CheckUserRobotName("keyword", strRuleName);
    if (hasSame_Connect || hasSame_KeyWord)
    {
        if (strRuleName != m_strModifyName)
        {
            ShowToastMsg(tr("规则名称重复，请重新输入！"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Warning, QPoint());
            return false;
        }
    }

    //校验关键词
    if (ui.m_edit_key->text().isEmpty())
    {
        ShowToastMsg(tr("请输入访客对话中的关键词！"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Warning, QPoint());
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

void UIYL_ManRobotRuleSettingWidget_KeyPrompt::SaveByModify()
{
    UIUserRuleInfo *info = UICommonDataModel::GetInstance()->FindUserRobotInfoById("keyword", m_strID);
    if (info)
    {
        //保存规则信息
        info->RuleName(ui.m_edit_ruleName->text());
        info->RuleType("keyword");
        info->RuleAlgorithm(ui.m_comb_containAny->currentIndex());

        //关键词检测到中文逗号，直接替换为英文，并更新UI显示信息
        QString strKey = ui.m_edit_key->text();
        if (strKey.indexOf("，") != -1)
        {
            strKey = strKey.replace("，", ",");
        }
        info->RuleValue(strKey);
        ui.m_edit_key->setText(strKey);

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

        //设置遮罩
        if (m_nWidgetType == 0)
        {
            m_pMask->hide();
        }
        else
        {
            m_pMask->setGeometry(0, 0, 709 - 148 - 10, 530 - 50);
            m_pMask->show();
        }
        SetWidgetType(1);
    }
}

void UIYL_ManRobotRuleSettingWidget_KeyPrompt::SaveByAdd()
{
    UIUserRuleInfo *info = new UIUserRuleInfo();

    //保存规则信息
    info->UserRuleId("");
    info->RuleName(ui.m_edit_ruleName->text());
    info->RuleType("keyword");
    info->RuleAlgorithm(ui.m_comb_containAny->currentIndex());

    //关键词检测到中文逗号，直接替换为英文，并更新UI显示信息
    QString strKey = ui.m_edit_key->text();
    if (strKey.indexOf("，") != -1)
    {
        strKey = strKey.replace("，", ",");
    }
    info->RuleValue(strKey);
    ui.m_edit_key->setText(strKey);

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

    Q_EMIT S_HideWndAndDelCenterWidget();
}

void UIYL_ManRobotRuleSettingWidget_KeyPrompt::ShowToastMsg(QString msg, int showTime, EYLUI_ToastShowPos showPos, EYLUI_ToastType type, QPoint tarPos)
{
    UI_Widget_SystemPrompt *pWidget_SystemPtompt = new UI_Widget_SystemPrompt(this);
    pWidget_SystemPtompt->SetMsg(msg, type);
    pWidget_SystemPtompt->SetShowTime(showTime);
    pWidget_SystemPtompt->SetShowPos(this, showPos, this->mapFromGlobal(tarPos));
    pWidget_SystemPtompt->show();
}

bool UIYL_ManRobotRuleSettingWidget_KeyPrompt::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui.m_label_effect)
    {
        if (event->type() == QEvent::Enter)
        {
            int nX = ui.scrollAreaWidgetContents->x() + ui.widget_6_performAction->x() + ui.widget_2->x() - m_pWidget_PreviewEffect->width();
            int nY = ui.scrollAreaWidgetContents->y() + ui.widget_6_performAction->y() + ui.widget_2->y() - m_pWidget_PreviewEffect->height() + 127;
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

    if (obj == m_pMask)
    {
        if (event->type() == QEvent::Wheel)
        {
            QScrollBar *pScrollBar = ui.m_scrollArea_content->verticalScrollBar();
            if (pScrollBar)
            {
                QWheelEvent *wheel = (QWheelEvent *)event;
                if (wheel->delta() > 0)
                {
                    //手指向前
                    pScrollBar->setValue(pScrollBar->value() - 20);
                }
                else
                {
                    //手指向后
                    pScrollBar->setValue(pScrollBar->value() + 20);
                }
            }
        }
    }

    return false;
}
