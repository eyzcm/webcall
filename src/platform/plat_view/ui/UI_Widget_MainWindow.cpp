#include "stdafx.h"
#include "UI_Widget_MainWindow.h"
#ifdef _MSC_VER
#include <Windows.h>
#else
#define UINT unsigned int
#endif

#include <QShortcut>
#include <QTimer>
#include "ui/tools/UI_Wnd_OutputLog.h"



#include "utils/ConstStringHelper.h"
#include "utils/EL_Util_Time.h"
#include "ui/MyIconStyle.h"



//#include "control/UIMainController.h"
#include "data/CommInfo.h"

#include "protocol/httpcmd.h"
#include "session/el_sessionservice.h"
#include "ui/Common/baseMainWnd.h"
#include "utils/utilshelper.h"
#include "QDesktopServices.h"
#include "uimessagebox.h"
#include "service/util_service.h"
#include "server/platuiservice.h"
#include "ui/common/UI_NoticeBubblePrompt.h"
#include "uiyl/tree/UIYL_TreeWidget.h"
#include "data/uimainwndsettinginfo.h"
#include "ui_UI_Widget_MainWindow.h"


UI_Widget_MainWindow::UI_Widget_MainWindow(QWidget *parent) : EasyLiaoMainWnd(parent)
{
    ui = new Ui::UI_Widget_MainWindowClass;
    ui->setupUi(this);
    m_initConfig               = 0;
    //m_nPanel_ChangeBackcolor   = nullptr;
    m_skinKey                  = "";
    m_shotcutCommonWord        = nullptr;
    m_shotcutKeyChatChange     = nullptr;
    m_shotcutKeyOpenCard       = nullptr;
    m_shotcutKeyCloseChat      = nullptr;

    m_flash                    = false;
    m_pTimer_SplitterMovedOver = nullptr;
    m_pPanel_ChangeSkin        = nullptr;
    m_pWidget_SystemPtompt     = nullptr;

    connect(gSessionService->LoginSignal(), SIGNAL(S_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)), this, SLOT(R_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)));
    connect(PlatUiService::GetInstance(), SIGNAL(S_LoadUi(int )), this, SLOT(R_LoadUi(int )));
   /* {


//
        m_widget_dialogue_list_panel = new UIYL_TreeWidget(this);
        ui->widget_dialogue_list_panel_rect_Layout->addWidget(m_widget_dialogue_list_panel);


		

    }
    */
    {
        ET("UI_Widget_MainWindow_InitMainWindow");
        InitMainWindow();
    }
    {
        ET("UI_Widget_MainWindow_Other");
        InitUserInfoPanel();
        InitAuxiliaryFunPanel();
        InitCommonPhrasesCardPanel();
        InitCompanyInfoStatisticsPanel();

        InitShowOrHideCommonWordAndCardPanel();


        InitShowCustomHoverPanel();
        InitNoticePanel();

        InitPanelScale();
    }
    {
        InitShotCut();
    }

    m_timer = new QTimer;
    m_timer->setInterval(500);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(R_OnTimer()));
    ui->widget_dialogue_list_panel_rect_Layout->setContentsMargins(0, 0, 0, 0);

    connect(CommonSignalService::Instance(), SIGNAL(S_ShowSettingChanged()), this, SLOT(R_ShowSettingChanged()));
    connect(CommonSignalService::Instance(), SIGNAL(S_ShowIeWndByUri(EURITYPE, int, QString)), this, SLOT(R_ShowIeWndByUri(EURITYPE, int, QString)));
    connect(CommonSignalService::Instance(), SIGNAL(S_SetAiPanelScale()), this, SLOT(R_SetAiPanelScale()));


#ifdef  _DEBUG
        UI_Wnd_OutputLog::GetInstance();
#endif //  _DEBUG


}

UI_Widget_MainWindow::~UI_Widget_MainWindow()
{
    //UI_Model_Statistics::GetInstance()->UnRegisterObserver(m_widget_statistics_panel);
}

void UI_Widget_MainWindow::InitMainWindow()
{


    connect(CommonSignalService::Instance(), SIGNAL(S_VisitorMsgNoReplySelf_Sound()), this, SLOT(R_VisitorMsgNoReplySelf_Sound()));
    connect(CommonSignalService::Instance(), SIGNAL(S_VisitorMsgNoReplyMonitor_Sound()), this, SLOT(R_VisitorMsgNoReplyMonitor_Sound()));
    connect(CommonSignalService::Instance(), SIGNAL(S_OpenPicture(QString)), this, SLOT(R_OpenPicture(QString)));


    connect(CommonSignalService::Instance(), SIGNAL(S_ShowToastMsg(QString, int, EYLUI_ToastShowPos, EYLUI_ToastType, QPoint)), this, SLOT(R_ShowToastMsg(QString, int, EYLUI_ToastShowPos, EYLUI_ToastType, QPoint)));


    //


}

void UI_Widget_MainWindow::InitUserInfoPanel()
{
    connect(CommonSignalService::Instance(), SIGNAL(S_UpdateUserNickName(QString)), this, SLOT(R_UpdateUserNickName(QString)));
}

void UI_Widget_MainWindow::InitAuxiliaryFunPanel()
{
    m_pPanel_ChangeSkin = new UI_Panel_ChangeSkin(this);
    m_pPanel_ChangeSkin->installEventFilter(this);
    m_pPanel_ChangeSkin->hide();

    connect(m_pPanel_ChangeSkin, SIGNAL(S_SkinBtn_Clicked(QString)), this, SLOT(R_SetMainWindowPanel_BgImg(QString)));


    connect(CommonSignalService::Instance(), SIGNAL(S_CurrentSkinChanged()), this, SLOT(R_CurrentSkinChanged()));
    connect(CommonSignalService::Instance(), SIGNAL(S_ShowNotice_Clicked()), this, SLOT(R_ShowNoticePanel()));

}


void UI_Widget_MainWindow::InitCommonPhrasesCardPanel()
{
    connect(CommonSignalService::Instance(), SIGNAL(S_CommonWord_TipMessage(QString)), this, SLOT(R_ShowMessageTips(QString)));
    connect(CommonSignalService::Instance(), SIGNAL(S_CommonWordPosChanged()), this, SLOT(R_CommonWordPosChanged()));

}

void UI_Widget_MainWindow::InitCompanyInfoStatisticsPanel()
{
    //UI_Model_Statistics::GetInstance()->RegisterObserver(m_widget_statistics_panel);
}


