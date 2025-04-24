#include "UI_Widget_CommonPhrasesCard_Panel.h"
#include "utils/ConstStringHelper.h"
#include "utils/EL_Util_Time.h"
#include "session/el_sessionservice.h"
#include "UI_Wnd_AddPersonalQuickCommonWord.h"
#include "control/CommonSignalService.h"
#include "control/IUIController.h"
#include "control/geasyliaoframework.h"
#include "service/DbUiWebcallService.h"
#include "model/UICardDataModel.h"
#include "protocol_biz/webcallprotocolservice.h"
#include "session/el_sessionservice.h"
#include "ui/uimessagebox.h"
#include "UI_Pane_ThirdTab.h"
#include "data/UIShowSettingInfo.h"
#include <QScrollBar>
#include "utils/jsonhelper.h"

UI_Widget_CommonPhrasesCard_Panel::UI_Widget_CommonPhrasesCard_Panel(QWidget* parent) : QWidget(parent)
{
    m_currTab    = E_Index_Card;
    m_currChat   = nullptr;
    m_scroll_pos = 0;
    //过渡页显示进度
    gLoginTransitionsPage->SendSchedule("65", tr("初始化右侧功能区..."));

    ET("UI_Widget_CommonPhrasesCard_Panel_setupUi");
    ui.setupUi(this);
    this->setMouseTracking(true);

    m_jshelper = nullptr;
    m_webView  = new UI_WebView_ShieldBackSpace(ui.widget_Common_files_rect);
    ui.widget_Common_files_rect_gridLayout->addWidget(m_webView);

    connect(gSessionService->LoginSignal(), SIGNAL(S_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)), this, SLOT(R_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_ShowSettingChanged()), this, SLOT(R_ShowSettingChanged()));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_ShowAiTab(int, QString)), this, SLOT(R_ShowAiTab(int, QString)));
    connect(gIProtocolAgent, SIGNAL(S_GetUserFormConfigResult(QString, QString, QString)), this, SLOT(R_GetUserFormConfigResult(QString, QString, QString)));

    InitSelf();
    InitSwitchPanel();
    InitStackedWidgetPanel();
    InitModel();


    if (CommonModelService::GetInstance()->GetClientType() == E_CLIENT_NEWMEDICAL)
    {
        ui.widget_knowledgeBase_rect->setVisible(false);
    }
    else if (CommonModelService::GetInstance()->GetClientType() == E_CLIENT_EDUEDITION)
    {
        ui.widget_knowledgeBase_rect->setVisible(true);
    }
    else
    {
        ui.widget_knowledgeBase_rect->setVisible(false);
    }
    ui.btn_newTab->setVisible(false);
    ui.btn_lemonLovesBeauty->setVisible(false);
    ui.m_rebotChecked->setChecked(false);
    ui.m_rebotPanel->setVisible(false);

    connect(ui.m_rebotChecked, SIGNAL(clicked()), this, SLOT(R_OnRebotCheckedClick()));
    connect(ui.m_right_btn, SIGNAL(clicked()), this, SLOT(R_OnRightClick()));
    connect(ui.m_left_btn, SIGNAL(clicked()), this, SLOT(R_OnLeftClick()));

    connect(ui.m_rebotList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(R_RebotItemClicked(QListWidgetItem*)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_ShowRebotList(QList<CommInfo*>&)), this, SLOT(R_ShowRebotList(QList<CommInfo*>&)));

    // CZC_个人设置保存成功，显隐“柠檬爱美”
    connect(CommonSignalService::GetInstance(), SIGNAL(S_PersonMainSettingSaveSuccess()), this, SLOT(R_PersonMainSettingSaveSuccess()));

    // CZC:个人设置-快捷键
    connect(CommonSignalService::GetInstance(), SIGNAL(S_Key_CommonWordSearch()), this, SLOT(R_Key_CommonWordSearch()));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_Key_OpenCard()), this, SLOT(R_Key_OpenCard()));

    InitPersonalQuickCommonWordPanel();
    ui.m_left_scroll->hide();

    ui.btn_ai_modify->hide();
    ui.btn_ai_build->hide();
}

UI_Widget_CommonPhrasesCard_Panel::~UI_Widget_CommonPhrasesCard_Panel()
{
}

