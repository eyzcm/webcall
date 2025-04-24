#include <QColor>
#include <QColorDialog>
#include <QListView>
#include <QWheelEvent>

#include "UIYL_ManRobotRuleSettingWidget.h"
#include "UI_Widget_SystemPrompt.h"
#include "control/CommonSignalService.h"
#include "control/IUIController.h"
#include "service/DbUiWebcallService.h"
#include "protocol_biz/webcallprotocolservice.h"
#include "session/el_sessionservice.h"
#include "data/UIOpertorSettinginfo.h"
#include <QEvent>
#include <QScrollBar>
#include "model/UICommonDataModel.h"

UIYL_ManRobotRuleSettingWidget::UIYL_ManRobotRuleSettingWidget(QWidget *parent)
    : QWidget(parent), m_nWidgetType(0), m_strID("-1"), m_strModifyName(""), m_IsNetBroken(0), m_pMask(nullptr), m_vecUrl(QVector<UIYL_ManRobotRuleSettingUrlWidget *>()), m_colorDefault(255, 237, 237), m_pTimer_HidePreviewEffectWidget(nullptr), m_pWidget_PreviewEffect(nullptr)
{
    ui.setupUi(this);

    ui.m_comb_ruleState->setView(new QListView());
    ui.m_comb_containMatch->setView(new QListView());
    ui.m_comb_containAny->setView(new QListView());
    ui.m_comb_visitorNoSayTime->setView(new QListView());

    //初始化网络状态监控
    connect(gSessionService->LoginSignal(), SIGNAL(S_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)), this, SLOT(R_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)));

    //初始化遮罩层
    m_pMask = new QWidget(ui.widget_content);
    m_pMask->setStyleSheet("background-color: rgba(255, 255, 255, 102);");
    m_pMask->hide();
    m_pMask->installEventFilter(this);

    //初始化规则名称、状态
    connect(ui.m_edit_ruleName, SIGNAL(textEdited(const QString &)), this, SLOT(R_TextEdited(const QString &)));

    //初始化条件一：访问URL
    ui.widget_url_condition->hide();
    connect(ui.m_cbox_visitUrl, SIGNAL(stateChanged(int)), this, SLOT(R_StateChanged_CBox_VisitUrl(int)));

    m_vecUrl.append(ui.m_widget_first);
    connect(ui.m_widget_first, SIGNAL(S_AddUrl(UIYL_ManRobotRuleSettingUrlWidget *)), this, SLOT(R_AddUrl(UIYL_ManRobotRuleSettingUrlWidget *)));
    connect(ui.m_widget_first, SIGNAL(S_DelUrl(UIYL_ManRobotRuleSettingUrlWidget *)), this, SLOT(R_DelUrl(UIYL_ManRobotRuleSettingUrlWidget *)));

    //初始化条件一：关系
    ui.m_rbtn_or->setChecked(true);

    //初始化条件一：搜索关键词隐藏
    ui.widget_search_condition->hide();
    connect(ui.m_cbox_searchKey, SIGNAL(stateChanged(int)), this, SLOT(R_StateChanged_CBox_SearchKey(int)));

    //初始化条件二：访客为说话时间隐藏
    ui.m_label_visitorNoSayTime->hide();
    ui.m_sbox_visitorNoSayTime->hide();
    ui.m_comb_visitorNoSayTime->hide();
    connect(ui.m_cbox_visitorNoSayTime, SIGNAL(stateChanged(int)), this, SLOT(R_StateChanged_CBox_VisitorNoSayTime(int)));

    //初始化条件三：访客发消息数隐藏
    ui.m_sbox_visitorSendMsgCount->hide();
    connect(ui.m_cbox_visitorSendMsgCount, SIGNAL(stateChanged(int)), this, SLOT(R_StateChanged_CBox_VisitorSendMsgCount(int)));

    //初始化执行动作
    ui.m_label_effect->installEventFilter(this);
    connect(ui.m_btn_color, SIGNAL(clicked()), this, SLOT(R_Btn_SelectColor_Clicked()));

    m_pTimer_HidePreviewEffectWidget = new QTimer(this);
    connect(m_pTimer_HidePreviewEffectWidget, SIGNAL(timeout()), this, SLOT(R_Timeout_HidePreviewEffectWidget()));

    m_pWidget_PreviewEffect = new UIYL_ManRobotRuleSetting_PreviewEffect_Widget(this);
    m_pWidget_PreviewEffect->hide();
    m_pWidget_PreviewEffect->installEventFilter(this);

    //初始化保存、修改按钮
    SetWidgetType(0);
    connect(ui.m_btn_modify, SIGNAL(clicked()), this, SLOT(R_Btn_Modify_Clicked()));
    connect(ui.m_btn_save, SIGNAL(clicked()), this, SLOT(R_Btn_Save_Clicked()));

    //获取规则的数量，设置到提示框中
    int     nCount       = gUIManRobotService->GetSelfRobotRuleCount();
    QString strRuleCount = tr("目前还可新增") + QString::number(100 - nCount) + tr("个");
    ui.m_label_addCount->setText(strRuleCount);

    //初始化规则列表、搜索列表item点击
    connect(CommonSignalService::GetInstance(), SIGNAL(S_OnClickManRobotRule(QString)), this, SLOT(R_RuleItemClicked(QString)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_OnClickManRobotRuleSearchItem(QString)), this, SLOT(R_RuleItemClicked(QString)));

    // item删除信号
    connect(CommonSignalService::GetInstance(), SIGNAL(S_DelManRobotRule(QString)), this, SLOT(R_DelManRobotRule(QString)));
}