void UI_Widget_MainWindow::InitShotCut()
{
    //初始化进程内快捷键。
    UIMainWndSettingInfo *showInfo = UIPlatCommonDataModel::Instance()->MainWndSettingInfo();
    if (showInfo)
    {
        if (showInfo->EnableKeyCommonWordSearch() == 1)
        {
            QString keyCommonWordSearch = showInfo->KeyCommonWordSearch().toUpper();
            keyCommonWordSearch         = keyCommonWordSearch.replace(" ", "");

            if (m_shotcutCommonWord)
            {
                delete m_shotcutCommonWord;
            }
            m_shotcutCommonWord = new QShortcut(QKeySequence(keyCommonWordSearch), this);
            connect(m_shotcutCommonWord, SIGNAL(activated()), CommonSignalService::Instance(), SIGNAL(S_Key_CommonWordSearch()));
        }
        else
        {
            if (m_shotcutCommonWord)
            {
                delete m_shotcutCommonWord;
                m_shotcutCommonWord = nullptr;
            }
        }
        if (showInfo->EnableKeyOpenCard() == 1)
        {
            QString keyOpenCard = showInfo->KeyOpenCard().toUpper();
            keyOpenCard         = keyOpenCard.replace(" ", "");

            if (m_shotcutKeyOpenCard)
            {
                delete m_shotcutKeyOpenCard;
            }
            m_shotcutKeyOpenCard = new QShortcut(QKeySequence(keyOpenCard), this);
            connect(m_shotcutKeyOpenCard, SIGNAL(activated()), CommonSignalService::Instance(), SIGNAL(S_Key_OpenCard()));
        }
        else
        {
            if (m_shotcutKeyOpenCard)
            {
                delete m_shotcutKeyOpenCard;
                m_shotcutKeyOpenCard = nullptr;
            }
        }
        if (showInfo->EnableKeyChatChange() == 1)
        {
            QString keyChatChange = showInfo->KeyChatChange().toUpper();
            keyChatChange         = keyChatChange.replace(" ", "");
            if (m_shotcutKeyChatChange)
            {
                delete m_shotcutKeyChatChange;
            }
            m_shotcutKeyChatChange = new QShortcut(QKeySequence(keyChatChange), this);

            connect(m_shotcutKeyChatChange, SIGNAL(activated()), CommonSignalService::Instance(), SIGNAL(S_Key_ChatChange()));
        }
        else
        {
            if (m_shotcutKeyChatChange)
            {
                delete m_shotcutKeyChatChange;
                m_shotcutKeyChatChange = nullptr;
            }
        }

        if (showInfo->EnableKeyCloseSelectedChat() == 1)
        {
            QString keyCloseSelectedChat = showInfo->KeyCloseSelectedChat().toUpper();
            keyCloseSelectedChat         = keyCloseSelectedChat.replace(" ", "");
            if (m_shotcutKeyCloseChat)
            {
                delete m_shotcutKeyCloseChat;
            }
            m_shotcutKeyCloseChat = new QShortcut(QKeySequence(keyCloseSelectedChat), this);

            connect(m_shotcutKeyCloseChat, SIGNAL(activated()), CommonSignalService::Instance(), SIGNAL(S_Key_CloseSelectedChat()));
        }
        else
        {
            QString keyCloseSelectedChat = showInfo->KeyCloseSelectedChat().toUpper();
            keyCloseSelectedChat         = keyCloseSelectedChat.replace(" ", "");
            if (m_shotcutKeyCloseChat)
            {
                delete m_shotcutKeyCloseChat;
                m_shotcutKeyCloseChat = nullptr;
            }
        }
    }
}

QWidget *UI_Widget_MainWindow::getCenterwidget()
{
    //todo 待处理
    return ui->widget_center_dialogue;
    //return m_widget_dialogue_input_panel;
}

//////////////////////////////////////////////////////////////////////////
// CZC:公用函数
void UI_Widget_MainWindow::resizeEvent(QResizeEvent *event)
{
    //重新调整中央对话区预知输入面板位置

    m_pNoticePromptWidget->move(this->width() - 275, 1);

    QWidget::resizeEvent(event);

}

void UI_Widget_MainWindow::keyPressEvent(QKeyEvent *event)
{
    //开启日志输出窗口
    if (event->modifiers() == (Qt::AltModifier | Qt::ControlModifier) && event->key() == Qt::Key_J)
    {
        ShowOutputLogWnd();
    }
}

bool UI_Widget_MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (object == m_pPanel_ChangeSkin)
    {
        if (event->type() == QEvent::FocusOut)
        {
            m_pPanel_ChangeSkin->hide();
        }
        else if (event->type() == QEvent::Shortcut)
        {
            Q_ASSERT(false);
        }
    }


    return QWidget::eventFilter(object, event);
}

void UI_Widget_MainWindow::R_OpenPicture(QString url)
{
    if (QFile::exists(url)) //若图片地址为本地地址,直接打开图片
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(url));
        return;
    }

    if (m_pWidget_SystemPtompt == nullptr)
    {
        m_pWidget_SystemPtompt = new UI_Widget_SystemPrompt(this);
    }
    if (m_pWidget_SystemPtompt->isVisible())
        return;
    HttpCmd *cmd = new HttpCmd;
    connect(cmd, SIGNAL(S_SourceFinished(QString, QString, QByteArray)), this, SLOT(R_DownloadFileFinished(QString, QString, QByteArray)));
    connect(cmd, SIGNAL(S_DownLoadProgress(qint64, qint64)), this, SLOT(R_DownLoadProgress(qint64, qint64)));


    cmd->SetHeader("Content-Type", "application/octet-stream");
    cmd->Uri(url);

    QString filename;
    QRegExp rx("(\\w+)(\\.\\w+)+(?!.*(\\w+)(\\.\\w+)+)"); //截取url中文件名
    int     pos = 0;
    pos         = rx.indexIn(url, pos);
    if (pos != -1)
    {
        filename = rx.cap(0);
    }


    QString path = GetDownLoadFileDir();
    QString m_OpenPictureppath = path + filename;
    cmd->AddCmdData("fileName", m_OpenPictureppath);
    if (!QFile::exists(m_OpenPictureppath)) //本地没有下载过名称为filename的图片,则发起下载操作,否则直接打开本地已经下载的文件
    {
        m_pWidget_SystemPtompt->SetMsg(tr("正在下载......"), Enum_YL_Toast_Type_Common);
        m_pWidget_SystemPtompt->SetShowTime(0);
        m_pWidget_SystemPtompt->SetShowPos(this, Enum_YL_Toast_Parent_CenterCenter, QPoint());
        m_pWidget_SystemPtompt->show(true);
        cmd->Get();
    }
    else
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(m_OpenPictureppath));
    }
}

void UI_Widget_MainWindow::R_DownloadFileFinished(QString code, QString reason, QByteArray body)
{
    HttpCmd *cmd = (HttpCmd *)QObject::sender();
    if (cmd)
    {

        if (code != "200")
        {
            GERRORLOG(reason);
            m_pWidget_SystemPtompt->SetMsg(tr("图片加载失败,失败原因 : ") + reason, Enum_YL_Toast_Type_Error);
            m_pWidget_SystemPtompt->SetShowTime(2);
            m_pWidget_SystemPtompt->show(true);
        }
        else
        {
            QString fileName = cmd->GetCmdDataByKey("fileName");
            QFile   file(fileName);
            if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
            {
                file.write(body);
            }
            file.close();

            QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
            m_pWidget_SystemPtompt->hide();
        }
        cmd->deleteLater();
        cmd = NULL;
    }
}