void UI_Widget_CommonPhrasesCard_Panel::InitPersonalQuickCommonWordPanel()
{
    m_isPersonalExpand = false;
    connect(ui.btn_Add, SIGNAL(clicked()), this, SLOT(R_BtnAddPersonalQuick_Clicked()));
    connect(ui.btn_expand, SIGNAL(clicked()), this, SLOT(R_BtnExpand_Clicked()));
    R_BtnExpand_Clicked();

    connect(CommonSignalService::GetInstance(), SIGNAL(S_ModifyPersonalQuickCommonWord(MPersonalQuickCommonWordData*)), this, SLOT(R_ModifyPersonalQuickCommonWord(MPersonalQuickCommonWordData*)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_RemovePersonalQuickCommonWord(QString)), this, SLOT(R_RemovePersonalQuickCommonWord(QString)));
}

void UI_Widget_CommonPhrasesCard_Panel::InitSelf()
{
    if (CommonModelService::GetInstance()->GetClientType() == E_CLIENT_NEWMEDICAL)
    {
        connect(CommonSignalService::GetInstance(), SIGNAL(S_OnYLMainTreeSelectTabChanged(E_YL_TREE_TAB, E_YL_TREE_TAB)), this, SLOT(R_MainTreeSelectTabChanged(E_YL_TREE_TAB, E_YL_TREE_TAB)));
    }
    // else if( CommonModelService::GetInstance()->GetClientType() == E_CLIENT_EDUEDITION)
    {
        connect(CommonSignalService::GetInstance(), SIGNAL(S_OnSelectedChatItemData(MChatItemData*)), this, SLOT(R_OnSelectedChatItemData(MChatItemData*)));
    }
}

void UI_Widget_CommonPhrasesCard_Panel::InitSwitchPanel()
{
    m_extSwitchButton.append(ui.btn_commonWord);
    m_extSwitchButton.append(ui.btn_card);
    m_extSwitchButton.append(ui.btn_Common_files);
    m_extSwitchButton.append(ui.btn_lemonLovesBeauty);
    m_extSwitchButton.append(ui.btn_newTab);
    m_extSwitchButton.append(ui.btn_ai_build);
    m_extSwitchButton.append(ui.btn_ai_modify);

    InitSwitchTabButton(E_Index_Card);

    for (int i = 0; i < m_extSwitchButton.count(); i++)
    {
        connect(m_extSwitchButton.at(i), SIGNAL(clicked()), this, SLOT(R_Btn_Clicked()));
    }
}

void UI_Widget_CommonPhrasesCard_Panel::InitStackedWidgetPanel()
{
    m_currTab = E_Index_Card;
    ui.stackedWidget->setCurrentIndex(E_Index_Card);
}

void UI_Widget_CommonPhrasesCard_Panel::InitQuickCommonWordPanel()
{
    m_IsExtending = true;
    connect(ui.btn_telescopic, SIGNAL(clicked()), this, SLOT(R_BtnTelescopic_Clicked()));

    //获取底层，设置是否显示快捷常用语
    UIShowSettingInfo* info = gUIPersonalConfigYLService->ShowSettingInfo();
    if (info)
    {
        int nShow = info->ShowQuickCommonWordPanel();
        if (nShow == 0)
        {
            R_BtnTelescopic_Clicked();
        }
        delete info;
    }
    info = nullptr;
}

void UI_Widget_CommonPhrasesCard_Panel::InitModel()
{
    UICardDataModel::GetInstance()->RegisterObserver(ui.widget_card);
}


void UI_Widget_CommonPhrasesCard_Panel::InitSwitchButton(SwitchButton* btn, bool isCheck)
{
    QString unCheckStyle = "QToolButton{background-color: rgba(255, 255, 255, 204);border:0 px;font: 12px '微软雅黑';color:#000000;}QToolButton:hover,QToolButton:pressed{font-weight:bold;}";
    QString checkStyle   = "QToolButton,QToolButton:hover,QToolButton:pressed{background-color: rgba(255, 255, 255, 230);border:0 px;font:12px '微软雅黑';color:#000000;font-weight:bold;}";

    btn->SetCheckStyle(checkStyle);
    btn->SetUnCheckStyle(unCheckStyle);
    btn->IsBtnChecked(isCheck);
    btn->Init();
}