UIYL_ManRobotRuleSettingWidget::~UIYL_ManRobotRuleSettingWidget()
{
}

void UIYL_ManRobotRuleSettingWidget::SetWidgetType(int type)
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

void UIYL_ManRobotRuleSettingWidget::SetBrokenNet(int isBroken)
{
    m_IsNetBroken = isBroken;
}

void UIYL_ManRobotRuleSettingWidget::InitUI(UIManRobotInfo *info)
{
    if (info)
    {
        //设置规则名称和状态
        ui.m_edit_ruleName->setText(info->RuleName());
        if (info->State() == 1)
        {
            ui.m_comb_ruleState->setCurrentIndex(0);
        }
        else
        {
            ui.m_comb_ruleState->setCurrentIndex(1);
        }

        //设置条件
        UIConditionRobotInfo *condition = info->Condition();
        if (condition)
        {
            //设置条件一
            SetConditionOne(condition->ConditionOne());

            //设置条件二
            SetConditionTwo(condition->ConditionTwo());

            //设置条件三
            SetConditionThree(condition->ConditionThree());
        }

        //设置执行动作
        SetAction(info->Action());
    }
}

void UIYL_ManRobotRuleSettingWidget::SetConditionOne(UIConditionOneInfo *conditionOne)
{
    if (conditionOne)
    {
        //设置条件一：访问Url
        int nVisitUrl = conditionOne->UrlState();
        if (nVisitUrl == 1)
        {
            //开启访问Url开关
            ui.m_cbox_visitUrl->setChecked(true);

            //设置包含匹配下拉选项
            QString strOption = conditionOne->UrlOption();
            if (strOption == "chatUrl")
            {
                ui.m_comb_containMatch->setCurrentIndex(0);
            }
            else if (strOption == "firstUrl")
            {
                ui.m_comb_containMatch->setCurrentIndex(1);
            }

            //添加url面板
            QStringList listUrl = conditionOne->UrlValue();
            for (int i = 0; i < listUrl.size(); ++i)
            {
                if (i == 0)
                {
                    m_vecUrl.at(0)->SetUrl(listUrl.at(i));
                }
                else
                {
                    AddUrl(listUrl.at(i));
                }
                m_vecUrl.at(i)->HideBtnAdd();
            }
            m_vecUrl.at(m_vecUrl.size() - 1)->ShowBtnAdd();
        }

        //设置条件一：关系
        int nRelation = conditionOne->Relation();
        if (nRelation == 0)
        {
            ui.m_rbtn_or->setChecked(true);
        }
        else if (nRelation == 1)
        {
            ui.m_rbtn_and->setChecked(true);
        }

        //设置条件一：搜索关键词
        int nSearchKey = conditionOne->KeyWordState();
        if (nSearchKey == 1)
        {
            //开启搜索关键词开关
            ui.m_cbox_searchKey->setChecked(true);

            //设置包含状态
            int nContain = conditionOne->KeyWordOption();
            if (nContain == 0)
            {
                ui.m_comb_containAny->setCurrentIndex(0);
            }
            else if (nContain == 1)
            {
                ui.m_comb_containAny->setCurrentIndex(1);
            }

            //设置关键词
            QStringList strListKeyWord = conditionOne->KeyWordValue();
            QString     strKeyWord     = strListKeyWord.join(",");
            ui.m_edit_key->setText(strKeyWord);
        }
    }
}