void UI_Widget_MainWindow::R_DownLoadProgress(qint64 v, qint64 m)
{
    if (m > 0)
        m_pWidget_SystemPtompt->SetMsg(QString(tr("正在下载... 已下载 %1%")).arg(v / m * 100), Enum_YL_Toast_Type_Common);
}
// CZC:显示自定义悬浮窗口
void UI_Widget_MainWindow::InitShowCustomHoverPanel()
{
    connect(CommonSignalService::Instance(), SIGNAL(S_ShowCustomHoverPanel(QString)), this, SLOT(R_ShowCustomHoverPanel(QString)));
}
void UI_Widget_MainWindow::R_ShowCustomHoverPanel(QString userID)
{
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CZC:显隐常用语面板
void UI_Widget_MainWindow::InitShowOrHideCommonWordAndCardPanel()
{

    m_nCommonWordAndCardPanelWidth = 0;

    connect(CommonSignalService::Instance(), SIGNAL(S_ShowCommonWordAndCardPanel(bool)), this, SLOT(R_ShowCommonWordAndCardPanel(bool)));
}
void UI_Widget_MainWindow::R_ShowCommonWordAndCardPanel(bool show)
{
    UIMainWndSettingInfo *info = UIPlatCommonDataModel::Instance()->MainWndSettingInfo();
    if (info)
    {
        //隐藏常用语名片面板
        if (!show)
        {

            QList<int> listSize = ui->splitter->sizes();
            if (listSize.size() > 2)
            {
                //常用语名片面板 位于主面板右侧
                if (info->CommonWordDisplayPos() == 0)
                {
                    m_nCommonWordAndCardPanelWidth = listSize.at(2);
                    ui->widget_commonPhrases_card_panel_rect->hide();

                    QList<int> listWidth;
                    listWidth.append(listSize.at(0));

                    listWidth.append(listSize.at(1) + listSize.at(2) + 5);
                    listWidth.append(0);
                    ui->splitter->setSizes(listWidth);
                }

                //常用语名片面板 位于主界面中央
                else
                {
                    m_nCommonWordAndCardPanelWidth = listSize.at(1);
                    ui->widget_commonPhrases_card_panel_rect->hide();

                    QList<int> listWidth;
                    listWidth.append(listSize.at(0));

                    listWidth.append(0);
                    listWidth.append(listSize.at(1) + listSize.at(2) + 5);
                    ui->splitter->setSizes(listWidth);
                }
            }
        }

        //显示常用语名片面板
        else
        {


            QList<int> listSize = ui->splitter->sizes();
            if (listSize.size() > 2)
            {
                //常用语名片面板 位于主面板右侧
                if (info->CommonWordDisplayPos() == 0)
                {
                    ui->widget_commonPhrases_card_panel_rect->show();
                    int nChatRectWidth = ui->widget_center_dialogue->width() - listSize.at(0) - listSize.at(1) - m_nCommonWordAndCardPanelWidth - 4 * 5;

                    QList<int> listWidth;
                    listWidth.append(listSize.at(0));
                    listWidth.append(listSize.at(1));
                    listWidth.append(nChatRectWidth);
                    listWidth.append(m_nCommonWordAndCardPanelWidth);
                    ui->splitter->setSizes(listWidth);
                }

                //常用语名片面板 位于主界面中央
                else
                {
                    ui->widget_commonPhrases_card_panel_rect->show();
                    int nChatRectWidth = ui->widget_center_dialogue->width() - listSize.at(0) - listSize.at(1) - m_nCommonWordAndCardPanelWidth - 4 * 5;

                    QList<int> listWidth;
                    listWidth.append(listSize.at(0));
                    listWidth.append(listSize.at(1));
                    listWidth.append(m_nCommonWordAndCardPanelWidth);
                    listWidth.append(nChatRectWidth);
                    ui->splitter->setSizes(listWidth);
                }
            }
        }

        delete info;
        info = nullptr;
    }
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CZC：通知公告
void UI_Widget_MainWindow::InitNoticePanel()
{
    m_pNoticePromptWidget = new UI_NoticeBubblePrompt(this);
    m_pNoticePromptWidget->hide();
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CZC：界面面板尺寸调整
void UI_Widget_MainWindow::InitPanelScale()
{
    connect(CommonSignalService::Instance(), SIGNAL(S_SetPanelScale()), this, SLOT(R_SetPanelScale()));
    connect(CommonSignalService::Instance(), SIGNAL(S_TopRight_ResetScale_Clicked()), this, SLOT(R_RestorePanelScale()));
    connect(ui->splitter, SIGNAL(splitterMoved(int, int)), this, SLOT(R_SplitterMoved(int, int)));
}

//程序刚设置Ai各个面板初始尺寸
void UI_Widget_MainWindow::R_SetAiPanelScale()
{
    UIMainWndSettingInfo *info = UIPlatCommonDataModel::Instance()->MainWndSettingInfo();
    if (info)
    {
        int        nCommonWordWidth = 520;
        QList<int> listSize = ui->splitter->sizes();
        if (listSize.size() > 2)
        {
            if (info->CommonWordDisplayPos() == 0)
            {
                if (listSize.at(2) >= nCommonWordWidth)
                    return;
            }
            else
            {
                if (listSize.at(1) >= nCommonWordWidth)
                    return;
            }
        }

        int nVisitorChatWidth = info->LeftPanelDefaultWidth_VisitorChat();


        int nCommonWordPanelPos = info->CommonWordDisplayPos(); //获取常用语面板位置（右侧=0，中间=1）

        ResetScale(nCommonWordPanelPos, nVisitorChatWidth, 0, nCommonWordWidth, 280);

        delete info;
        info = nullptr;
    }
}

//程序刚启动设置各个面板初始尺寸
void UI_Widget_MainWindow::R_SetPanelScale()
{
    UIMainWndSettingInfo *info = UIPlatCommonDataModel::Instance()->MainWndSettingInfo();
    if (info)
    {
        int nVisitorChatWidth = info->LeftPanelDefaultWidth_VisitorChat();
        int nCommonWordWidth = info->RightPanelDefaultWidth();

        int nCommonWordPanelPos = info->CommonWordDisplayPos(); //获取常用语面板位置（右侧=0，中间=1）

        ResetScale(nCommonWordPanelPos, nVisitorChatWidth, 0, nCommonWordWidth, 280);

        delete info;
        info = nullptr;
    }
}
//点击右上角功能区还原尺寸按钮
void UI_Widget_MainWindow::R_RestorePanelScale()
{
    UIMainWndSettingInfo *info = UIPlatCommonDataModel::Instance()->MainWndSettingInfo();
    if (info)
    {
        int nCommonWordPanelPos = info->CommonWordDisplayPos(); //获取常用语面板位置（右侧=0，中间=1）
        ResetScale(nCommonWordPanelPos, 270, 0, 240, 280);
        info->LeftPanelDefaultWidth_VisitorChat(270);
        info->RightPanelDefaultWidth(240);

        //保存面板宽度到个人设置配置文件中
        UIPlatCommonDataModel::Instance()->MainWndSettingInfo(info);

        delete info;
        info = nullptr;
    }
}

void UI_Widget_MainWindow::R_SplitterMoved(int pos, int index)
{
    if (!m_pTimer_SplitterMovedOver)
    {
        m_pTimer_SplitterMovedOver = new QTimer(this);
        connect(m_pTimer_SplitterMovedOver, SIGNAL(timeout()), this, SLOT(R_SplitterMovedOver()));
    }
    m_pTimer_SplitterMovedOver->stop();
    m_pTimer_SplitterMovedOver->start(200);
}

void UI_Widget_MainWindow::R_SplitterMovedOver()
{
    m_pTimer_SplitterMovedOver->stop();
    UIMainWndSettingInfo *info = UIPlatCommonDataModel::Instance()->MainWndSettingInfo();
    if (info)
    {
        QList<int> list = ui->splitter->sizes();
        if (list.size() > 2)
        {
            info->LeftPanelDefaultWidth_VisitorChat(list.at(0));

            int nCommonWordPanelPos = info->CommonWordDisplayPos(); //获取常用语面板位置（右侧=0，中间=1）
            if (nCommonWordPanelPos == 0)
            {
                info->RightPanelDefaultWidth(list.at(2));
            }
            else
            {
                info->RightPanelDefaultWidth(list.at(1));
            }

            UIPlatCommonDataModel::Instance()->MainWndSettingInfo(info);
        }

        delete info;
        info = nullptr;
    }
}
void UI_Widget_MainWindow::ResetScale(int commonWordPos, int mainTreeWidth, int myChatWidth, int commonWordWidth, int editInput)
{

    m_nCommonWordAndCardPanelWidth = commonWordWidth;

    int nCenterWidth = ui->widget_center_dialogue->width() - mainTreeWidth - myChatWidth - commonWordWidth - 25;

    QList<int> listWidth;
    if (commonWordPos == 0)
    {
        listWidth.append(mainTreeWidth);

        listWidth.append(nCenterWidth);
        listWidth.append(commonWordWidth);
    }
    else
    {
        listWidth.append(mainTreeWidth);

        listWidth.append(commonWordWidth);
        listWidth.append(nCenterWidth);
    }
    ui->splitter->setSizes(listWidth);

    //输入功能区
    if (editInput > 0)
    {
        //todo 待处理
       // m_widget_dialogue_input_panel->ResetScale(editInput);
    }
}
//////////////////////////////////////////////////////////////////////////

void UI_Widget_MainWindow::R_ShowToastMsg(QString msg, int showTime, EYLUI_ToastShowPos showPos, EYLUI_ToastType type, QPoint tarPos)
{
    UI_Widget_SystemPrompt *pWidget_SystemPtompt = new UI_Widget_SystemPrompt(this);
    pWidget_SystemPtompt->SetMsg(msg, type);
    pWidget_SystemPtompt->SetShowTime(showTime);
    pWidget_SystemPtompt->SetShowPos(this, showPos, this->mapFromGlobal(tarPos));
    pWidget_SystemPtompt->show();
}

void UI_Widget_MainWindow::R_SetMainWindowPanel_BgImg(QString strImg)
{
    ET("UI_Widget_MainWindow::R_SetMainWindowPanel_BgImg");
    ChangeStringFormat(strImg);

    IBaseMainWnd *wnd = MainWnd();
    wnd->SetBackPanelIcon(strImg);
//todo 999
    //UIMainController::GetInstance()->SetSkinImage(strImg);
}

QString UI_Widget_MainWindow::QColorToQString(QColor color)
{
    QString strR     = QString::number(color.red());
    QString strG     = QString::number(color.green());
    QString strB     = QString::number(color.blue());
    QString strA     = QString::number(color.alpha());
    QString strColor = strR + "," + strG + "," + strB + "," + strA;
    return strColor;
}

void UI_Widget_MainWindow::R_ShowMessageTips(QString msg)
{
    UIMessageBox *wnd = new UIMessageBox;
    wnd->setWindowTitle(tr("提示"));
    wnd->SetContentTip(msg);
    wnd->SetCancelBtnVisible(false);
    wnd->exec();
}


QString UI_Widget_MainWindow::GetUrlByType(EURITYPE eType)
{
    QString retStr = "";
    switch (eType)
    {
        case E_Uri_ReportAnalysis:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("report");
            if (info)
            {
                retStr = info->GetValueByKey("auth");
                if (retStr.isEmpty())
                {
                    retStr = gSessionService->GetGlobalConfig()->GetValueByKey("reportLoginURL");
                }
            }
        }
        break;
        case E_Uri_ViewConversation:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("chatRecord");
            if (info)
            {
                retStr = info->GetValueByKey("auth");
                if (retStr.isEmpty())
                {
                    retStr = gSessionService->GetGlobalConfig()->GetValueByKey("recordLoginURL");
                }
            }
        }
        break;
        case E_Uri_CardPush:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("cardPush");
            if (info)
            {
                retStr = info->GetValueByKey("auth");
            }
            break;
        }
        case E_Uri_CenterManager:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("console");
            if (info)
            {
                retStr = info->GetValueByKey("auth");
                if (retStr.isEmpty())
                {
                    retStr = gSessionService->GetGlobalConfig()->GetValueByKey("consoleLoginURL");
                }
            }
        }
        break;
        case E_Uri_VisitorFilter:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("VisitorFilter");
            if (info)
            {
                retStr = info->GetValueByKey("auth");
                if (retStr.isEmpty())
                {
                    retStr = gSessionService->GetGlobalConfig()->GetValueByKey("consoleLoginURL");
                }
            }
        }
        break;
        case E_Uri_CommonWord:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("CommonWord");
            if (info)
            {
                retStr = info->GetValueByKey("auth");
                if (retStr.isEmpty())
                {
                    retStr = gSessionService->GetGlobalConfig()->GetValueByKey("consoleLoginURL");
                }
            }
        }
        break;
        case E_Uri_WeiPage:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("miniHTML");
            if (info)
            {
                retStr = info->GetValueByKey("auth");
                if (retStr.isEmpty())
                {
                    retStr = gSessionService->GetGlobalConfig()->GetValueByKey("consoleLoginURL");
                }
            }
        }
        break;
        case E_Uri_AutoResponse:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("miniHTML");
            if (info)
            {
                retStr = info->GetValueByKey("auth");
                if (retStr.isEmpty())
                {
                    retStr = gSessionService->GetGlobalConfig()->GetValueByKey("consoleLoginURL");
                }
            }
        }
        break;
        case E_Uri_ViewMessage:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("leaveMessage");
            if (info)
            {
                retStr = info->GetValueByKey("auth");
                if (retStr.isEmpty())
                {
                    retStr = gSessionService->GetGlobalConfig()->GetValueByKey("crmLoginURL");
                }
            }
        }
        break;
        case E_Uri_ViewRegister:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("registryForm");
            if (info)
            {
                retStr = info->GetValueByKey("auth");
                if (retStr.isEmpty())
                {
                    retStr = gSessionService->GetGlobalConfig()->GetValueByKey("crmLoginURL");
                }
            }
        }
        break;
        case E_Uri_ViewCard:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("visitorCard");
            if (info)
            {
                retStr = info->GetValueByKey("auth");
                if (retStr.isEmpty())
                {
                    retStr = gSessionService->GetGlobalConfig()->GetValueByKey("crmLoginURL");
                }
            }
        }
        break;
        case E_Uri_VerifyCard:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("verifyCard");
            if (info)
            {
                retStr = info->GetValueByKey("auth");
                if (retStr.isEmpty())
                {
                    retStr = gSessionService->GetGlobalConfig()->GetValueByKey("crmLoginURL");
                }
            }
        }
        break;
        case E_Uri_CRM:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("CRM");
            if (info)
            {
                retStr = info->GetValueByKey("auth");
                if (retStr.isEmpty())
                {
                    retStr = gSessionService->GetGlobalConfig()->GetValueByKey("crmLoginURL");
                }
            }
        }
        break;
        case E_Uri_FreePhone:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("freePhone");
            if (info)
            {
                retStr = info->GetValueByKey("auth");
                if (retStr.isEmpty())
                {
                    retStr = gSessionService->GetGlobalConfig()->GetValueByKey("crmLoginURL");
                }
            }
        }
        break;
        case E_Uri_ProcessDianosis:

        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("visitorAnalyse");
            if (info)
            {
                retStr = info->GetValueByKey("auth");
                if (retStr.isEmpty())
                {
                    retStr = gSessionService->GetGlobalConfig()->GetValueByKey("visitorLoginURL");
                }
            }
        }
        break;
        case E_Uri_CardAssign:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("CARD_ALLOCATION");
            if (info)
            {
                retStr = info->GetValueByKey("auth");
            }
        }
        break;
        case E_Uri_WorkOrder:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("WORK_ORDER");
            if (info)
            {
                retStr = info->GetValueByKey("auth");
            }
        }
        break;
        case E_Uri_EachbotAI:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("AI");
            if (info)
            {
                retStr = info->GetValueByKey("auth");
            }
        }
        break;
        case E_Uri_AISettingCenter:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("AISettingCenter");
            if (info)
            {
                retStr = info->GetValueByKey("auth");
            }
        }

        break;
        case E_Uri_ClueQuery:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("clues");
            if (info)
            {
                retStr = info->GetValueByKey("auth");
            }
        }

        break;

        case E_Uri_AuthOpen:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("10000:AuthOpen");
            if (info)
            {
                retStr = info->GetValueByKey("auth");
            }
        }
        break;
        case E_Uri_BuidWeb:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("EWIDE_WEBSITE_ABC");
            if (info)
            {
                retStr = info->GetValueByKey("auth");
            }
        }
        break;
        default:
            break;
    }
    return retStr;
}

