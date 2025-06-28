#include "ui_widget_mainother.h"

#include "service/viewuiservice.h"
#include "ui/common/UtilsViewHelper.h"
#include <el_common_popup_ui.h>
#include "lefttree/ui/yl_ui/UIYL_TreeWidget.h"
#include "chatui/ui/UIYL_Centerwidget.h"


UI_Widget_MainOther::UI_Widget_MainOther()
{
    m_pAdvancedFilter = nullptr;
    m_assistantWnd             = nullptr;
    m_Announcement_Notice      = nullptr;
    m_Announcement_Details     = nullptr;

    connect(CommonSignalService::GetInstance(), &CommonSignalService::S_ShowAnnouncementDetails, this, &UI_Widget_MainWindow::R_ShowAnnouncement_Details);
    connect(CommonSignalService::GetInstance(), &CommonSignalService::S_ShowAnnouncementNotice, this, &UI_Widget_MainWindow::R_ShowAnnouncement_Notice);
}

void UI_Widget_MainWindow::ShowAdvancedFilterVisitorWnd()
{
    if (m_pAdvancedFilter == nullptr)
    {
        m_pAdvancedFilter = new UI_Wnd_AdvancedFilterVisitor;
    }
    UI_Model_AdvancedFilterVisitor::GetInstance()->InitData();
    m_pAdvancedFilter->showNormal();
    m_pAdvancedFilter->activateWindow();
}



void UI_Widget_MainWindow::InitMainWindow()
{
	connect(CommonSignalService::GetInstance(), SIGNAL(S_OnSendClpboardImage(MChatItemData *)), this, SLOT(R_OnSendClpboardImage(MChatItemData *)));

    //


}

void UI_Widget_MainWindow::R_OnRedEnvelopesClick()
{
    UI_Panel_RedEnvelopes *pRedEnvelopesWnd = new UI_Panel_RedEnvelopes;
    pRedEnvelopesWnd->InitData();

    BaseMainWnd *pRedEnvelopes = new BaseMainWnd;
    pRedEnvelopes->AddContentWidget(pRedEnvelopesWnd);
    pRedEnvelopes->HideMaxBtn();
    pRedEnvelopes->HideMinBtn();
    pRedEnvelopes->SetWndTitle(tr("发送柠檬红包"));
    pRedEnvelopes->SetWndIcon(":/YL/Resources/YL/LemonLovesBeauty/title_icon_RedEnvelopes.png");
    pRedEnvelopes->SetTitlePanelIcon(":/YL/Resources/YL/LemonLovesBeauty/title_icon_RedEnvelopes.png");
    pRedEnvelopes->setFixedSize(480, 480);
    pRedEnvelopes->SetDeleteOnClose(false);
    pRedEnvelopes->ShowCenter();
    pRedEnvelopes->activateWindow();
}


void UI_Widget_MainWindow::InitPopupPanenl()
{

    // CZC_柠檬爱美
    connect(CommonSignalService::GetInstance(), SIGNAL(S_OnRedEnvelopesClick()), this, SLOT(R_OnRedEnvelopesClick()));
}


void UI_Widget_MainWindow::DoAssistantWnd()
{
    IBaseMainWnd *wnd = MainWnd();
    if (wnd)
    {
        if (m_assistantWnd == nullptr)
        {
            m_assistantWnd = new UI_AssistantWnd(wnd);

            connect(m_assistantWnd, SIGNAL(S_CloseWidget_Assistant()), this, SLOT(R_CloseWidget_Assistant()));
        }
        m_assistantWnd->resize(wnd->size().width(), wnd->size().height());
        m_assistantWnd->ShowWnd();
    }
}


void UI_Widget_MainWindow::R_CloseWidget_Assistant()
{
    if (m_assistantWnd)
    {
        m_assistantWnd->hide();
    }
}