void UI_Widget_CommonPhrasesCard_Panel::InitPanelVisible()
{
    CommInfo* info = gUIPersonalConfigManager->GetUserOtherSetting();
    if (info)
    {
        if (info->GetValueByKey(ConstStringHelper::m_cUILemonLovesBeauty) == "1")
        {
            if (CommonModelService::GetInstance()->GetClientType() == E_CLIENT_NEWMEDICAL)
            {
                //非医疗版本不启用  柠檬爱美
                ui.btn_lemonLovesBeauty->setVisible(true);
            }
            else
            {
                ui.btn_lemonLovesBeauty->setVisible(false);
            }
        }
        else
        {
            ui.btn_lemonLovesBeauty->setVisible(false);
        }
    }
}

void UI_Widget_CommonPhrasesCard_Panel::SetFontBold(SwitchButton* btn, bool bold)
{
    Q_UNUSED(bold);
    QFont font = btn->font();
    font.setWeight(75);
    btn->setFont(font);
}

void UI_Widget_CommonPhrasesCard_Panel::R_BtnCommonWord_Clicked()
{
    ui.stackedWidget->setCurrentIndex(E_Index_Common);
    ui.btn_Common_files->IsBtnChecked(false);
    ui.btn_commonWord->IsBtnChecked(true);
    ui.btn_card->IsBtnChecked(false);
    ui.btn_ai_build->IsBtnChecked(false);
    ui.btn_ai_modify->IsBtnChecked(false);
    ui.btn_lemonLovesBeauty->IsBtnChecked(false);
    ui.btn_newTab->IsBtnChecked(false);

    SetFontBold(ui.btn_commonWord, true);
    SetFontBold(ui.btn_card, false);
    SetFontBold(ui.btn_ai_build, false);
    SetFontBold(ui.btn_ai_modify, false);
    SetFontBold(ui.btn_lemonLovesBeauty, false);
    SetFontBold(ui.btn_newTab, false);
    Telescopic_Show(m_IsExtending);
    Expand_Show(m_isPersonalExpand);
}

void UI_Widget_CommonPhrasesCard_Panel::R_BtnCard_Clicked()
{
    ui.stackedWidget->setCurrentIndex(E_Index_Card);
    ui.btn_Common_files->IsBtnChecked(false);
    ui.btn_card->IsBtnChecked(true);
    ui.btn_ai_build->IsBtnChecked(false);
    ui.btn_ai_modify->IsBtnChecked(false);
    ui.btn_commonWord->IsBtnChecked(false);
    ui.btn_lemonLovesBeauty->IsBtnChecked(false);
    ui.btn_newTab->IsBtnChecked(false);

    SetFontBold(ui.btn_commonWord, false);
    SetFontBold(ui.btn_card, true);
    SetFontBold(ui.btn_ai_build, false);
    SetFontBold(ui.btn_ai_modify, false);

    SetFontBold(ui.btn_lemonLovesBeauty, false);
    SetFontBold(ui.btn_newTab, false);
    Telescopic_Show(m_IsExtending);
    Expand_Show(m_isPersonalExpand);
}

void UI_Widget_CommonPhrasesCard_Panel::R_BtnLemonLovesBeauty_Clicked()
{
    ui.stackedWidget->setCurrentIndex(E_Index_lemonLovesBeauty);
    ui.btn_Common_files->IsBtnChecked(false);
    ui.btn_card->IsBtnChecked(false);
    ui.btn_ai_modify->IsBtnChecked(false);
    ui.btn_ai_build->IsBtnChecked(false);
    ui.btn_commonWord->IsBtnChecked(false);
    ui.btn_lemonLovesBeauty->IsBtnChecked(true);
    ui.btn_newTab->IsBtnChecked(false);

    SetFontBold(ui.btn_commonWord, false);
    SetFontBold(ui.btn_card, false);
    SetFontBold(ui.btn_ai_modify, false);
    SetFontBold(ui.btn_ai_build, false);
    SetFontBold(ui.btn_lemonLovesBeauty, true);
    SetFontBold(ui.btn_newTab, false);
    Telescopic_Show(m_IsExtending);
    Expand_Show(m_isPersonalExpand);
}

void UI_Widget_CommonPhrasesCard_Panel::R_BtnTelescopic_Clicked()
{
    if (m_IsExtending)
    {
        //收缩操作

        m_IsExtending = false;
    }
    else
    {
        //伸展操作
        m_IsExtending = true;
    }
    Telescopic_Show(m_IsExtending, true);
}