QString UI_Widget_MainWindow::GetUrl(QString tempUri)
{
    static QString format = "%1?c=%2&userName=%3&version=%4&s=%5&check=%6";
    QString        strUrl = format.arg(tempUri).arg(gSessionService->UserInfo()->CompanyId()).arg(gSessionService->UserInfo()->UserId()).arg(gUISystemConfigManager->GetCurrentVersion()).arg(QString("%1").arg(QDateTime::currentDateTime().toMSecsSinceEpoch())).arg(gSessionService->Token());
    return strUrl;
}

QString UI_Widget_MainWindow::GetSubUrlByType(EURITYPE eType)
{
    QString strUrl = "";
    switch (eType)
    {
        case E_Uri_ProcessDianosis:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("visitorAnalyse");
            if (info)
            {
                QString tempUrl = info->GetValueByKey("url");
                if (tempUrl.isEmpty())
                {
                    strUrl = "url=/visitorAnalyse";
                }
                else
                {
                    strUrl = QString("url=%1").arg(tempUrl);
                }
            }
        }
        break;
        case E_Uri_ViewConversation:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("chatRecord");
            if (info)
            {
                QString tempUrl = info->GetValueByKey("url");
                if (tempUrl.isEmpty())
                {
                    strUrl = "url=/chatRecord";
                }
                else
                {
                    strUrl = QString("url=%1").arg(tempUrl);
                }
            }
        }
        break;
        case E_Uri_CardPush:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("cardPush");
            if (info)
            {
                QString tempUrl = info->GetValueByKey("url");
                if (tempUrl.isEmpty())
                {
                    strUrl = "url=/chatRecord";
                }
                else
                {
                    strUrl = QString("url=%1").arg(tempUrl);
                }
            }
            break;
        }
        case E_Uri_ViewMessage:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("leaveMessage");
            if (info)
            {
                QString tempUrl = info->GetValueByKey("url");
                if (tempUrl.isEmpty())
                {
                    strUrl = "url=leaveMsgMan";
                }
                else
                {
                    strUrl = QString("url=%1").arg(tempUrl);
                }
            }
        }
        break;
        case E_Uri_ViewRegister:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("registryForm");
            if (info)
            {
                QString tempUrl = info->GetValueByKey("url");
                if (tempUrl.isEmpty())
                {
                    strUrl = "url=formManage";
                }
                else
                {
                    strUrl = QString("url=%1").arg(tempUrl);
                }
            }
        }
        break;
        case E_Uri_ViewCard:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("visitorCard");
            if (info)
            {
                QString tempUrl = info->GetValueByKey("url");
                if (tempUrl.isEmpty())
                {
                    strUrl = "url=cardsMan";
                }
                else
                {
                    strUrl = QString("url=%1").arg(tempUrl);
                }
            }
        }
        break;
        case E_Uri_VerifyCard:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("visitorCard");
            if (info)
            {
                QString tempUrl = info->GetValueByKey("url");
                if (tempUrl.isEmpty())
                {
                    strUrl = "url=verifyCard";
                }
                else
                {
                    strUrl = QString("url=%1").arg(tempUrl);
                }
            }
        }
        break;
        case E_Uri_VisitorFilter:
            strUrl = "url=/Visitor/VisitorFilter";
            break;
        case E_Uri_AutoResponse:
            strUrl = "url=/Customer/AutoResp";
            break;
        case E_Uri_CommonWord:
            strUrl = "url=/CommonWord/Items/personnel";
            break;
        case E_Uri_CenterManager:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("console");
            if (info)
            {
                QString tempUrl = info->GetValueByKey("url");
                if (tempUrl.isEmpty())
                {
                    strUrl = "url=/Console/Index";
                }
                else
                {
                    strUrl = QString("url=%1").arg(tempUrl);
                }
            }
        }
        break;
        case E_Uri_CRM:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("CRM");
            if (info)
            {
                QString tempUrl = info->GetValueByKey("url");
                if (tempUrl.isEmpty())
                {
                    strUrl = "url=/CRM";
                }
                else
                {
                    strUrl = QString("url=%1").arg(tempUrl);
                }
            }
        }
        break;
        case E_Uri_WeiPage:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("miniHTML");
            if (info)
            {
                QString tempUrl = info->GetValueByKey("url");
                if (tempUrl.isEmpty())
                {
                    strUrl = "url=/mihtml/index";
                }
                else
                {
                    strUrl = QString("url=%1").arg(tempUrl);
                }
            }
        }
        break;
        case E_Uri_FreePhone:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("freePhone");
            if (info)
            {
                QString tempUrl = info->GetValueByKey("url");
                if (tempUrl.isEmpty())
                {
                    strUrl = "url=freePhoneMan";
                }
                else
                {
                    strUrl = QString("url=%1").arg(tempUrl);
                }
            }
        }
        break;
        case E_Uri_CardAssign:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("CARD_ALLOCATION");
            if (info)
            {
                QString tempUrl = info->GetValueByKey("url");
                if (tempUrl.isEmpty())
                {
                    strUrl = "url=/index";
                }
                else
                {
                    strUrl = QString("url=%1").arg(tempUrl);
                }
            }
        }
        break;
        case E_Uri_WorkOrder:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("WORK_ORDER");
            if (info)
            {
                QString tempUrl = info->GetValueByKey("url");
                if (tempUrl.isEmpty())
                {
                    strUrl = "url=/index";
                }
                else
                {
                    strUrl = QString("url=%1").arg(tempUrl);
                }
            }
        }
        break;
        case E_Uri_EachbotAI:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("AI");
            if (info)
            {
                QString tempUrl = info->GetValueByKey("url");
                if (tempUrl.isEmpty())
                {
                    strUrl = "url=/index";
                }
                else
                {
                    strUrl = QString("url=%1").arg(tempUrl);
                }
            }
        }
        break;
        case E_Uri_AISettingCenter:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("AISettingCenter");
            if (info)
            {
                QString tempUrl = info->GetValueByKey("url");
                if (tempUrl.isEmpty())
                {
                    strUrl = "";
                }
                else
                {
                    strUrl = QString("url=%1").arg(tempUrl);
                }
            }
        }
        break;
        case E_Uri_ClueQuery:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("clues");
            if (info)
            {
                QString tempUrl = info->GetValueByKey("url");
                if (tempUrl.isEmpty())
                {
                    strUrl = "/clues";
                }
                else
                {
                    strUrl = QString("url=%1").arg(tempUrl);
                }
            }
        }
        break;
        case E_Uri_BuidWeb:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("EWIDE_WEBSITE_ABC");
            if (info)
            {
                QString tempUrl = info->GetValueByKey("url");
                if (tempUrl.isEmpty())
                {
                    strUrl = "url=https://www.jzabc.cn/";
                }
                else
                {
                    strUrl = QString("url=%1").arg(tempUrl);
                }
            }
            else
            {
                strUrl = "url=https://www.jzabc.cn/";
            }
        }
        break;
        case E_Uri_AuthOpen:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("10000:AuthOpen");
            if (info)
            {
                QString tempUrl = info->GetValueByKey("url");
                if (tempUrl.isEmpty())
                {
                    strUrl = "url=http://auth.easyliao.com/auth-open/vi/ad/10000";
                }
                else
                {
                    strUrl = QString("url=%1").arg(tempUrl);
                }
            }
            else
            {
                strUrl = "url=http://auth.easyliao.com/auth-open/vi/ad/10000";
            }
        }
        break;
        case E_Uri_ReportAnalysis:
        {
            CommInfo *info = UICommonDataModel::GetInstance()->GetIconInfoByKey("report");
            if (info)
            {
                QString tempUrl = info->GetValueByKey("url");
                if (tempUrl.isEmpty())
                {
                    strUrl = "/report";
                }
                else
                {
                    strUrl = QString("url=%1").arg(tempUrl);
                }
            }
        }
        break;
        default:
            break;
    }
    return strUrl;
}