void UI_Widget_MainWindow::R_ShowVisitorToolTip(bool show, bool rightOff, YL_MainTreeLeafNodeItemData *nodeLeaf)
{
    if (show)
    {

        int x = m_widget_dialogue_list_panel->x() + m_widget_dialogue_list_panel->width();
        int y = this->mapFromGlobal(QCursor::pos()).y();
        if (y + m_pTreeToolTip->height() > this->height())
        {
            y = this->height() - m_pTreeToolTip->height();
        }
        m_pTreeToolTip->InitData(nodeLeaf);
        m_pTreeToolTip->setGeometry(x, y, m_pTreeToolTip->width(), m_pTreeToolTip->height());
        m_pTreeToolTip->ShowWindow();

    }
    else
    {
        if (rightOff)
        {
            m_pTreeToolTip->HideWindow_RightOff();
        }
        else
        {
            m_pTreeToolTip->HideWindow();
        }
    }
}


void UI_Widget_MainWindow::InitMainWindow()
{

    connect(CommonSignalService::GetInstance(), SIGNAL(S_OnBeginMainTreeActionClicked(QString, YL_MainTreeItem *)), this, SLOT(R_OnMainTreeActionClicked(QString, YL_MainTreeItem *)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_OnBeginRecentTreeActionClicked(QString, YL_MainTreeItem *)), this, SLOT(R_OnRecentTreeActionClicked(QString, YL_MainTreeItem *)));



    // CZC:主树和历史树弹出toolTip窗口
    m_pTreeToolTip = new UI_MainTreeToolTip(this);
    m_pTreeToolTip->hide();
    connect(CommonSignalService::GetInstance(), SIGNAL(S_ShowVisitorToolTip(bool, bool, YL_MainTreeLeafNodeItemData *)), this, SLOT(R_ShowVisitorToolTip(bool, bool, YL_MainTreeLeafNodeItemData *)));



    //


}


void UI_Widget_MainWindow::resizeEvent(QResizeEvent *event)
{

    if (m_Announcement_Notice && !m_Announcement_Notice->isHidden()) //若公告通知UI为显示状态,则更新其位置
    {
        ShowAnnouncement_Notice();
    }


}


void UI_Widget_MainWindow::ShowAnnouncement_Notice()//显示公告通知页面
{
    if (m_Announcement_Notice)
    {
        IBaseMainWnd *wnd = this->MainWnd();
        if (wnd)
        {
            QPoint mp = wnd->mapFromGlobal(m_pPanenl_AuxiliaryFun->getNoticepos());
            QRect  wr = wnd->getWorkarea();
            m_Announcement_Notice->move(wr.width() - m_Announcement_Notice->width() + wnd->getmargin(), mp.y());
            m_Announcement_Notice->Xposition(m_Announcement_Notice->width() - (wr.width() - mp.x()) + wnd->getmargin());
            m_Announcement_Notice->show();
        }
    }
}

void UI_Widget_MainWindow::R_ShowAnnouncement_Notice(QString title, QString content, QString time)
{
    if (title.isEmpty() && content.isEmpty() && time.isEmpty()) //公告通知内容为空时,不弹窗提示
    {
        return;
    }
    ShowAnnouncement_Notice();
    if (m_Announcement_Notice)
    {
        m_Announcement_Notice->showAnnouncement_Notice(title, content, time);
    }
}

void UI_Widget_MainWindow::R_ShowAnnouncement_Details()
{
    if (m_Announcement_Details)
    {
        Q_EMIT m_Announcement_Details->S_Closed();
        m_Announcement_Details->close();
        m_Announcement_Details->deleteLater();
        m_Announcement_Details = nullptr;
    }
    BaseMainWnd *pBaseWnd = new BaseMainWnd;
    pBaseWnd->setFixedSize(820, 700);
    m_Announcement_Details = new EL_Announcement_Details(1);
    pBaseWnd->AddContentWidget(m_Announcement_Details);
    pBaseWnd->SetCloseType(6);
    pBaseWnd->HideMaxBtn();
    pBaseWnd->HideMinBtn();
    pBaseWnd->SetWndTitle(tr("系统通知"));
    pBaseWnd->ShowCenter();
}


