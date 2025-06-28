#include "platviewuiservice.h"

#include <baseWnd.h>

#include <ui/common/BaseMainWnd.h>
#include <ui/UI_Widget_MainWindow.h>

#include <uinotifymessagebox.h>
#include "interface/el_topfuninterface.h"
#include "interface/ui_widget_commonphrasescard_interface.h"
#include "interface/widget_user_interface.h"

PlatViewUiService::PlatViewUiService() : View_interface()
{

    m_mainWnd = nullptr;
}

EasyLiaoMainWnd * PlatViewUiService::CreateMainWnd()
{
    if (m_mainWnd == nullptr)
    {
        UI_Widget_MainWindow * wnd = new UI_Widget_MainWindow();

        EL_TopFunInterface * topWnd = new EL_TopFunInterface(wnd->getWidget("widget_top_right_rect"));
        wnd->AddTopWnd(topWnd);

        wnd->AddStatisticsPanel(new UI_Widget_Statistics_Interface(wnd->getWidget("bottom_rect_bg")));
        wnd->AddCommonPhrasesPanel(new UI_Widget_CommonPhrasesCard_Interface(wnd->getWidget("commonPhrases_card_panel_rect")));
        wnd->AddWidgetUser(new widget_user_interface(wnd->getWidget("widget_top_rect")));


        m_mainWnd = wnd;
    }

    return m_mainWnd;

}

void PlatViewUiService::ClearMainWnd( EasyLiaoMainWnd * wnd)
{
    wnd->deleteLater();
}

IBaseMainWnd * PlatViewUiService::CreateBaseMainWnd()
{

    BaseMainWnd * wnd = new BaseMainWnd( nullptr);
    QString style = "{color: rgba(255, 255, 255,200);background-color: rgba(255, 255, 255,0);}";
    wnd->SetTitleTextStyle(style);
    wnd->SetCommonButtonStyle("white", "min");
    wnd->SetCommonButtonStyle("white", "max");
    wnd->SetCommonButtonStyle("white", "store");
    wnd->SetCommonButtonStyle("white", "close");

    wnd->InitSystemTrayIcon(true);


    wnd->LoadMenuNavWnd();


    return wnd;

}
void PlatViewUiService::ClearBaseMainWnd(IBaseMainWnd * wnd)
{
    wnd->deleteLater();
}


void PlatViewUiService::DoGotoUrl(QString url)
{

}
void PlatViewUiService::addNavigationBar_button(QToolButton *webbut)
{

}
void PlatViewUiService::OpenAIWindow(QString key, QString json)
{

}

void PlatViewUiService::ShowNotifyCommonBox(QObject * obj,const QString& title, const QString& text, const QString& url, const QString& btnTxt, const QString& cateGory, bool isCloseEnable,bool isIe)
{
    UINotifyMessageBox *wnd = ShowNotifyCommonBox(title, text, url, btnTxt, cateGory, isCloseEnable);
    if( isIe ) wnd->SetUrlType(true);
    if( obj )
    {
        connect(wnd, SIGNAL(S_GotoUrlBegin(QString)), obj, SLOT(R_UINotifyMessageBoxClosed(QString)));
    }
}

void PlatViewUiService::ShowNotifyCommonBox(const QString& title, const QString& text, const QString& url, const QString& btnTxt)
{
    QRect rect = QApplication::desktop()->availableGeometry();
    int   nX   = rect.width() - 360;
    int   nY   = rect.height() - 210;

    UINotifyMessageBox* wnd = new UINotifyMessageBox(nullptr);
    wnd->SetContentTip(text);
    wnd->SetLinkBtnText(btnTxt);
    wnd->SetLinkUrl(url);

    BaseWnd* pBaseWnd = new BaseWnd();
    pBaseWnd->setFixedSize(340, 170);
    pBaseWnd->SetWndTitle(title);
    pBaseWnd->SetWndIcon(":/Resources/Image/easy.ico");
    pBaseWnd->SetTitlePanelIcon(":/Resources/Image/easy.ico");
    pBaseWnd->HideMaxBtn();
    pBaseWnd->HideMinBtn();
    pBaseWnd->SetCloseType(1);
    pBaseWnd->AddContentWidget(wnd);
    pBaseWnd->move(nX, nY);
    pBaseWnd->show();

}


UINotifyMessageBox* PlatViewUiService::ShowNotifyCommonBox(const QString& title, const QString& text, const QString& url, const QString& btnTxt, const QString& cateGory, bool isCloseEnable)
{
    QRect rect = QApplication::desktop()->availableGeometry();
    int   nX   = rect.width() - 360;
    int   nY   = rect.height() - 210;

    UINotifyMessageBox* wnd = new UINotifyMessageBox;
    wnd->SetContentTip(text);
    wnd->SetLinkBtnText(btnTxt);
    wnd->SetLinkUrl(url);
    wnd->SetCategory(cateGory);
    wnd->IsCloseEnable(isCloseEnable);

    BaseWnd* pBaseWnd = new BaseWnd();
    pBaseWnd->setFixedSize(340, 170);
    pBaseWnd->SetWndTitle(title);
    pBaseWnd->SetWndIcon(":/Resources/Image/easy.ico");
    pBaseWnd->SetTitlePanelIcon(":/Resources/Image/easy.ico");
    pBaseWnd->HideMaxBtn();
    pBaseWnd->HideMinBtn();
    pBaseWnd->SetCloseType(1);
    pBaseWnd->AddContentWidget(wnd);
    pBaseWnd->move(nX, nY);
    pBaseWnd->show();
    return wnd;
}