/* 曹振超：右上角功能区 */
void UI_Widget_MainWindow::ChangeStringFormat(QString &str)
{
    QString strTemp = str;
    strTemp.simplified();
    strTemp.trimmed();
    strTemp.replace("\\", "/");
    strTemp.replace("//", "/");

    str = strTemp;
}

//日志查看窗口
void UI_Widget_MainWindow::ShowOutputLogWnd()
{
    UI_Wnd_OutputLog::GetInstance()->ShowType(true);
    UI_Wnd_OutputLog::GetInstance()->activateWindow();
}



//调换中央对话区面板和常用语面板（根据个人设置）
void UI_Widget_MainWindow::ChangeWidget_MainWindow_Splitter(int commonWordPos)
{
    QWidget *widget2 = ui->splitter->widget(1);
    QWidget *widget3 = ui->splitter->widget(2);

    widget2->setParent(nullptr);
    widget3->setParent(nullptr);

    QWidget *widgetCenterChat = nullptr;
    QWidget *widgetCommonWord = nullptr;
    QString  strWidget2Name   = widget2->objectName();
    if (strWidget2Name == "widget_commonPhrases_card_panel_rect")
    {
        widgetCenterChat = widget3;
        widgetCommonWord = widget2;
    }
    else
    {
        widgetCenterChat = widget2;
        widgetCommonWord = widget3;
    }

    if (commonWordPos == 0)
    {
        ui->splitter->addWidget(widgetCenterChat);
        ui->splitter->addWidget(widgetCommonWord);
    }
    else
    {
        ui->splitter->addWidget(widgetCommonWord);
        ui->splitter->addWidget(widgetCenterChat);
    }
}