void UI_Widget_MainWindow::R_ShowNoticePanel()
{
    UI_CommPanel_Notice *wnd         = new UI_CommPanel_Notice();
    BaseMainWnd         *noticePanel = new BaseMainWnd;
    noticePanel->setFixedSize(810, 580);
    noticePanel->SetWndTitle(tr("易聊助手"));
    noticePanel->SetWndIcon(":/CommIcon/Resources/CommIcon/icon_notice.png");
    noticePanel->SetTitlePanelIcon(":/CommIcon/Resources/CommIcon/icon_notice.png");
    noticePanel->HideMaxBtn();
    noticePanel->HideMinBtn();
    noticePanel->SetCloseType(5);
    noticePanel->AddContentWidget(wnd);
    noticePanel->ShowCenter_Model();
}
void UI_Widget_MainWindow::R_BtnCloseNotice_Clicked()
{
    m_pNoticePromptWidget->move(this->width() - 275, 1);
    m_pNoticePromptWidget->show();
}


void UI_Widget_MainWindow::R_SendTable(QString chatId, QString groupId)
{
    UI_CommPanel_SendTable *wnd = new UI_CommPanel_SendTable();

    BaseMainWnd *sendTablePanel = new BaseMainWnd;
    sendTablePanel->setFixedSize(440, 420);
    sendTablePanel->SetWndTitle(tr("信息表单"));
    sendTablePanel->SetWndIcon(":/Resources/Image/easy.ico");
    sendTablePanel->SetTitlePanelIcon(":/Resources/Image/easy.ico");
    sendTablePanel->HideMaxBtn();
    sendTablePanel->HideMinBtn();
    sendTablePanel->SetCloseType(1);
    sendTablePanel->AddContentWidget(wnd);
    sendTablePanel->ShowCenter_Model();
    // sendTablePanel->ShowCenter();
}
void UI_Widget_MainWindow::R_SendTable_PromptNoCheck()
{
    UI_Widget_SystemPrompt *pWidget_SystemPtompt = new UI_Widget_SystemPrompt(this);
    pWidget_SystemPtompt->SetMsg(tr("未勾选任何项"), Enum_YL_Toast_Type_Warning);
    pWidget_SystemPtompt->SetShowTime(2);
    int nX = this->width() / 2 - pWidget_SystemPtompt->width() / 2;
    int nY = this->height() / 2 - pWidget_SystemPtompt->height() / 2 - 300;
    pWidget_SystemPtompt->move(nX, nY);
    pWidget_SystemPtompt->show();
}



void UI_Widget_MainWindow::DoShowAllocationWnd(QString id, QString groupId)
{
    BaseMainWnd *pBaseWnd = new BaseMainWnd;

    UIAllocationChatWnd *wnd = new UIAllocationChatWnd();
    connect(wnd, SIGNAL(S_AllocationChat(const QString &, const QString &, const QString &)), this, SLOT(R_OnAllocationChat(const QString &, const QString &, const QString &)));
    wnd->Init(id, groupId);

    pBaseWnd->AddContentWidget(wnd);
    pBaseWnd->HideMaxBtn();
    pBaseWnd->HideMinBtn();
    pBaseWnd->SetWndTitle(tr("分配对话"));
    pBaseWnd->SetModal();
    pBaseWnd->ShowCenter();
}

void UI_Widget_MainWindow::R_OnAllocationChat(const QString &vid, const QString &groupId, const QString &userid)
{
    UIContactInfo *info = UICommonDataModel::GetInstance()->GetColleagueMemberByID(userid);
    if (info)
    {
        ConversionController::GetInstance()->DoAllocationChat(vid, groupId, userid);
    }
    else
    {
        UIMessageBox *box = new UIMessageBox;
        box->setWindowTitle(tr("提示"));
        box->SetContentTip(tr("请选择分配的客服。"));
        box->SetCancelBtnVisible(false);
        box->exec();
    }
}


void UI_Widget_MainWindow::R_ShowChatTransferWidget(YL_MainTreeLeafNodeItemData *nodeLeaf)
{
    UI_Widget_ChatTransfer *wnd           = new UI_Widget_ChatTransfer();
    BaseMainWnd            *pChatTransfer = new BaseMainWnd();

    pChatTransfer->SetTitlePanelIcon(":/Resources/Image/easy.ico");
    pChatTransfer->setWindowIcon(QIcon(":/Resources/Image/easy.ico"));
    pChatTransfer->SetWndTitle(tr("对话转接提示窗口"));
    pChatTransfer->SetDragEnable(false);
    pChatTransfer->SetMoveEnable(true);
    pChatTransfer->AddContentWidget(wnd);

    wnd->InitData(nodeLeaf);
    pChatTransfer->ShowCenter();
}