void UIYL_ManRobotRuleSettingWidget::SetConditionTwo(UIConditionTwoInfo *conditionTwo)
{
    if (conditionTwo)
    {
        int nState = conditionTwo->VisitorState();
        if (nState == 1)
        {
            //设置访客未说话时间开关
            ui.m_cbox_visitorNoSayTime->setChecked(true);

            //设置超时时间
            int nTimeout = conditionTwo->TimeOut();
            ui.m_sbox_visitorNoSayTime->setValue(nTimeout);

            //设置时间类型
            int nTimeType = conditionTwo->TimeType();
            if (nTimeType == 0)
            {
                ui.m_comb_visitorNoSayTime->setCurrentIndex(0);
            }
            else if (nTimeType == 1)
            {
                ui.m_comb_visitorNoSayTime->setCurrentIndex(1);
            }
        }
    }
}

void UIYL_ManRobotRuleSettingWidget::SetConditionThree(UIConditionThreeInfo *conditionThree)
{
    if (conditionThree)
    {
        int nState = conditionThree->VisitorMsgState();
        if (nState == 1)
        {
            //设置开关
            ui.m_cbox_visitorSendMsgCount->setChecked(true);

            //设置消息数
            ui.m_sbox_visitorSendMsgCount->setValue(conditionThree->Count());
        }
    }
}

void UIYL_ManRobotRuleSettingWidget::SetAction(UIActionRobotInfo *action)
{
    if (action)
    {
        //设置声音提醒
        int nVoiceState = action->VoiceState();
        if (nVoiceState == 1)
        {
            ui.m_cbox_soundPrompt->setChecked(true);
        }

        //设置闪动提醒
        int nFlickerState = action->FlickerState();
        if (nFlickerState == 1)
        {
            ui.m_cbox_flickerPrompt->setChecked(true);
        }

        //设置对话列表提醒
        int nChatListState = action->ChatListState();
        if (nChatListState == 1)
        {
            ui.m_cbox_chatListPrompt->setChecked(true);
        }

        //设置预览颜色及预览窗口
        QString strColor = action->Color();
        if (!strColor.isEmpty())
        {
            m_colorDefault = QColor(strColor);

            QString strStyle = "QPushButton{border: 1px solid #DDDDDD;border-radius: 2px;background-color: " + strColor + ";}";
            ui.m_btn_color->setStyleSheet(strStyle);
            m_pWidget_PreviewEffect->SetChatItemColor(strColor);
        }
    }
}

