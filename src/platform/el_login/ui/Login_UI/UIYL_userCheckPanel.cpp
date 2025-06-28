#include "UIYL_userCheckPanel.h"
//#include "control/geasyliaoframework.h"

#include <QIcon>

#include <utils/UtilsHelper.h>

UIYL_UserCheckPanel::UIYL_UserCheckPanel(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
    m_webView = new UI_WebView_Login(this);
    this->setWindowIcon(QIcon(":/Resources/Image/easy.ico"));
}

UIYL_UserCheckPanel::~UIYL_UserCheckPanel()
{
}

void UIYL_UserCheckPanel::Init(int type, QString url, int isPwdSafed)
{
    m_isCheck    = type;
    m_checkUrl   = url;
    m_isPwdSafed = isPwdSafed;
    m_webView->adjustSize();
    m_userCheckJsObj = new JsUserCheckHelper;
    m_userCheckJsObj->SetWebView(m_webView, "jsObj");
#ifdef _Q_OS_WIN
    QString pathDir = QString("%1/%2").arg(UtilsHelper::GetRunAppDir()).arg(m_checkUrl);
#else
    QString pathDir;
    if( m_checkUrl.indexOf("file://") == 0 ) pathDir = m_checkUrl;
    else pathDir = QString("file:///%1/%2").arg(UtilsHelper::GetRunAppDir()).arg(m_checkUrl);
#endif
    m_userCheckJsObj->SetUrl(pathDir);
    m_userCheckJsObj->SetIsPwdSafed(m_isPwdSafed);
    connect(m_userCheckJsObj, SIGNAL(S_closeWnd()), this, SLOT(R_closeWnd()));
    connect(m_userCheckJsObj, SIGNAL(S_quitSys()), this, SLOT(R_quitSys()));

    m_webView->page()->setBackgroundColor(Qt::transparent); //网页透明背景
}

void UIYL_UserCheckPanel::resizeEvent(QResizeEvent *e)
{
    if (m_userCheckJsObj)
    {
        this->m_webView->resize(e->size().width(), e->size().height());
    }
}

void UIYL_UserCheckPanel::InitData()
{
    if (m_userCheckJsObj)
    {
        m_userCheckJsObj->SetInitData();
    }
}

int UIYL_UserCheckPanel::CheckModify()
{
    if (m_userCheckJsObj)
    {
        return m_userCheckJsObj->CheckModify();
    }
    else
        return 0;
}

void UIYL_UserCheckPanel::closeEvent(QCloseEvent *event)
{
    if (this->m_isCheck == 2)
        event->ignore();
    return;
}

void UIYL_UserCheckPanel::R_closeWnd()
{
    Q_EMIT S_closeWnd();
}
void UIYL_UserCheckPanel::R_quitSys()
{
    qApp->exit(0);
}