// CZC：发送表单
void UI_Widget_MainWindow::InitSendTable()
{
    connect(CommonSignalService::GetInstance(), SIGNAL(S_SendTable(QString, QString)), this, SLOT(R_SendTable(QString, QString)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_SendTable_PromptNoCheck()), this, SLOT(R_SendTable_PromptNoCheck()));
}


void UI_Widget_MainWindow::R_Assistant_Clicked()
{
    this->DoAssistantWnd();
}

void UI_Widget_MainWindow::R_ChangeSkin_Clicked(int type)
{
    if (type == 1)
    {
        if (m_nPanel_ChangeBackcolor == nullptr)
            m_nPanel_ChangeBackcolor = new UIBackgroundColorWidget(this);
        m_nPanel_ChangeBackcolor->show();
        m_nPanel_ChangeBackcolor->setFocus();
    }
    else
    {
        if (!m_pPanel_ChangeSkin)
        {
            m_pPanel_ChangeSkin = new UI_Panel_ChangeSkin(this);
        }
        QPoint a  = mapToGlobal(this->rect().topRight());
        int    nW = m_pPanel_ChangeSkin->width();
        int    nH = m_pPanel_ChangeSkin->height();
        m_pPanel_ChangeSkin->setGeometry(a.x() - 375, a.y() - 30, nW, nH);
        m_pPanel_ChangeSkin->show();
        m_pPanel_ChangeSkin->setFocus();
        m_pPanel_ChangeSkin->activateWindow();
    }
}

void UI_Widget_MainWindow::ShowSendImageWnd(MChatItemData *chatItem)
{
    UI_SendImage *pSendImage = new UI_SendImage;
    if (pSendImage->LoadBoardData())
    {
        pSendImage->SetChatItem(chatItem);

        BaseMainWnd *pBaseWnd = new BaseMainWnd;
        pBaseWnd->setAttribute(Qt::WA_DeleteOnClose);
        pBaseWnd->AddContentWidget(pSendImage);
        pBaseWnd->SetWndTitle(tr("截图预览窗口"));
        pBaseWnd->HideMinBtn();
        pBaseWnd->SetModal();
        pBaseWnd->SetDragEnable(true);
        pBaseWnd->SetMoveEnable(true);
        pBaseWnd->ShowCenter();
    }
}


void UI_Widget_MainWindow::OnShieldVisitorWnd(QString vid, QString chatid, QString ip, QString ipv6, QString chatType)
{
    UIShieldVisitorWnd *wnd = new UIShieldVisitorWnd;
    connect(wnd, SIGNAL(S_ShieldVisitorFinished(const QString &, const QString &, const QString &, const QString &, const QString &, const QString &, int, const QString &, const QString &)), this,
                                                                SLOT(R_ShieldVisitorFinished(const QString &, const QString &, const QString &, const QString &, const QString &, const QString &, int, const QString &, const QString &)));

    wnd->Init(vid, ip, ipv6, chatid, chatType);
    gIProtocolAgent->DoUploadOperatorLog("online_client_conversation_27");
    BaseMainWnd *pBaseWnd = new BaseMainWnd;
    pBaseWnd->AddContentWidget(wnd);
    pBaseWnd->HideMaxBtn();
    pBaseWnd->HideMinBtn();
    pBaseWnd->SetWndTitle(tr("屏蔽访客"));
    pBaseWnd->SetModal();
    pBaseWnd->ShowCenter();
}

void UI_Widget_MainWindow::R_ShieldVisitorFinished(const QString &id, const QString &ip, const QString &type, const QString &expires, const QString &note, const QString &chatId, int flag, const QString &scope, const QString &groupId)
{

    ConversionController::GetInstance()->DoShieldVisitor(id, ip, type, expires, note, chatId, flag, scope, groupId);
    UICommUtilsHelper::ShowCommonBox(tr("提示"), tr("您已屏蔽该访客。"));
}

