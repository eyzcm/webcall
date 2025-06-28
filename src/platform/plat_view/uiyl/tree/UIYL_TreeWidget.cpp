#include "stdafx.h"
#include "UIYL_TreeWidget.h"
#include "utils/EL_Util_Time.h"


/*
#include "control/CommonSignalService.h"
#include "service/DbUiWebcallService.h"
#include "model/UICommonDataModel.h"
#include "protocol_biz/webcallprotocolservice.h"
*/

#include "session/el_sessionservice.h"

#include <QComboBox>
#include <QDebug>
#include <QTreeWidget>
#ifdef _MSC_VER
#include <Windows.h>
#endif
#include "interface/uiyl_treeviewinterface.h"

#include <server/platuiservice.h>

#define YL_MAIN_TREE 0
#define YL_CUSTOMER_TREE 1

UIYL_TreeWidget::UIYL_TreeWidget(QWidget *parent) : UIYL_ChatTreeInterface(parent)
{
    //过渡页显示进度
    gLoginTransitionsPage->SendSchedule("30", tr("初始化对话列表区..."));

    ET("UIYL_TreeWidget_setupUi");
    ui.setupUi(this);

    m_number             = 0;
    m_receverMsgFlashing = false;
    m_treeTab = 0;
    connect(PlatUiService::GetInstance(), SIGNAL(S_LoadUi(int)), this, SLOT(R_LoadUi(int)));

    /*
    foreach( UIYL_TreeViewInterface * treeView ,m_treeList )
    {
        treeView->SearchLine()->installEventFilter(this);
        treeView->SearchResultWnd()->hide();
    }
    */
    //Init();

}

UIYL_TreeWidget::~UIYL_TreeWidget()
{
}


void UIYL_TreeWidget::OnTreeBtnClick(UIYL_TreeViewInterface * treeview)
{
    if (m_treeList.size() <= 0) return;
    m_treeList.at(m_treeTab)->SearchResultWnd()->hide();
    int newTab = (int)m_treeList.indexOf(treeview);
    Q_EMIT S_OnYLMainTreeSelectTabChanged(m_treeTab, newTab);
    m_treeTab = newTab;
    ui.m_stackedWidget->setCurrentIndex(newTab);

    foreach( UIYL_TreeViewInterface * treeView ,m_treeList )
    {
        treeView->SwithBtn()->IsBtnChecked(treeview==treeView);
    }
    R_OnFlashingTreeView(false);

    foreach( UIYL_TreeViewInterface * treeView ,m_treeList )
    {

        treeView->SelectCurPanel(treeview==treeView);
    }

    //todo 9999
    //gIProtocolAgent->DoUploadOperatorLog(QString("online_client_conversation_%d").arg(32+newTab));

}

void UIYL_TreeWidget::R_OnTreeBtnClick()
{
    UIYL_TreeViewInterface * sender = (UIYL_TreeViewInterface *)this->sender();
    OnTreeBtnClick(sender);

}

void UIYL_TreeWidget::OnTreeSearchLineTextChanged(UIYL_TreeViewInterface * sender, const QString &text)
{
    if( sender )
    {
        if (text.isEmpty())
        {
            sender->SearchResultWnd()->hide();
        }
        else
        {
            if (sender->SearchResultWnd() == nullptr)
            {
              //  m_mainTreeSearchResultWnd = new UIYL_MainTreeSearchResultWidget(this);
            }
            sender->SearchText(text);
            // m_mainTree->SearchResultWnd()->setGeometry(5, 67, m_mainTree->SearchLine()->width(), 310);
            sender->SearchResultWnd()->setGeometry(0, 67, this->width(), sender->TreeWidget()->height());
            sender->SearchResultWnd()->show();
            sender->SearchResultWnd()->activateWindow();


        }
    }
}

void UIYL_TreeWidget::R_OnTreeSearchLineTextChanged(const QString &text)
{

    UIYL_TreeViewInterface * sender = (UIYL_TreeViewInterface *)this->sender();
    OnTreeSearchLineTextChanged( sender,text );

}


void UIYL_TreeWidget::HideWnd()
{
     UIYL_TreeViewInterface * treeView = m_treeList.at(m_treeTab);
    if( treeView )
    {
        if (treeView->SearchResultWnd())
        {
            treeView->SearchResultWnd()->hide();
        }
    }
}