void UIYL_ManRobotRuleSettingWidget::ClearUI()
{
    //清空规则名称
    ui.m_edit_ruleName->setText("");
    ui.m_comb_ruleState->setCurrentIndex(0);

    //清理条件一：访问Url
    ClearUrl();
    AddUrl();
    ui.m_comb_containMatch->setCurrentIndex(0);
    ui.m_cbox_visitUrl->setChecked(false);

    //清理条件一：关系
    ui.m_rbtn_or->setChecked(true);

    //清理条件一：搜索关键词
    ui.m_comb_containAny->setCurrentIndex(0);
    ui.m_edit_key->setText("");
    ui.m_cbox_searchKey->setChecked(false);

    //清理条件二
    ui.m_sbox_visitorNoSayTime->setValue(1);
    ui.m_comb_visitorNoSayTime->setCurrentIndex(0);
    ui.m_cbox_visitorNoSayTime->setChecked(false);

    //清理条件三
    ui.m_sbox_visitorSendMsgCount->setValue(1);
    ui.m_cbox_visitorSendMsgCount->setChecked(false);

    //清理执行动作
    ui.m_cbox_soundPrompt->setChecked(false);
    ui.m_cbox_flickerPrompt->setChecked(false);
    ui.m_cbox_chatListPrompt->setChecked(false);

    ui.m_btn_color->setStyleSheet("QPushButton{border: 1px solid #DDDDDD;border-radius: 2px;background-color: #FFEDED;}");
    m_pWidget_PreviewEffect->ResetChatItemColor();
}

void UIYL_ManRobotRuleSettingWidget::ShowMask()
{
    if (m_pMask)
    {
        m_pMask->setGeometry(ui.widget_content->x(), ui.widget_content->y(), ui.widget_content->width() - 10, ui.widget_content->height());
        m_pMask->show();
    }
}

void UIYL_ManRobotRuleSettingWidget::R_StateChanged_CBox_VisitUrl(int state)
{
    if (state == 0)
    {
        ui.widget_url_condition->hide();
    }
    else if (state == 2)
    {
        ui.widget_url_condition->show();
    }
}

void UIYL_ManRobotRuleSettingWidget::R_StateChanged_CBox_SearchKey(int state)
{
    if (state == 0)
    {
        ui.widget_search_condition->hide();
    }
    else if (state == 2)
    {
        ui.widget_search_condition->show();
    }
}

void UIYL_ManRobotRuleSettingWidget::R_StateChanged_CBox_VisitorNoSayTime(int state)
{
    if (state == 0)
    {
        ui.m_label_visitorNoSayTime->hide();
        ui.m_sbox_visitorNoSayTime->hide();
        ui.m_comb_visitorNoSayTime->hide();
    }
    else if (state == 2)
    {
        ui.m_label_visitorNoSayTime->show();
        ui.m_sbox_visitorNoSayTime->show();
        ui.m_comb_visitorNoSayTime->show();
    }
}

void UIYL_ManRobotRuleSettingWidget::R_StateChanged_CBox_VisitorSendMsgCount(int state)
{
    if (state == 0)
    {
        ui.m_sbox_visitorSendMsgCount->hide();
    }
    else if (state == 2)
    {
        ui.m_sbox_visitorSendMsgCount->show();
    }
}

void UIYL_ManRobotRuleSettingWidget::R_AddUrl(UIYL_ManRobotRuleSettingUrlWidget *widget)
{
    if (widget)
    {
        if (m_vecUrl.size() < 100)
        {
            widget->HideBtnAdd();
            AddUrl();
        }
        else
        {
            ShowToastMsg("URL数量超过100条！", 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Warning, QPoint());
        }
    }
}

void UIYL_ManRobotRuleSettingWidget::R_DelUrl(UIYL_ManRobotRuleSettingUrlWidget *widget)
{
    if (widget && m_vecUrl.size() > 1)
    {
        //判断此面板是否为最后一个
        int nSize = m_vecUrl.size();
        if (widget == m_vecUrl.at(nSize - 1))
        {
            UIYL_ManRobotRuleSettingUrlWidget *temp = m_vecUrl.at(nSize - 2);
            if (temp)
            {
                temp->ShowBtnAdd();
            }
        }

        //从布局中移除此面板
        ui.m_vLayout_Url->removeWidget(widget);

        //如果当前只存在一个面板，则隐藏删除按钮
        if (m_vecUrl.size() == 1)
        {
            UIYL_ManRobotRuleSettingUrlWidget *temp = m_vecUrl.at(0);
            if (temp)
            {
                temp->ShowBtnAdd();
                temp->HideBtnDel();
            }
        }

        //从内存中移除此面板
        int nIndex = -1;
        for (int i = 0; i < m_vecUrl.size(); ++i)
        {
            if (widget == m_vecUrl.at(i))
            {
                nIndex = i;
                break;
            }
        }

        if (nIndex != -1)
        {
            m_vecUrl.remove(nIndex);
        }

        delete widget;
        widget = nullptr;
    }
}