void UI_Widget_CommonPhrasesCard_Panel::Telescopic_Show(bool extending, bool isSaveParam)
{
    if (extending == false)
    {
        //收缩操作
        ui.widget_quickCommonWord->hide();
        ui.btn_telescopic->setStyleSheet("QPushButton{border-image: url(:/YL/Resources/YL/CommonWord/btn_quickCommonWord_popup.png);}");
        ui.widget_quickCommonWord_rect->setMinimumHeight(26);
        ui.widget_quickCommonWord_rect->setMaximumHeight(26);
        if (isSaveParam)
        {
            UIShowSettingInfo* info = gUIPersonalConfigYLService->ShowSettingInfo();
            if (info)
            {

                info->ShowQuickCommonWordPanel(0);
                gUIPersonalConfigYLService->ShowSettingInfo(info);

                delete info;
            }
            info = nullptr;
        }
    }
    else
    {
        //伸展操作

        ui.widget_quickCommonWord->show();
        ui.btn_telescopic->setStyleSheet("QPushButton{border-image: url(:/YL/Resources/YL/CommonWord/btn_quickCommonWord_pickup.png);}");
        ui.widget_quickCommonWord_rect->setMinimumHeight(197);
        ui.widget_quickCommonWord_rect->setMaximumHeight(197);
        if (isSaveParam)
        {
            UIShowSettingInfo* info = gUIPersonalConfigYLService->ShowSettingInfo();
            if (info)
            {
                info->ShowQuickCommonWordPanel(1);
                gUIPersonalConfigYLService->ShowSettingInfo(info);
                delete info;
            }
            info = nullptr;
        }
    }
}

//监听主树界面tab切换消息，当为同事对话时，设置当前选中为常用语，并且隐藏名片按钮
void UI_Widget_CommonPhrasesCard_Panel::R_MainTreeSelectTabChanged(E_YL_TREE_TAB oldType, E_YL_TREE_TAB newTypw)
{
    CommInfo* info = gUIPersonalConfigManager->GetUserOtherSetting();
    if (newTypw == E_YL_MAIN_TREE)
    {
        ui.btn_card->show();
        ui.btn_ai_build->hide();
        ui.btn_ai_modify->hide();
        if (info)
        {
            if (info->GetValueByKey(ConstStringHelper::m_cUILemonLovesBeauty) == "1")
            {
                ui.btn_lemonLovesBeauty->setVisible(true);
            }
        }
    }
    else if (newTypw == E_YL_CUSTOMER_TREE)
    {
        BtnTab_Clicked(E_Index_Common);
        ui.btn_card->hide();
        ui.btn_lemonLovesBeauty->hide();
        ui.btn_ai_build->hide();
        ui.btn_ai_modify->hide();
    }
    else if (newTypw == E_YL_RECENT_TREE)
    {
        ui.btn_card->show();
        ui.btn_ai_build->hide();
        ui.btn_ai_modify->hide();
        if (info)
        {
            if (info->GetValueByKey(ConstStringHelper::m_cUILemonLovesBeauty) == "1")
            {
                ui.btn_lemonLovesBeauty->setVisible(true);
            }
        }
    }
}

// CZC_个人设置保存成功，显隐“柠檬爱美”
void UI_Widget_CommonPhrasesCard_Panel::R_PersonMainSettingSaveSuccess()
{
    CommInfo* info = gUIPersonalConfigManager->GetUserOtherSetting();
    if (info)
    {
        if (info->GetValueByKey(ConstStringHelper::m_cUILemonLovesBeauty) == "1")
        {
            ui.btn_lemonLovesBeauty->setVisible(true);
        }
        else
        {
            if (ui.stackedWidget->currentIndex() == E_Index_lemonLovesBeauty)
            {
                BtnTab_Clicked(E_Index_Common);
            }
            ui.btn_lemonLovesBeauty->setVisible(false);
        }
        m_scroll_pos = 0;
        R_OnLeftClick();

    }
}

void UI_Widget_CommonPhrasesCard_Panel::R_Key_CommonWordSearch()
{
    //切换常用语tab
    BtnTab_Clicked(E_Index_Common);

    //设置常用语搜索框获取焦点
    ui.widget_commonWord->SetSearchEditGetFocus();
}

void UI_Widget_CommonPhrasesCard_Panel::R_Key_OpenCard()
{
    BtnTab_Clicked(E_Index_Card);
}