void UI_Widget_MainWindow::ShowChatSummaryWnd(QString chatId, QString Vid)
{
    EL_Common_Popup_UI *box = new EL_Common_Popup_UI;
    connect(box, SIGNAL(S_Result(bool)), this, SLOT(R_SummaryResult(bool)));
    box->ChatSummary(chatId, Vid);
}


void UI_Widget_MainWindow::R_TransferChat(const QString &vid, const QString &userid, const QString &chatId, bool hasHistory, const QString &summary)
{
    ConversionController::GetInstance()->DoTransChat(vid, userid, chatId, hasHistory, summary);
    gIProtocolAgent->DoUploadOperatorLog("online_client_conversation_06");
}


void UI_Widget_MainWindow::InitExtendBtnWidget()
{
    m_Announcement_Notice  = new EL_Announcement_Notice;
    m_Announcement_Details = new EL_Announcement_Details;

    IBaseMainWnd *wnd = MainWnd();
    if (wnd)
    {
        wnd->AddExtendBtnWidget(m_pPanenl_AuxiliaryFun, true);
        m_Announcement_Notice->setParent(wnd);
        m_Announcement_Notice->hide();
        //启动首次获取未读消息数
    }
    else
    {
        Q_ASSERT(false);
    }
}


void UI_Widget_MainWindow::R_OnMainTreeActionClicked(QString key, YL_MainTreeItem *item)
{

    if (key.indexOf("modifyCard", 0, Qt::CaseInsensitive) != -1)
    {
        //编辑名片, 打开名片tab页面
        m_widget_commonPhrases_card_panel->BtnTab_Clicked(E_Index_Card);
    }
    else if (key.indexOf("summaryTagChat", 0, Qt::CaseInsensitive) != -1)
    {
        //总结标签
        if (item)
        {
            YL_MainTreeLeafNodeItemData *itemLeafNode = (YL_MainTreeLeafNodeItemData *)item->Ptr();
            if (itemLeafNode)
            {
                ShowChatSummaryWnd(itemLeafNode->Chatid(), itemLeafNode->Vid());
            }
        }
    }
    else if (key.indexOf("shieldVisitor", 0, Qt::CaseInsensitive) != -1)
    {
        //屏蔽访客
        if (item)
        {
            YL_MainTreeLeafNodeItemData *itemLeafNode = (YL_MainTreeLeafNodeItemData *)item->Ptr();
            if (itemLeafNode)
            {
                QString chatid = itemLeafNode->Chatid();
                OnShieldVisitorWnd(itemLeafNode->Userid(), chatid, itemLeafNode->GetExtendData("ip"), itemLeafNode->GetExtendData("baidu_ipv6"), itemLeafNode->SearchingType());
            }
        }
    }
    else if (key.indexOf("assignChat", 0, Qt::CaseInsensitive) != -1)
    {
        //分配对话
        if (item)
        {
            YL_MainTreeLeafNodeItemData *itemLeafNode = (YL_MainTreeLeafNodeItemData *)item->Ptr();
            if (itemLeafNode)
            {
                DoShowAllocationWnd(itemLeafNode->Vid(), itemLeafNode->GetExtendData("GroupId"));
            }
        }

    }
    else if (key.indexOf("advancedFilterSetting", 0, Qt::CaseInsensitive) != -1)
    {
        //高级筛选访客
        ShowAdvancedFilterVisitorWnd();
    }
}


