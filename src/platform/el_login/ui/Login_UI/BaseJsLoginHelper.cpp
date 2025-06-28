#include "BaseJsLoginHelper.h"
#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>

#include "utils/DefineHelper.h"

#include "ui/uimessagebox.h"

#include "utils/jsonhelper.h"
#include "utils/UtilsHelper.h"
#include "service/util_service.h"



BaseJsLoginHelper::BaseJsLoginHelper() : QObject()
{
    m_isLoadOk    = false;
    m_webBaseView = NULL;

}

BaseJsLoginHelper::~BaseJsLoginHelper()
{
}

void BaseJsLoginHelper::SetWebView(UI_WebView_Login *obj, const QString &objName)
{
    if (obj == NULL || obj->page() == NULL)
    {
        Q_ASSERT(false);
        return;
    }
    m_objName     = objName;
    m_webBaseView = obj;


    QWebChannel *channel = new QWebChannel(m_webBaseView);
     channel->registerObject(objName, this);
     m_webBaseView->page()->setWebChannel(channel);

    // Qt5中没有这个信号
    connect(m_webBaseView, SIGNAL(loadFinished(bool)), this, SLOT(R_WebLoadfinished(bool)));
}

void BaseJsLoginHelper::SetUrl(const QString &url)
{
     m_webBaseView->setUrl( QUrl(QString("file:///%1").arg(url)) );

}

void BaseJsLoginHelper::ReloadPage()
{
    QString jsVal = "reloadWindow()";
    BaseJsLoginHelper::CallJs(jsVal);
}

void BaseJsLoginHelper::R_WebLoadfinished(bool isOk)
{

    QWebChannel *channel = new QWebChannel(m_webBaseView);
           channel->registerObject(m_objName, this);
           m_webBaseView->page()->setWebChannel(channel);

    LoadFinished();
    LoadFinished(isOk);
}

void BaseJsLoginHelper::OnTest(const QString &arg)
{
    GSCRIPTLOG(arg);
}

void BaseJsLoginHelper::OnLog(const QString &arg)
{
    GSCRIPTLOG(arg);
}

void BaseJsLoginHelper::IsLoadFinished()
{
    m_isLoadOk = true;
    foreach (QString js, m_cacheCmdList)
    {
        CallJs(js);
    }
    m_cacheCmdList.clear();
}

void BaseJsLoginHelper::GotoUrl(const QString &url)
{
    if (!url.isEmpty())
    {
        QDesktopServices::openUrl(QUrl(url));
    }
}

void BaseJsLoginHelper::SystemOpenUrl(const QString &url, const QString &type)
{
    if (!url.isEmpty())
    {
        QDesktopServices::openUrl(QUrl(url));
    }
}

void BaseJsLoginHelper::CallJs(const QString &js)
{
    if (m_webBaseView)
    {
        GSCRIPTLOG(js);

        if (!m_isLoadOk)
        {
            m_cacheCmdList.push_back(js);
        }
        else
        {
             m_webBaseView->page()->runJavaScript(js);

            qDebug() << "CallJs   " << js;
            GSCRIPTLOG("CallJs ok");
        }
    }
}


void BaseJsLoginHelper::DoDrump()
{
    return;
    QString jsVal = "drump()";
    CallJs(jsVal);
}


void BaseJsLoginHelper::ReSetToken(QString token)
{
    QString jsVal = QString("ReSetToken(\"%1\")").arg(token);
    CallJs(jsVal);
}

void BaseJsLoginHelper::SizeChanged(int width, int height)
{
     return;
    Json::Value val;
    val["width"]  = QString::number(width).toStdString();
    val["height"] = QString::number(height).toStdString();

    QString jsonStr = QString::fromStdString(val.toStyledString());
    QString jsVal   = QString("OnSizeChanged(%1)").arg(jsonStr);
    CallJs(jsVal);
}

void BaseJsLoginHelper::DoScrollBottom()
{
    QString jsVal = QString("scrollBottom()");
    CallJs(jsVal);
}

UI_WebView_Login *BaseJsLoginHelper::GetCurrentView()
{
    return m_webBaseView;
}

//曹振超 2019年9月27日 添加共用的提示弹窗
void BaseJsLoginHelper::R_PromptWidget(QString msg)
{
    UIMessageBox *box = new UIMessageBox;
    box->setWindowTitle(tr("提示"));
    box->SetContentTip(msg);
    box->SetCancelBtnVisible(false);
    box->exec();
}


void BaseJsLoginHelper::R_MessageBox(QString msg, QString rtype)
{
    UIMessageBox *box = new UIMessageBox;
    box->setWindowTitle(tr("提示"));
    box->SetContentTip(msg);
    box->SetCancelBtnVisible(true);
    int     re    = box->exec();
    QString jsVal = QString("JSMessageBoxComplete('%1','%2')").arg(QString::number(re), rtype);
    CallJs(jsVal);
}


void BaseJsLoginHelper::JsCloseWnd()
{
    Q_EMIT S_closeWnd();
}


void BaseJsLoginHelper::LoadFinished(bool isl)
{

}