void UI_Widget_MainWindow::R_UpdateUserNickName(QString nickName)
{
    QString name = QString("%1(%2)").arg(gSessionService->UserInfo()->RealName()).arg(gSessionService->UserInfo()->NickName());
    m_widget_user_panel->SetUserName(name);
}

void UI_Widget_MainWindow::R_CommonWordPosChanged()
{
    UIMainWndSettingInfo *info = UIPlatCommonDataModel::Instance()->MainWndSettingInfo();
    if (info)
    {
        int nCommonWordDisplayPos = info->CommonWordDisplayPos();

        //获取中间区域四个面板的宽度
        int nChatListRect = ui->widget_dialogue_list_panel_rect->width();
        int nCommonWordRect = ui->widget_commonPhrases_card_panel_rect->width();

        //调换中央对话区面板和常用语面板（根据个人设置）
        ChangeWidget_MainWindow_Splitter(nCommonWordDisplayPos);

        //重新设置各个面板宽度
        ResetScale(nCommonWordDisplayPos, nChatListRect, 0, nCommonWordRect, -1);
    }
}

void UI_Widget_MainWindow::ShowIeWndByUri(EURITYPE type, int openType, QString subURiParam)
{
    QString strUrl     = "";
    QString strTempUri = GetUrlByType(type);
    QString strUri;

    QString subURi;
    subURi = subURiParam;

    strUri = GetUrl(strTempUri);
    if (subURi.isEmpty())
        subURi = GetSubUrlByType(type);
    if (subURi.isEmpty())
    {
        strUrl = strUri;
    }
    else
    {
        strUrl = QString("%1&%2").arg(strUri).arg(subURi);
    }
    if (!strUrl.isEmpty())
    {
        gViewService->DoGotoUrl(strUrl);

    }
}

void UI_Widget_MainWindow::SetDisconnectTip(int isDisconnect)
{
	if(m_widget_statistics_panel)m_widget_statistics_panel->SetBrokenNet(isDisconnect);
}
void UI_Widget_MainWindow::DoReceiveMessage(const Im_Message &msg)
{
    if (msg.GetValueByKey("colleagueMessage") == "0")
    {
        DoReceiveVisitorMessage(msg);
    }
    else if (msg.GetValueByKey("colleagueMessage") == "1")
    {
        //同事消息
        DoReceiveColleagueMessage(msg);
    }
}

void UI_Widget_MainWindow::DoRobotAutoReplyError(MChatItemData *objData, const QString &content, QString code)
{
    Q_UNUSED(objData);
    Q_UNUSED(content);
    Q_UNUSED(code);
    DoSystemTipProcess("robotNoTaking");
}

void UI_Widget_MainWindow::DoCreateTransferChat(UIChatInfo *pinfo, QString msgData)
{
    Q_UNUSED(msgData);
    // todo 9815
    if (pinfo->CustomerID() == gSessionService->UserInfo()->UserId())
    {
        DoSystemTipProcess("receiveTransferMessage");
    }
    else
    {
        DoSystemTipProcess("receiveFriendTransferMessage");
    }
}
void UI_Widget_MainWindow::DoCreateChat(UIChatInfo *info)
{
    if (info)
    {
        if (info->TempChat() == false)
        {

            if (info->CustomerID() == gSessionService->UserInfo()->UserId())
            {
                if (info->MsgType() == EMESSAGETYPE::E_MSGTYPE_ACCEPTTAKEOVER)
                {
                    DoSystemTipProcess("receiveTransferMessage");
                }
                else
                    DoSystemTipProcess("creatchat");
            }
            else
                DoSystemTipProcess("creatFriendChat");
        }
    }
}


void UI_Widget_MainWindow::DoCustomerPresenceChanged(QString fromId, EPRESENCESTATUS newStatus, EPRESENCESTATUS oldStatus, CUSTOMER_LOGIN_TYPE loginType)
{
    if (fromId == gSessionService->UserInfo()->UserId())
    {
        if (newStatus == E_NONE)
        {
            //当新状态为 E_NONE表示失败
            EYLUI_ToastShowPos tpos = Enum_YL_Toast_Target_LeftTop_Left;
            EYLUI_ToastType    type = Enum_YL_Toast_Type_Warning;

            UI_Widget_SystemPrompt *pWidget_SystemPtompt = new UI_Widget_SystemPrompt(this);
            pWidget_SystemPtompt->SetMsg(tr("网络异常，请重新切换状态"), type);
            pWidget_SystemPtompt->SetShowTime(10);
            pWidget_SystemPtompt->SetStyle(0, QColor("#ffffff"), 3, QColor("#ffffFF"));
            pWidget_SystemPtompt->SetFont(12, "#000000");
            pWidget_SystemPtompt->SetIcon(":/YL/Resources/YL/Toast/icon_toast_warning2.png");
            pWidget_SystemPtompt->SetShowPos(this, tpos, QPoint(250, 195));
            pWidget_SystemPtompt->show();
        }
    }
}