void UIYL_ManRobotRuleSettingWidget::R_Btn_SelectColor_Clicked()
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

void UIYL_ManRobotRuleSettingWidget::R_Btn_Modify_Clicked()
{
    m_pMask->hide();

    ui.m_btn_save->show();
    ui.m_btn_modify->hide();
}

void UIYL_ManRobotRuleSettingWidget::R_Btn_Save_Clicked()
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

void UIYL_ManRobotRuleSettingWidget::R_Timeout_HidePreviewEffectWidget()
{
    m_pTimer_HidePreviewEffectWidget->stop();
    m_pWidget_PreviewEffect->hide();
}

void UIYL_ManRobotRuleSettingWidget::R_TextEdited(const QString &value)
{
    if (value.size() > 20)
    {
        QString str = value.mid(0, 20);
        ui.m_edit_ruleName->setText(str);
    }
}

void UIYL_ManRobotRuleSettingWidget::R_RuleItemClicked(QString id)
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
            UIManRobotInfo *info = gUIManRobotService->FindRobotInfoById(id);
            if (info)
            {
                ClearUI();
                InitUI(info);
                m_strID         = info->Id();
                m_strModifyName = info->RuleName();

                delete info;
                info = nullptr;

                this->show();

                // 0, 0, 709, 530
                // m_pMask->setGeometry(ui.widget_content->x(), ui.widget_content->y(), ui.widget_content->width() - 10, ui.widget_content->height());
                m_pMask->setGeometry(0, 0, 709 - 148 - 10, 530 - 50);
                m_pMask->show();
                m_pMask->setFocus();
                SetWidgetType(1);
            }
        }
        else
        {
            this->hide();
        }
    }
}

void UIYL_ManRobotRuleSettingWidget::R_DelManRobotRule(QString id)
{
    if (m_strID == id)
    {
        ClearUI();
        ShowMask();
    }
}

void UIYL_ManRobotRuleSettingWidget::R_LoginStateChanged(ELOGINSTATUS oldStatus, ELOGINSTATUS newStatus)
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

void UIYL_ManRobotRuleSettingWidget::AddUrl(QString url)
{
    UIYL_ManRobotRuleSettingUrlWidget *newWidget = new UIYL_ManRobotRuleSettingUrlWidget();
    newWidget->SetUrl(url);
    connect(newWidget, SIGNAL(S_AddUrl(UIYL_ManRobotRuleSettingUrlWidget *)), this, SLOT(R_AddUrl(UIYL_ManRobotRuleSettingUrlWidget *)));
    connect(newWidget, SIGNAL(S_DelUrl(UIYL_ManRobotRuleSettingUrlWidget *)), this, SLOT(R_DelUrl(UIYL_ManRobotRuleSettingUrlWidget *)));

    ui.m_vLayout_Url->addWidget(newWidget);
    m_vecUrl.append(newWidget);
}

void UIYL_ManRobotRuleSettingWidget::ClearUrl()
{
    for (int i = m_vecUrl.size() - 1; i > -1; --i)
    {
        UIYL_ManRobotRuleSettingUrlWidget *widget = m_vecUrl.at(i);
        if (widget)
        {
            ui.m_vLayout_Url->removeWidget(widget);
            m_vecUrl.remove(i);

            delete widget;
            widget = nullptr;
        }
    }
}