bool UIYL_TreeWidget::eventFilter(QObject *obj, QEvent *e)
{
    if (e->type() == QEvent::FocusIn)
    {
        foreach( UIYL_TreeViewInterface * treeView ,m_treeList )
        {
            if (obj == treeView->SearchLine())
            {
                QString text = treeView->SearchLine()->text().trimmed();
                OnTreeSearchLineTextChanged( treeView,text );
                break;
            }
        }
    }
    else if (e->type() == QEvent::FocusOut)
    {
        bool isHide = false;
        foreach( UIYL_TreeViewInterface * treeView ,m_treeList )
        {
            if (obj == treeView->SearchLine() )
            {
                isHide = true;
                break;
            }

        }
        if (isHide ) {
            HideWnd();
        }
    }
    else if (e->type() == QEvent::KeyPress)
    {

        foreach( UIYL_TreeViewInterface * treeView ,m_treeList )
        {
            if (obj == treeView->SearchLine())
            {
                QString text = treeView->SearchLine()->text().trimmed();
                OnTreeSearchLineTextChanged( treeView,text );
                break;
            }
        }

    }

    return QWidget::eventFilter(obj, e);
}


void UIYL_TreeWidget::R_LoginStateChanged(ELOGINSTATUS oldStatus, ELOGINSTATUS newStatus)
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
                //根据设置中心角色权限决定是否显示"内部对话"面板
                bool hasPermission_colleagueList = gSessionService->HasPermissionByKey("colleague");
                if (!hasPermission_colleagueList)
                {
                    m_treeList.at(0)->SwithBtn()->hide();
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

void UIYL_TreeWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        HideWnd();
    }
    QWidget::mousePressEvent(event);
}




void UIYL_TreeWidget::resizeEvent(QResizeEvent *event)
{
	Q_UNUSED(event);
    foreach( UIYL_TreeViewInterface * treeView ,m_treeList )
    {
        treeView->SearchResultWnd()->setGeometry(0, 67, this->width(), treeView->TreeWidget()->height());
    }

}

void UIYL_TreeWidget::R_OnClickMainVisitorSearchItem(QString vid, QString type)
{
    bool isSearch = false;
    foreach( UIYL_TreeViewInterface * treeView ,m_treeList )
    {
       isSearch = treeView->OnClickMainVisitorSearchItem(vid,  type);
       if( isSearch ) break;

    }



}

void UIYL_TreeWidget::R_OnFlashingTreeView(bool isFlashing)
{
    foreach( UIYL_TreeViewInterface * treeView ,m_treeList )
    {
        treeView->OnFlashingTreeView( isFlashing);
    }

}

/*
void UIYL_TreeWidget::R_OnFlashingTreeView(bool isFlashing)
{
    QString strmUncheck = "QToolButton"
                          "{"
                          "background-color:rgba(255,255,255,204);"
                          "border:0px;"
                          "padding: 5px;"
                          "image: url(:/Resources/SVG/%1.svg);"
                          "}"
                          "QToolButton:hover,QToolButton:pressed"
                          "{"
                          "image: url(:/Resources/SVG/%2.svg);"
                          "}";

    if (m_receverMsgFlashing && E_YL_CUSTOMER_TREE == m_treeTab)
    {
        m_number++;
        if (m_number < 4)
        {
            QString strUncheck = strmUncheck.arg("visitor_chat_more_normal", "visitor_chat_more_over");
            m_mainTree->SwithBtn()->SetUnCheckStyle(strUncheck);
            m_mainTree->SwithBtn()->IsBtnChecked(false);
            return;
        }
        else
        {
            m_receverMsgFlashing = false;
            m_number             = 0;
        }
    }

    if (E_YL_MAIN_TREE == m_treeTab || E_YL_RECENT_TREE == m_treeTab)
    {
        QString strUncheck = strmUncheck.arg("visitor_chat_normal", "visitor_chat_over");
        m_mainTree->SwithBtn()->SetUnCheckStyle(strUncheck);
        if (E_YL_RECENT_TREE == m_treeTab)
            m_mainTree->SwithBtn()->IsBtnChecked(false);
    }
    else
    {
        if (isFlashing)
        {
            QString strUncheck = strmUncheck.arg("visitor_chat_more_normal", "visitor_chat_more_over");

            m_mainTree->SwithBtn()->SetUnCheckStyle(strUncheck);
            m_mainTree->SwithBtn()->IsBtnChecked(false);
        }
        else
        {
            QString strUncheck = strmUncheck.arg("visitor_chat_normal", "visitor_chat_over");
            m_mainTree->SwithBtn()->SetUnCheckStyle(strUncheck);
            m_mainTree->SwithBtn()->IsBtnChecked(false);
        }
    }
}
*/
void UIYL_TreeWidget::R_ReceiveMessage(const Im_Message &msg)
{
    if (msg.GetValueByKey("colleagueMessage") == "0")
    {
        QString sendId      = msg.GetValueByKey("fromUserId");
        //todo 9999
        /*
        bool    isColleague = gUIColleagueManager->FindColleagueMemberByID(sendId);
        if (isColleague)
        {
            //同事回复的消息。
        }
        else
        {
            if (sendId == gSessionService->UserInfo()->UserId())
            {
                //自己的自动回复。
            }
            else
            {
                m_receverMsgFlashing = true;
                m_number             = 0;
                R_OnFlashingTreeView(true);
            }
        }
        */
    }
    else if (msg.GetValueByKey("colleagueMessage") == "1")
    {
        //暂时不处理同事消息
    }
}