void UI_Widget_MainWindow::DoGetVisitorInfoResult(QString userid, UICardInfo &info, int type)
{
    if (type == 1) //只有获取到TCP通知名片变更时,才通知
    {
        QString strCreateUserId = info.CreateUserId();
        QString strEditUserId   = info.EditUserId();
        QString strCurUserId    = gSessionService->UserInfo()->UserId();

        if (strEditUserId.isEmpty())
        {
            if (strCreateUserId != strCurUserId)
            {
                DoSystemTipProcess("saveFriendCard");
            }
        }
        else
        {
            if (strEditUserId != strCurUserId)
            {
                DoSystemTipProcess("saveFriendCard");
            }
        }
    }
}
void UI_Widget_MainWindow::DoSystemTipProcess(const QString &type, const QString &vid, const QString &charid)
{
    UISystemTipData *tipObj = CommonModelService::GetInstance()->GetSystemTipByKey(type);
    if (tipObj && tipObj->IsEnable())
    {
        //系统通知
        if (tipObj->IsBubbleTip())
        {
            if (type == "visitOnline")
            {
                ShowMessage(tr("提示"), tr("新到一个访客！"));
            }
            else if (type == "creatchat")
            {
                ShowMessage(tr("提示"), tr("新创建一个会话！"));
            }
            else if (type == "receiveMessage")
            {
                ShowMessage(tr("提示"),tr("收到一个消息！"));
            }
            else if (type == "receiveTransferMessage")
            {
                ShowMessage(tr("提示"),tr("收到一个转移的会话！"));
            }else if (type == "receiveFriendTransferMessage")
            {
                ShowMessage(tr("提示"), tr("好友收到一个转移的会话！"));
            }
            else if (type == "innerContact")
            {
                ShowMessage(tr("提示"),tr("收到一个同事好友的会话！"));
            }
            else if (type == "waitQueueVisitor")
            {
                ShowMessage(tr("提示"),tr("新到一个排队访客！"));
            }
            else if (type == "saveCard" )
            {
                ShowMessage(tr("提示"),tr("新到一个访客留联信息！"));
            }
            else if (type == "robotNoTaking")
            {
                ShowMessage(tr("提示"),tr("有一条机器人未学习话术！"));
            }
            else if (type == "waitQueueVisitor")
            {
                ShowMessage(tr("提示"),tr("新到一个排队访客！"));
            }
            else if (type == "receiveFriendMessage")
            {
                ShowMessage(tr("提示"),tr("同事好友收到一条新消息！"));
            }
            else if (type == "creatFriendChat")
            {
                ShowMessage(tr("提示"),tr("同事好友创建一条新的对话！"));
            }
            else if (type == "saveFriendCard")
            {
                ShowMessage(tr("提示"),tr("收到同事保存名片或访客留联的消息！"));
            }
        }

        // Toast 提醒
        if (tipObj->IsToastTip())
        {
            if (type == "creatchat")
            {
                R_ShowToastMsg(tr("您新到一个对话"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Common, QPoint());
            }
            else if (type == "receiveTransferMessage")
            {
                R_ShowToastMsg(tr("您新到一个转接对话"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Warning, QPoint());
            }
            else if (type == "waitQueueVisitor")
            {
                R_ShowToastMsg(tr("您新到一个排队中的访客"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Urgent, QPoint());
            }
        }

        //主窗口置顶
        if (tipObj->IsShowMainWndTip())
        {
            if (vid.isEmpty() == false)
            {
                IBaseMainWnd *wnd = MainWnd();

                if ((wnd) && ((wnd->isHidden() || wnd->isMinimized() || (wnd->isActiveWindow() == false))))
                    CommonSignalService::Instance()->DoMainSelectChat(vid);
            }
            Q_EMIT S_OpenMainwnd();
        }
        else
        {
            if (m_timer)
            {
                if (tipObj->IsFlashTip())
                {
                    if (!m_timer->isActive())
                    {
                        m_timer->start();
                    }
                    m_flash = true;
                }
            }
        }

        //声音提醒
        if (tipObj->GetVoiceType() != "0")
        {
            if (gUICacheService->GetCacheData(charid) == "1" && "receiveMessage" == type)
            {
                return;
            }
            //todo 待处理
            //IUIController::GetInstance()->OnPlayAudition("SystemTip", tipObj->GetVoiceFileName());
        }
    }
}

void UI_Widget_MainWindow::ShowMessage(QString title, QString msg)
{
    Q_EMIT S_BubbleTipShowMessage(title, msg);
}



void UI_Widget_MainWindow::R_SummaryResult(bool isOk)
{
    //todo 9999
    /*
    if (isOk)
    {
        MChatItemData *currentItem = UIYL_ChatTreeModel::GetInstance()->GetCurrentChatItemData();
        if (currentItem)
        {
            CommonSignalService::Instance()->DoSummaryResult(currentItem->Vid(), currentItem->Chatid(), isOk);
        }
    }
    */
}


void UI_Widget_MainWindow::R_OnTimer()
{
    if (isActiveWindow())
    {
        if (m_timer)
        {
            m_flash = false;
        }
    }
    if (m_flash)
    {
        QWidget *pActiveWindow = QApplication::activeWindow();
        if (pActiveWindow == nullptr)
            QApplication::alert(MainWnd());
    }
}

void UI_Widget_MainWindow::DoReceiveVisitorMessage(const Im_Message &msg)
{
    QString chatId = msg.GetValueByKey("chatId");

    QString vid = msg.GetValueByKey("fromUserId");

    if (msg.GetMsgType() == E_MSGTYPE_SCREENSHOTS || msg.GetMsgType() == E_MSGTYPE_FILE || msg.GetMsgType() == E_MSGTYPE_MESSAGE)
    {
        UIChatInfo *chat = UICommonDataModel::GetInstance()->GetChatByChatId(chatId);
        if ((chat) && (chat->CustomerID() == gSessionService->UserInfo()->UserId()))
        {
            DoSystemTipProcess("receiveMessage", vid, chatId);

            //切到最新对话里
        }
        else
        {
            if ((chat) && (chat->HasVisitorByVid(vid) || (chat->HasVisitorByUserId(vid))))
            {
                DoSystemTipProcess("receiveFriendMessage", vid);
                //切到最新对话里
            }
        }
    }
}

void UI_Widget_MainWindow::DoReceiveColleagueMessage(const Im_Message &msg)
{
    DoSystemTipProcess("innerContact");
}


void UI_Widget_MainWindow::DoSaveVisitorInfoFinished(QString isAuto, QString vid, QString staticId)
{
    DoSystemTipProcess("saveCard");
}

void UI_Widget_MainWindow::R_AutoLeaveTimeOut()
{
    EPRESENCESTATUS fromStatus;
    if (gSessionService->UserInfo())
    {
        QString userid = gSessionService->UserInfo()->UserId();
        fromStatus = gUIColleagueManager->GetPresenceStatusByID(userid);
        if (fromStatus == E_LEAVE)
        {
            return;
        }
    }
    int       leaveTime     = 0;
    CommInfo *locInfoConfig = gSessionService->LocalConfig();
    if (locInfoConfig)
    {
        QString valAutoLeave = locInfoConfig->GetValueByKey("autoleavestate");
        if (valAutoLeave.isEmpty())
        {
            return;
        }
        else
        {
            leaveTime = valAutoLeave.toInt();
        }
    }else
    {
        return;
    }

    //设置离开状态
#ifdef _MSC_VER
    LASTINPUTINFO lii = {sizeof(LASTINPUTINFO)};
    GetLastInputInfo(&lii);
    int iTemp = (GetTickCount() - lii.dwTime) / 1000; //当前闲置秒数

    if (iTemp > leaveTime * 60)
    {
        EPRESENCESTATUS tostate = UtilsHelper::GetPresenceStatusByString(ConstStringHelper::m_cLeave);
        CommonSignalService::Instance()->DoStateChange(fromStatus, tostate);
    }
#endif
}



void UI_Widget_MainWindow::ActivateGlobalShortcut(const quint32 keyCode, const quint32 modifier)
{
    UIMainWndSettingInfo *showInfo = UIPlatCommonDataModel::Instance()->MainWndSettingInfo();
    if (showInfo)
    {
        if (showInfo->EnableKeyActiveMainWnd() == 1)
        {
            QString strKeyActiveMainWnd = showInfo->KeyActiveMainWnd().toUpper();
            if (!strKeyActiveMainWnd.isEmpty())
            {
                strKeyActiveMainWnd = strKeyActiveMainWnd.replace(" ", "");
                UINT vk             = UtilsHelper::GetKeyByShotKey(strKeyActiveMainWnd);
                UINT modifyKey      = UtilsHelper::GetModifyKeyByHotKey(strKeyActiveMainWnd);

                if (UtilsHelper::NativeModifiers(modifyKey) == modifier && UtilsHelper::NativeKeyCode(vk) == keyCode)
                {
                    Q_EMIT S_OpenMainwnd();
                }
            }
        }
    }
}

void UI_Widget_MainWindow::R_ShotCutKey()
{
    Q_ASSERT(false);
}

void UI_Widget_MainWindow::R_ShowSettingChanged()
{
    InitShotCut();
}

void UI_Widget_MainWindow::R_CurrentSkinChanged()
{
    UISkinInfo *info = gUISystemConfigManager->CurrentSkin();
    if (info)
    {
        if (m_skinKey != info->Key())
        {
            m_skinKey = info->Key();

            QString strFullPath = UtilsHelper::GetFullPathByRunDir(info->FileName());
            if (!QFile::exists(strFullPath))
            {
                if (!m_pPanel_ChangeSkin)
                {
                    m_pPanel_ChangeSkin = new UI_Panel_ChangeSkin(this);
                }
                m_pPanel_ChangeSkin->SetDefaultSkin(info->Name());
            }
            else
            {
                R_SetMainWindowPanel_BgImg(strFullPath);
            }
        }
    }
}

void UI_Widget_MainWindow::R_LoginStateChanged(ELOGINSTATUS oldStatus, ELOGINSTATUS newStatus)
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

                static int logined = 0;
                if (logined == 0)
                {
                    logined = 1;
                    InitUserInfo();

                    InitSkinPicture();
                    InitCompanyCommonWord();
                    InitAutoLeaveTimer();
                }

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

void UI_Widget_MainWindow::R_VisitorMsgNoReplySelf_Sound()
{
    DoSystemTipProcess("visitorMsgNoReply_Self");
}

void UI_Widget_MainWindow::R_VisitorMsgNoReplyMonitor_Sound()
{
    DoSystemTipProcess("visitorMsgNoReply_Monitor");
}

void UI_Widget_MainWindow::InitUserInfo()
{
    QString name = QString("%1(%2)").arg(gSessionService->UserInfo()->RealName()).arg(gSessionService->UserInfo()->NickName());
    m_widget_user_panel->SetUserName(name);
    m_widget_user_panel->SetAccountNumber(gSessionService->UserInfo()->UserId());

    QString headUri = ":/Resources/SVG/user_head_portrait.svg";
    if (gSessionService->UserInfo() && (!gSessionService->UserInfo()->UserId().isEmpty()))
    {
        if (gSessionService->UserInfo()->AiFlag() == 0)
        {
            headUri = ":/Resources/SVG/user_head_portrait.svg";
        }
        else if (gSessionService->UserInfo()->AiFlag() == 1)
        {
            headUri = ":/Resources/SVG/user_ai_head_portrait.svg";
        }
        else
        {
            Q_ASSERT(false);
        }
    }

    m_widget_user_panel->SetHeadPortrait(headUri);
}


void UI_Widget_MainWindow::InitSkinPicture()
{
    UISkinInfo *info        = gUISystemConfigManager->CurrentSkin();
    QString     strFullPath = UtilsHelper::GetFullPathByRunDir(info->FileName());
    R_SetMainWindowPanel_BgImg(strFullPath);
}

void UI_Widget_MainWindow::InitCompanyCommonWord()
{
    //根据配置信息，设置常用语面板位置
    UIMainWndSettingInfo *info = UIPlatCommonDataModel::Instance()->MainWndSettingInfo();
    if (info && (info->CommonWordDisplayPos() == 1))
    {
        ChangeWidget_MainWindow_Splitter(info->CommonWordDisplayPos());
        R_SetPanelScale();

        delete info;
        info = nullptr;
    }

}

void UI_Widget_MainWindow::InitAutoLeaveTimer()
{
    m_autoLeaveTimer = new QTimer;
    connect(m_autoLeaveTimer, SIGNAL(timeout()), this, SLOT(R_AutoLeaveTimeOut()));
    m_autoLeaveTimer->setInterval(60 * 1000);
    m_autoLeaveTimer->start();
}

void UI_Widget_MainWindow::R_ShowIeWndByUri(EURITYPE type, int openType, QString subURiParam)
{
    ShowIeWndByUri(type, openType, subURiParam);
}

QString  UI_Widget_MainWindow::GetDownLoadFileDir()
{
    return "";
}


void UI_Widget_MainWindow::AddTopWnd(EL_TopFunInterface * wnd)
{
    m_widget_main_fun_panel = wnd;
	m_widget_main_fun_panel->setObjectName(QString::fromUtf8("widget_main_fun_panel"));

    ui->widget_top_right_rect_Layout->addWidget(m_widget_main_fun_panel);

	QSpacerItem * widget_main_fun_Spacer = new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::Minimum);

    ui->widget_top_right_rect_Layout->addItem(widget_main_fun_Spacer);
    m_widget_main_fun_panel->setTopFun();



}

void UI_Widget_MainWindow::AddStatisticsPanel(UI_Widget_Statistics_Interface * panel)
{
    m_widget_statistics_panel = panel;
    m_widget_statistics_panel->setObjectName(QString::fromUtf8("widget_statistics_panel"));

    ui->horizontalLayout->addWidget(m_widget_statistics_panel);

}

void UI_Widget_MainWindow::AddWidgetUser(widget_user_interface * panel )
{
            m_widget_user_panel = panel;
           
           QHBoxLayout * horizontalLayout_6 = new QHBoxLayout(ui->widget_user_panel_rect);
           horizontalLayout_6->setSpacing(0);
           horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
           horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
           horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
           //widget_user_panel = new UI_Widget_User_Panel(widget_user_panel_rect);

           m_widget_user_panel->setObjectName(QString::fromUtf8("widget_user_panel"));

           horizontalLayout_6->addWidget(m_widget_user_panel);


}

void UI_Widget_MainWindow::AddCommonPhrasesPanel(UI_Widget_CommonPhrasesCard_Interface * panel )
{

            m_widget_commonPhrases_card_panel = panel;//new QWidget(splitter);

            ui->widget_commonPhrases_card_panel_rect_Layout->addWidget(m_widget_commonPhrases_card_panel);


}


QWidget *UI_Widget_MainWindow::getWidget(QString type)
{
	if (type == "commonPhrases_card_panel_rect")
	{
        return ui->widget_commonPhrases_card_panel_rect;

	}else if (type == "bottom_rect_bg")
	{
        return ui->widget_bottom_rect_bg;
	}
	else if (type == "widget_top_rect")
	{
        return ui->widget_top_rect;
	}
	else if (type == "widget_top_right_rect")
	{
        return ui->widget_top_right_rect;
	}

    return nullptr;
}

void UI_Widget_MainWindow::R_LoadUi(int type)
{
    m_widget_dialogue_list_panel = PlatUiService::GetInstance()->GetChatTree();
    ui->widget_dialogue_list_panel_rect_Layout->addWidget(m_widget_dialogue_list_panel);


}