void UI_Widget_CommonPhrasesCard_Panel::R_OnSelectedChatItemData(MChatItemData* chatdata)
{
    if (m_currChat != chatdata)
    {

        m_currChat = chatdata;
        ui.btn_ai_modify->hide();
        ui.btn_ai_build->hide();

        m_scroll_pos = 0;
        R_OnLeftClick();

        if ((ui.stackedWidget->currentIndex() == E_Index_ai_build) || (ui.stackedWidget->currentIndex() == E_Index_ai_modify))
        {
            if ((chatdata) && ((chatdata->UserType() == E_VISITOR) || (chatdata->UserType() == E_MONITOR)))
            {
                BtnTab_Clicked(E_Index_Card);
            }
            else
                BtnTab_Clicked(E_Index_Common);
        }
    }

    if (CommonModelService::GetInstance()->GetClientType() != E_CLIENT_EDUEDITION)
    {

        return;
    }

    if ((chatdata) && ((chatdata->UserType() == E_VISITOR) || (chatdata->UserType() == E_MONITOR)))
    {
        ui.btn_card->show();

        CommInfo* info = gUIPersonalConfigManager->GetUserOtherSetting();
        if (info)
        {
            if (info->GetValueByKey(ConstStringHelper::m_cUILemonLovesBeauty) == "1")
            {
                ui.btn_lemonLovesBeauty->setVisible(true);
            }
        }
    }
    else
    {
        BtnTab_Clicked(E_Index_Common);
        ui.btn_card->hide();
        ui.btn_lemonLovesBeauty->hide();
    }
}

void UI_Widget_CommonPhrasesCard_Panel::R_OnRebotCheckedClick()
{
    if (!ui.m_rebotChecked->isChecked())
    {
        ui.m_rebotPanel->setVisible(false);
        ui.m_rebotList->clear();
    }
}

void UI_Widget_CommonPhrasesCard_Panel::R_RebotItemClicked(QListWidgetItem* item)
{
    QString answer = item->data(Qt::UserRole).toString();
    CommonSignalService::GetInstance()->DoGetRobotAnswer(answer);
}

void UI_Widget_CommonPhrasesCard_Panel::keyPressEvent(QKeyEvent* arg)
{
    if (arg->key() == Qt::Key_Return || arg->key() == Qt::Key_Enter)
    {
        if (ui.m_searchrebotEdit->hasFocus())
        {
            DoSearchRebotList();
        }
    }
    else
    {
        QWidget::keyPressEvent(arg);
    }
}

void UI_Widget_CommonPhrasesCard_Panel::DoSearchRebotList()
{
    if (ui.m_rebotChecked->isChecked())
    {
        QString keyword = ui.m_searchrebotEdit->text().trimmed();
        IUIController::GetInstance()->DoSearchRebotListByKeyWord(keyword);
    }
}

void UI_Widget_CommonPhrasesCard_Panel::R_ShowRebotList(QList<CommInfo*>& list)
{
    if (list.size() > 0)
    {
        ui.m_rebotPanel->setVisible(true);
        ui.m_rebotList->clear();
    }
    else
    {
        ui.m_rebotPanel->setVisible(false);
        ui.m_rebotList->clear();
    }

    foreach (CommInfo* info, list)
    {
        QListWidgetItem* item = new QListWidgetItem(QIcon(""), info->GetValueByKey("question"));
        item->setData(Qt::UserRole, info->GetValueByKey("answer"));
        ui.m_rebotList->addItem(item);
    }
}

void UI_Widget_CommonPhrasesCard_Panel::R_LoginStateChanged(ELOGINSTATUS oldStatus, ELOGINSTATUS newStatus)
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
            if (oldStatus == E_LOGIN_DOING)
            {
                InitPanelVisible();
                InitQuickCommonWordPanel();
            }
            break;
        case E_LOGIN_PRE_MONITORTCP:
            break;
        case E_LOGIN_ERROR:

            break;
        case E_LOGIN_ERROR_MAINTCP:
        case E_LOGIN_RETRY_MAINTCP:
            break;
        case E_LOGIN_ERROR_MONITORTCP:
        case E_LOGIN_RETRY_MONITORTCP:
            break;
        case E_LOGIN_ALLOK:
            break;
        case E_LOGIN_LOGOUT:

            break;
    }
}

void UI_Widget_CommonPhrasesCard_Panel::R_BtnAddPersonalQuick_Clicked()
{
    // todo shizhenyu

    UI_Wnd_AddPersonalQuickCommonWord* m_addQuickWnd = new UI_Wnd_AddPersonalQuickCommonWord;
    m_addQuickWnd->Modify(false);
    m_addQuickWnd->exec();
}