void UI_Widget_MainWindow::R_OnRecentTreeActionClicked(QString key, YL_MainTreeItem *item)
{
    if (key.indexOf("modifyCard", 0, Qt::CaseInsensitive) != -1)
    {
        //编辑名片, 打开名片tab页面
        m_widget_commonPhrases_card_panel->BtnTab_Clicked(E_Index_Card);
    }
    else if (key.indexOf("summaryTagChat", 0, Qt::CaseInsensitive) != -1)
    {
        //总结标签
        if (item)
        {
            YL_MainTreeLeafNodeItemData *itemLeafNode = (YL_MainTreeLeafNodeItemData *)item->Ptr();
            if (itemLeafNode)
            {
                ShowChatSummaryWnd(itemLeafNode->Chatid(), itemLeafNode->Vid());
            }
        }
    }
    else if (key.indexOf("shieldVisitor", 0, Qt::CaseInsensitive) != -1)
    {
        //屏蔽访客
        if (item)
        {
            YL_MainTreeLeafNodeItemData *itemLeafNode = (YL_MainTreeLeafNodeItemData *)item->Ptr();
            if (itemLeafNode)
            {

                QString chatid = itemLeafNode->Chatid();
                OnShieldVisitorWnd(itemLeafNode->Userid(), chatid, itemLeafNode->GetExtendData("ip"), itemLeafNode->GetExtendData("baidu_ipv6"), itemLeafNode->SearchingType());
            }
        }
    }
}


void UI_Widget_MainWindow::R_OnSendClpboardImage(MChatItemData *chatItem)
{
    ShowSendImageWnd(chatItem);
}


// CZC:转接提示弹窗
void UI_Widget_MainWindow::InitChatTransferWidget()
{
    connect(CommonSignalService::GetInstance(), SIGNAL(S_ShowChatTransferWidget(YL_MainTreeLeafNodeItemData *)), this, SLOT(R_ShowChatTransferWidget(YL_MainTreeLeafNodeItemData *)));
}


void UI_Widget_MainWindow::R_SendTable_PromptNoCheck()
{
    UI_Widget_SystemPrompt *pWidget_SystemPtompt = new UI_Widget_SystemPrompt(this);
    pWidget_SystemPtompt->SetMsg(tr("未勾选任何项"), Enum_YL_Toast_Type_Warning);
    pWidget_SystemPtompt->SetShowTime(2);
    int nX = this->width() / 2 - pWidget_SystemPtompt->width() / 2;
    int nY = this->height() / 2 - pWidget_SystemPtompt->height() / 2 - 300;
    pWidget_SystemPtompt->move(nX, nY);
    pWidget_SystemPtompt->show();
}



void UI_Widget_MainWindow::R_ChangeSkin_Clicked(int type)
{
    if (type == 1)
    {
        if (m_nPanel_ChangeBackcolor == nullptr)
            m_nPanel_ChangeBackcolor = new UIBackgroundColorWidget(this);
        m_nPanel_ChangeBackcolor->show();
        m_nPanel_ChangeBackcolor->setFocus();
    }
    else
    {
        if (!m_pPanel_ChangeSkin)
        {
            m_pPanel_ChangeSkin = new UI_Panel_ChangeSkin(this);
        }
        QPoint a  = mapToGlobal(this->rect().topRight());
        int    nW = m_pPanel_ChangeSkin->width();
        int    nH = m_pPanel_ChangeSkin->height();
        m_pPanel_ChangeSkin->setGeometry(a.x() - 375, a.y() - 30, nW, nH);
        m_pPanel_ChangeSkin->show();
        m_pPanel_ChangeSkin->setFocus();
        m_pPanel_ChangeSkin->activateWindow();
    }
}


void UI_Widget_MainWindow::DoAddWaitQueueInfo(UIWaitVisitorInfo *pinfo)
{
    if (pinfo)
    {
        DoSystemTipProcess("waitQueueVisitor");
        if (m_queueTipTimerList.find(pinfo->ViewVid()) == m_queueTipTimerList.end())
        {
            QString strValue  = CommonModelService::GetInstance()->GetSystemConfigByKey("queuetimetip", "60");
            QTimer *tmepTimer = new QTimer(this);
            tmepTimer->start(strValue.toInt() * 1000);
            tmepTimer->setSingleShot(true);
            connect(tmepTimer, SIGNAL(timeout()), this, SLOT(R_SendQueueTimeOutTip()));
            m_queueTipTimerList.insert(pinfo->ViewVid(), tmepTimer);
        }
    }
}



void UI_Widget_MainWindow::DoAddVisitorInfo(UIVisitorInfo *pinfo)
{
    DoSystemTipProcess("visitOnline");
}