bool UIYL_ManRobotRuleSettingWidget::IsCheckNormal()
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
        int nCount = gUIManRobotService->GetSelfRobotRuleCount();
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

    //条件校验
    bool noChecked_One = true;
    if (ui.m_cbox_visitUrl->isChecked() || ui.m_cbox_searchKey->isChecked())
    {
        noChecked_One = false;
    }

    if (noChecked_One && !ui.m_cbox_visitorNoSayTime->isChecked() && !ui.m_cbox_visitorSendMsgCount->isChecked())
    {
        ShowToastMsg(tr("必须选择一个条件！"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Warning, QPoint());
        return false;
    }

    //校验条件一
    if (ui.m_cbox_visitUrl->isChecked())
    {
        for (int i = 0; i < m_vecUrl.size(); ++i)
        {
            if (m_vecUrl.at(i)->IsEmpty())
            {
                ShowToastMsg(tr("条件一的URL没有填写！"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Warning, QPoint());
                return false;
            }
        }
    }

    if (ui.m_cbox_searchKey->isChecked())
    {
        if (ui.m_edit_key->text().isEmpty())
        {
            ShowToastMsg(tr("条件一的关键词没有填写！"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Warning, QPoint());
            return false;
        }
    }

    //校验执行动作
    if (!ui.m_cbox_soundPrompt->isChecked() && !ui.m_cbox_flickerPrompt->isChecked() && !ui.m_cbox_chatListPrompt->isChecked())
    {
        ShowToastMsg(tr("请选择执行动作！"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Warning, QPoint());
        return false;
    }

    return true;
}

void UIYL_ManRobotRuleSettingWidget::SaveByModify()
{
    UIManRobotInfo *info = gUIManRobotService->FindRobotInfoById(m_strID);
    if (info)
    {
        //保存规则名称、状态
        info->RuleName(ui.m_edit_ruleName->text());

        int nIndex = ui.m_comb_ruleState->currentIndex();
        if (nIndex == 0)
        {
            info->State(1);
        }
        else
        {
            info->State(0);
        }

        //保存条件
        UIConditionRobotInfo *condition = new UIConditionRobotInfo(info->Condition());
        if (condition)
        {
            //保存条件一
            UIConditionOneInfo *conditionOne = condition->ConditionOne();
            if (conditionOne)
            {
                //保存访问Url
                if (!ui.m_cbox_visitUrl->isChecked())
                {
                    conditionOne->UrlState(0);
                }
                else
                {
                    conditionOne->UrlState(1);

                    if (ui.m_comb_containMatch->currentIndex() == 0)
                    {
                        conditionOne->UrlOption("chatUrl");
                    }
                    else
                    {
                        conditionOne->UrlOption("firstUrl");
                    }

                    QStringList urlList;
                    for (int i = 0; i < m_vecUrl.size(); ++i)
                    {
                        urlList.append(m_vecUrl.at(i)->GetUrl());
                    }
                    conditionOne->UrlValue(urlList);
                }

                //保存关系
                if (ui.m_rbtn_or->isChecked())
                {
                    conditionOne->Relation(0);
                }
                else
                {
                    conditionOne->Relation(1);
                }

                //保存搜索关键词
                if (!ui.m_cbox_searchKey->isChecked())
                {
                    conditionOne->KeyWordState(0);
                }
                else
                {
                    conditionOne->KeyWordState(1);
                    conditionOne->KeyWordOption(ui.m_comb_containAny->currentIndex());

                    //关键词检测到中文逗号，直接替换为英文，并更新UI显示信息
                    QString strKeys = ui.m_edit_key->text();
                    if (strKeys.indexOf("，") != -1)
                    {
                        strKeys = strKeys.replace("，", ",");
                    }
                    ui.m_edit_key->setText(strKeys);
                    QStringList KeyList = strKeys.split(",");
                    conditionOne->KeyWordValue(KeyList);
                }

                condition->ConditionOne(conditionOne);
            }

            //保存条件二
            UIConditionTwoInfo *conditionTwo = condition->ConditionTwo();
            if (conditionTwo)
            {
                if (!ui.m_cbox_visitorNoSayTime->isChecked())
                {
                    conditionTwo->VisitorState(0);
                }
                else
                {
                    conditionTwo->VisitorState(1);
                    conditionTwo->TimeOut(ui.m_sbox_visitorNoSayTime->value());
                    conditionTwo->TimeType(ui.m_comb_visitorNoSayTime->currentIndex());
                }

                condition->ConditionTwo(conditionTwo);
            }

            //保存条件三
            UIConditionThreeInfo *conditionThree = condition->ConditionThree();
            if (conditionThree)
            {
                if (!ui.m_cbox_visitorSendMsgCount->isChecked())
                {
                    conditionThree->VisitorMsgState(0);
                }
                else
                {
                    conditionThree->VisitorMsgState(1);
                    conditionThree->Count(ui.m_sbox_visitorSendMsgCount->value());
                }

                condition->ConditionThree(conditionThree);
            }
        }

        info->Condition(condition);
        //保存执行动作
        UIActionRobotInfo *action = new UIActionRobotInfo(info->Action());
        if (action)
        {
            if (!ui.m_cbox_soundPrompt->isChecked())
            {
                action->VoiceState(0);
            }
            else
            {
                action->VoiceState(1);
            }

            if (!ui.m_cbox_flickerPrompt->isChecked())
            {
                action->FlickerState(0);
            }
            else
            {
                action->FlickerState(1);
            }

            if (!ui.m_cbox_chatListPrompt->isChecked())
            {
                action->ChatListState(0);
            }
            else
            {
                action->ChatListState(1);
            }

            QRgb    mRgb    = qRgb(m_colorDefault.red(), m_colorDefault.green(), m_colorDefault.blue());
            QString mRgbStr = "#" + QString::number(mRgb, 16).mid(2, 6);
            action->Color(mRgbStr);
        }
        info->Action(action);
        //保存到底层（因为直接将此指针添加到底层，所以指针不能在这里删除）
        gIProtocolAgent->DoUpdateAiRule(info);

        ShowToastMsg(tr("保存成功！"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Common, QPoint());

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

void UIYL_ManRobotRuleSettingWidget::SaveByAdd()
{
    UIManRobotInfo *info = new UIManRobotInfo();

    //保存规则名称、状态
    info->RuleName(ui.m_edit_ruleName->text());
    int nIndex = ui.m_comb_ruleState->currentIndex();
    if (nIndex == 0)
    {
        info->State(1);
    }
    else
    {
        info->State(0);
    }

    //保存条件
    {
        UIConditionRobotInfo *condition = new UIConditionRobotInfo();

        //保存条件一
        {
            UIConditionOneInfo *conditionOne = new UIConditionOneInfo();

            //访问Url
            if (!ui.m_cbox_visitUrl->isChecked())
            {
                conditionOne->UrlState(0);
            }
            else
            {
                conditionOne->UrlState(1);

                if (ui.m_comb_containMatch->currentIndex() == 0)
                {
                    conditionOne->UrlOption("chatUrl");
                }
                else
                {
                    conditionOne->UrlOption("firstUrl");
                }

                QStringList urlList;
                for (int i = 0; i < m_vecUrl.size(); ++i)
                {
                    urlList.append(m_vecUrl.at(i)->GetUrl());
                }
                conditionOne->UrlValue(urlList);
            }

            //关系
            if (ui.m_rbtn_or->isChecked())
            {
                conditionOne->Relation(0);
            }
            else
            {
                conditionOne->Relation(1);
            }

            //搜索关键词
            if (!ui.m_cbox_searchKey->isChecked())
            {
                conditionOne->KeyWordState(0);
            }
            else
            {
                conditionOne->KeyWordState(1);
                conditionOne->KeyWordOption(ui.m_comb_containAny->currentIndex());

                //关键词检测到中文逗号，直接替换为英文，并更新UI显示信息
                QString strKeys = ui.m_edit_key->text();
                if (strKeys.indexOf("，") != -1)
                {
                    strKeys = strKeys.replace("，", ",");
                }
                ui.m_edit_key->setText(strKeys);
                QStringList KeyList = strKeys.split(",");
                conditionOne->KeyWordValue(KeyList);
            }

            condition->ConditionOne(conditionOne);
        }

        //保存条件二
        {
            UIConditionTwoInfo *conditionTwo = new UIConditionTwoInfo();
            if (!ui.m_cbox_visitorNoSayTime->isChecked())
            {
                conditionTwo->VisitorState(0);
            }
            else
            {
                conditionTwo->VisitorState(1);
                conditionTwo->TimeOut(ui.m_sbox_visitorNoSayTime->value());
                conditionTwo->TimeType(ui.m_comb_visitorNoSayTime->currentIndex());
            }

            condition->ConditionTwo(conditionTwo);
        }

        //保存条件三
        {
            UIConditionThreeInfo *conditionThree = new UIConditionThreeInfo();
            if (!ui.m_cbox_visitorSendMsgCount->isChecked())
            {
                conditionThree->VisitorMsgState(0);
            }
            else
            {
                conditionThree->VisitorMsgState(1);
                conditionThree->Count(ui.m_sbox_visitorSendMsgCount->value());
            }

            condition->ConditionThree(conditionThree);
        }

        info->Condition(condition);
    }

    //保存执行动作
    {
        UIActionRobotInfo *action = new UIActionRobotInfo();

        if (!ui.m_cbox_soundPrompt->isChecked())
        {
            action->VoiceState(0);
        }
        else
        {
            action->VoiceState(1);
        }

        if (!ui.m_cbox_flickerPrompt->isChecked())
        {
            action->FlickerState(0);
        }
        else
        {
            action->FlickerState(1);
        }

        if (!ui.m_cbox_chatListPrompt->isChecked())
        {
            action->ChatListState(0);
        }
        else
        {
            action->ChatListState(1);
        }

        QRgb    mRgb    = qRgb(m_colorDefault.red(), m_colorDefault.green(), m_colorDefault.blue());
        QString mRgbStr = "#" + QString::number(mRgb, 16).mid(2, 6);
        action->Color(mRgbStr);

        info->Action(action);
    }

    //保存到底层（底层直接使用该指针，因此这里不能删除指针）
    // gUIManRobotService->AddManRobotInfo(info);

    gIProtocolAgent->DoAddAiRule(info->RuleName(), info->Condition()->ToJson().toStyledString().c_str(), info->Action()->ToJson().toStyledString().c_str(), info->State());

    Q_EMIT S_HideWndAndDelCenterWidget();
}

void UIYL_ManRobotRuleSettingWidget::ShowToastMsg(QString msg, int showTime, EYLUI_ToastShowPos showPos, EYLUI_ToastType type, QPoint tarPos)
{
    UI_Widget_SystemPrompt *pWidget_SystemPtompt = new UI_Widget_SystemPrompt(this);
    pWidget_SystemPtompt->SetMsg(msg, type);
    pWidget_SystemPtompt->SetShowTime(showTime);
    pWidget_SystemPtompt->SetShowPos(this, showPos, this->mapFromGlobal(tarPos));
    pWidget_SystemPtompt->show();
}

bool UIYL_ManRobotRuleSettingWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui.m_label_effect)
    {
        if (event->type() == QEvent::Enter)
        {
            int nX = ui.scrollAreaWidgetContents->x() + ui.widget_6_performAction->x() + ui.m_label_effect->x() + ui.m_label_effect->width() / 2 - m_pWidget_PreviewEffect->width() + 50;
            int nY = ui.scrollAreaWidgetContents->y() + ui.widget_6_performAction->y() + ui.m_label_effect->y() - m_pWidget_PreviewEffect->height();
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