void UI_Widget_CommonPhrasesCard_Panel::R_BtnExpand_Clicked()
{
    if (m_isPersonalExpand)
    {
        //收缩操作
        m_isPersonalExpand = false;
    }
    else
    {
        //伸展操作
        m_isPersonalExpand = true;
    }
    Expand_Show(m_isPersonalExpand);
}

void UI_Widget_CommonPhrasesCard_Panel::Expand_Show(bool expand)
{
    if (expand == false)
    {
        //收缩操作

        ui.m_personalQuickCommonWordList->hide();
        ui.btn_expand->setStyleSheet("QPushButton{border-image: url(:/YL/Resources/YL/CommonWord/btn_quickCommonWord_popup.png);}");
        ui.PersonalQuickCommonWordWidget->setMinimumHeight(26);
        ui.PersonalQuickCommonWordWidget->setMaximumHeight(26);
    }
    else
    {
        //伸展操作

        ui.m_personalQuickCommonWordList->show();
        ui.btn_expand->setStyleSheet("QPushButton{border-image: url(:/YL/Resources/YL/CommonWord/btn_quickCommonWord_pickup.png);}");
        ui.PersonalQuickCommonWordWidget->setMinimumHeight(197);
        ui.PersonalQuickCommonWordWidget->setMaximumHeight(197);
    }
}

void UI_Widget_CommonPhrasesCard_Panel::R_ModifyPersonalQuickCommonWord(MPersonalQuickCommonWordData* data)
{
    if (data)
    {
        UI_Wnd_AddPersonalQuickCommonWord* m_addQuickWnd = new UI_Wnd_AddPersonalQuickCommonWord;
        m_addQuickWnd->Modify(true);
        m_addQuickWnd->SetPersonalQuickItemData(data);
        m_addQuickWnd->exec();
    }
}

void UI_Widget_CommonPhrasesCard_Panel::R_RemovePersonalQuickCommonWord(QString id)
{
    m_removeId        = id;
    UIMessageBox* box = new UIMessageBox;
    connect(box, SIGNAL(S_ResultOK(bool)), this, SLOT(R_RemovePersonalQuickCommonWordResultOK(bool)));
    box->setWindowTitle(tr("提示"));
    box->SetContentTip(tr("确定删除此条快捷咨询吗？"));
    box->SetCancelBtnVisible(false);
    box->exec();
}

void UI_Widget_CommonPhrasesCard_Panel::R_RemovePersonalQuickCommonWordResultOK(bool isOK)
{
    if (isOK)
    {
        gIProtocolAgent->DoDelQuickReplyCommonWord(m_removeId);
    }
}

void UI_Widget_CommonPhrasesCard_Panel::R_BtnNewTab_Clicked()
{
    ui.stackedWidget->setCurrentIndex(E_Index_newTab);
    ui.btn_Common_files->IsBtnChecked(false);
    ui.btn_card->IsBtnChecked(false);
    ui.btn_ai_build->IsBtnChecked(false);
    ui.btn_ai_modify->IsBtnChecked(false);
    ui.btn_commonWord->IsBtnChecked(false);
    ui.btn_lemonLovesBeauty->IsBtnChecked(false);
    ui.btn_newTab->IsBtnChecked(true);

    SetFontBold(ui.btn_commonWord, false);
    SetFontBold(ui.btn_card, false);
    SetFontBold(ui.btn_ai_build, false);
    SetFontBold(ui.btn_ai_modify, false);
    SetFontBold(ui.btn_lemonLovesBeauty, false);
    SetFontBold(ui.btn_newTab, true);
    Telescopic_Show(m_IsExtending);
    Expand_Show(m_isPersonalExpand);
}

void UI_Widget_CommonPhrasesCard_Panel::R_BtnAIBuild_Clicked()
{
    ui.stackedWidget->setCurrentIndex(E_Index_ai_build);
    ui.btn_Common_files->IsBtnChecked(false);
    ui.btn_card->IsBtnChecked(false);
    ui.btn_ai_build->IsBtnChecked(true);
    ui.btn_ai_modify->IsBtnChecked(false);
    ui.btn_commonWord->IsBtnChecked(false);
    ui.btn_lemonLovesBeauty->IsBtnChecked(false);
    ui.btn_newTab->IsBtnChecked(false);
    Telescopic_Show(false);
    Expand_Show(false);
}
void UI_Widget_CommonPhrasesCard_Panel::R_BtnAIModify_Clicked()
{
    ui.stackedWidget->setCurrentIndex(E_Index_ai_modify);
    ui.btn_Common_files->IsBtnChecked(false);
    ui.btn_card->IsBtnChecked(false);
    ui.btn_ai_build->IsBtnChecked(false);
    ui.btn_ai_modify->IsBtnChecked(true);
    ui.btn_commonWord->IsBtnChecked(false);
    ui.btn_lemonLovesBeauty->IsBtnChecked(false);
    ui.btn_newTab->IsBtnChecked(false);
    Telescopic_Show(false);
    Expand_Show(false);
}