void UIYL_TreeWidget::R_StartChat(QString customerId)
{
    //选中客服页面。
    if( m_treeList.size() > YL_CUSTOMER_TREE )
    {
       OnTreeBtnClick(m_treeList.at(YL_CUSTOMER_TREE));
    }

}

void UIYL_TreeWidget::R_MainSelectChat(QString vid)
{
    if (m_treeTab != YL_MAIN_TREE)
    {
       OnTreeBtnClick(m_treeList.at(YL_MAIN_TREE));
    }
}

void UIYL_TreeWidget::R_LoadUi(int type )
{
    m_treeList = PlatUiService::GetInstance()->GetTreeViewList();
    Init();

}
void UIYL_TreeWidget::Init()
{
    m_treeTab = E_YL_MAIN_TREE;
    foreach( UIYL_TreeViewInterface * treeView ,m_treeList )
    {
        QLineEdit * line = treeView->SearchLine();
        SwitchButton * button = treeView->SwithBtn();
        QWidget * headWidget = treeView->HeadWidget();
        if( headWidget )
            ui.horizontalLayout_2->addWidget(headWidget);
        else if( button )
            ui.horizontalLayout_2->addWidget(button);

        ui.m_stackedWidget->addWidget(treeView);
        connect(treeView, SIGNAL(S_SwitchBtnClicked()), this, SLOT(R_OnTreeBtnClick()));
        connect(treeView->SearchLine(), SIGNAL(textChanged(const QString &)), this, SLOT(R_OnTreeSearchLineTextChanged(const QString &)));
    }



    connect(CommonSignalService::GetInstance(), SIGNAL(S_MainSelectChat(QString)), this, SLOT(R_MainSelectChat(QString)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_OnFlashingTreeView(bool)), this, SLOT(R_OnFlashingTreeView(bool)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_ReceiveMessage(const Im_Message &)), this, SLOT(R_ReceiveMessage(const Im_Message &)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_StartChat(QString)), this, SLOT(R_StartChat(QString)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_OnClickSearchItem(QString, QString)), this, SLOT(R_OnClickMainVisitorSearchItem(QString, QString)));


    connect(this, SIGNAL(S_ShowNotice_Clicked()), CommonSignalService::GetInstance(), SIGNAL(S_BtnNotice_Clicked()));
    connect(this, SIGNAL(S_BtnCloseNotice_Clicked()), CommonSignalService::GetInstance(), SIGNAL(S_BtnCloseNotice_Clicked()));
    connect(this, SIGNAL(S_OnYLMainTreeSelectTabChanged(E_YL_TREE_TAB, E_YL_TREE_TAB)), CommonSignalService::GetInstance(), SIGNAL(S_OnYLMainTreeSelectTabChanged(E_YL_TREE_TAB, E_YL_TREE_TAB)));
    connect(gSessionService->LoginSignal(), SIGNAL(S_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)), this, SLOT(R_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)));


    foreach( UIYL_TreeViewInterface * treeView ,m_treeList )
    {
        treeView->InitSwitchButton( m_treeList.at(m_treeTab) == treeView );
    }

    OnTreeBtnClick(m_treeList.at(0));
}