void UI_Widget_CommonPhrasesCard_Panel::R_ShowSettingChanged()
{

    UIShowSettingInfo* info = gUIPersonalConfigYLService->ShowSettingInfo();
    if (info)
    {
        if (info->EnableThirdPage() == 1)
        {
            QString strTitle = info->ThirdTitle();
            ui.btn_newTab->setText(strTitle);
            ui.btn_newTab->setVisible(true);
        }
        else
        {
            if (ui.btn_newTab->IsBtnChecked())
            {
                BtnTab_Clicked(E_Index_Card);
            }
            ui.btn_newTab->setVisible(false);
        }

        m_scroll_pos = 0;
        R_OnLeftClick();


    }
}

void UI_Widget_CommonPhrasesCard_Panel::R_ShowAiTab(int type, QString val)
{
    if (type == 0)
    {
        ui.btn_ai_build->setVisible(true);
        ui.widget_selfBuildTab->InitData(val);
        BtnTab_Clicked(E_Index_ai_build);
    }
    else if (type == 1)
    {
        ui.btn_ai_modify->setVisible(true);
        ui.widget_AiModifyTab->InitData(val);
        BtnTab_Clicked(E_Index_ai_modify);
    }
}

void UI_Widget_CommonPhrasesCard_Panel::R_OnRightClick()
{
    int maxpos = ui.scrollArea->horizontalScrollBar()->maximum();

    if (m_scroll_pos < maxpos)
    {
        m_scroll_pos += 10;
        if (m_scroll_pos < maxpos)
        {

            ui.scrollArea->horizontalScrollBar()->setValue(m_scroll_pos);
            ui.m_left_scroll->show();
        }
    }
    else
    {
        ui.m_right_btn->hide();
    }

}

void UI_Widget_CommonPhrasesCard_Panel::R_OnLeftClick()
{

    int maxpos = ui.scrollArea->horizontalScrollBar()->maximum();

    if (m_scroll_pos >= maxpos && maxpos != 0)
    {
        ui.m_right_btn->hide();
    }
    else
    {
        ui.m_right_btn->show();
    }

    m_scroll_pos -= 10;
    if (m_scroll_pos <= 0)
        m_scroll_pos = 0;

    ui.scrollArea->horizontalScrollBar()->setValue(m_scroll_pos);
    if (m_scroll_pos == 0)
    {
        ui.m_left_scroll->hide();
    }

    // ui.scrollArea->horizontalScrollBar()->setValue(50);
}

void UI_Widget_CommonPhrasesCard_Panel::resizeEvent(QResizeEvent* e)
{
    m_scroll_pos = 0;
    R_OnLeftClick();
}

void UI_Widget_CommonPhrasesCard_Panel::on_btn_Common_files_clicked()
{
    if (!m_jshelper)
    {
        m_jshelper = new JsCommonfilesHelper;
        m_jshelper->SetWebView(m_webView, "Commonfilesjsobj");
        m_jshelper->SetUrl(gEasyLiaoFramework->GetHtmlNameByKey("commonFile"));
    }

    BtnTab_Clicked(E_Index_Common_files);
}

void UI_Widget_CommonPhrasesCard_Panel::InitSwitchTabButton(int tabIndex)
{
    if ((tabIndex > 0) && (tabIndex < m_extSwitchButton.count()))
    {
        for (int i = 0; i < m_extSwitchButton.count(); i++)
        {
            if (tabIndex == i)
            {
                InitSwitchButton(m_extSwitchButton.at(i), true);
            }
            else
            {
                InitSwitchButton(m_extSwitchButton.at(i), false);
            }
        }
    }
}

void UI_Widget_CommonPhrasesCard_Panel::BtnTab_Clicked(int tabIndex)
{
    if (m_currTab == E_Index_Card)
    {
        //如果从访客名片切走，同步第三方表单
        ui.widget_card->UpdateThirdFormData();
    }
    m_currTab = tabIndex;
    ui.stackedWidget->setCurrentIndex(tabIndex);

    if ((tabIndex >= 0) && (tabIndex < m_extSwitchButton.count()))
    {
        for (int i = 0; i < m_extSwitchButton.count(); i++)
        {
            if (tabIndex == i)
            {
                m_extSwitchButton.at(i)->IsBtnChecked(true);
                SetFontBold(m_extSwitchButton.at(i), true);
            }
            else
            {
                m_extSwitchButton.at(i)->IsBtnChecked(false);
                SetFontBold(m_extSwitchButton.at(i), true);
            }
        }
    }

    Telescopic_Show(m_IsExtending);
    Expand_Show(m_isPersonalExpand);
}

void UI_Widget_CommonPhrasesCard_Panel::R_Btn_Clicked()
{
    for (int i = 0; i < m_extSwitchButton.count(); i++)
    {
        if (sender() == m_extSwitchButton.at(i))
        {
            BtnTab_Clicked(i);
            break;
        }
    }
}

void UI_Widget_CommonPhrasesCard_Panel::AddNewTab(UIFormConfigInfo* info)
{
    int starti = m_extSwitchButton.size();

    QWidget* widget_newTab_rect = new QWidget();
    widget_newTab_rect->setObjectName(QString("widget_newTab_rect%1").arg(info->FormConfigId()));
    QGridLayout* gridLayout = new QGridLayout(widget_newTab_rect);
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setObjectName(QString("gridLayout%1").arg(info->FormConfigId()));
    UI_Pane_ThirdTab* widget_NewTab = new UI_Pane_ThirdTab(widget_newTab_rect);

    widget_NewTab->setObjectName(QString("widget_NewTab%1").arg(info->FormConfigId()));

    gridLayout->addWidget(widget_NewTab, 0, 0, 1, 1);

    ui.stackedWidget->addWidget(widget_newTab_rect);

    SwitchButton* btn_newTab = new SwitchButton(ui.widget_switchCommonWordCard);
    btn_newTab->setText(info->ApplyName());
    btn_newTab->setObjectName(QString("btn_newTab%1").arg(info->FormConfigId()));
    btn_newTab->setMinimumSize(QSize(80, 36));
    btn_newTab->setMaximumSize(QSize(80, 36));
    // btn_newTab->setFont(font);
    btn_newTab->setStyleSheet(QString::fromUtf8("\n"
                                                "QToolButton{\n"
                                                "border-image: url(:/Resources/Ws/btn56x36_normal.png);\n"
                                                "color: #FF000000;\n"
                                                "}\n"
                                                "QToolButton:hover{\n"
                                                "border-image: url(:/Resources/Ws/btn56x36_over.png);\n"
                                                "color:#FF000000;\n"
                                                "}\n"
                                                "QToolButton:pressed{\n"
                                                "border-image: url(:/Resources/Ws/btn56x36_over.png);\n"
                                                "color:#FF000000;\n"
                                                "}\n"));

    ui.horizontalLayout->insertWidget(starti, btn_newTab);

    InitSwitchButton(btn_newTab, false);
    m_extSwitchButton.append(btn_newTab);
    QString url = info->ApplyUrl();
    if (url.indexOf("?") > 0)
        url += "&";
    else
        url += "?";
    url += QString("appkey=%1&appsecret=%2").arg(info->ApplyKey()).arg(info->ApplySecret());
    widget_NewTab->SetUrl(url);

    connect(btn_newTab, SIGNAL(clicked()), this, SLOT(R_Btn_Clicked()));
}

void UI_Widget_CommonPhrasesCard_Panel::R_GetUserFormConfigResult(QString code, QString reason, QString body)
{
    Json::Value val;
    bool        isOk = JsonHelper::GetJsonValueFromBody(body, val);
    if (isOk)
    {
        if (val.isObject())
        {
            QString succeed = JsonHelper::GetValueByKey("code", val);
            if ((succeed == "200"))
            {
                val = val["data"];
                if ((val.isNull() == false) && (val.isArray()))
                {
                    for (int i = 0; i < (int)val.size(); i++)
                    {
                        QMap<QString, QString> tempMap;
                        bool                   isParse = JsonHelper::ParseValueToMap(val[i], tempMap);
                        if (isParse)
                        {
                            UIFormConfigInfo info;
                            info.LoadMapInfo(tempMap);
                            AddNewTab(&info);
                        }
                    }

                    m_scroll_pos = 0;
                    R_OnLeftClick();
                }
            }
        }
    }
}

