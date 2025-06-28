#include "BaseJsCommHandle.h"
#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
#include "utils/DefineHelper.h"



#include "model/uiplatcommondatamodel.h"
#include "protocol/httpcmd.h"

#include "session/el_sessionservice.h"


#include "service/util_service.h"
#include "utils/jsonhelper.h"
#include "utils/UtilsHelper.h"


BaseJsCommHandle::BaseJsCommHandle()
    : QObject()
{
    m_isLoadOk    = false;
    m_webBaseView = NULL;
}

BaseJsCommHandle::~BaseJsCommHandle()
{
}

void BaseJsCommHandle::SetWebView(QWebEngineView *obj, const QString &objName, bool toaddJavaScript)
{
    if (obj == NULL || obj->page() == NULL)
    {
        Q_ASSERT(false);
        return;
    }
    m_objName     = objName;
    m_webBaseView = obj;
    if (toaddJavaScript)
    {
        addJavaScript();
    }
}

void BaseJsCommHandle::addJavaScript()
{
    if (m_webBaseView)
    {
        QWebChannel *channel = new QWebChannel(m_webBaseView);
        channel->registerObject(m_objName, this);
        m_webBaseView->page()->setWebChannel(channel);

        connect(m_webBaseView, SIGNAL(loadFinished(bool)), this, SLOT(R_WebLoadfinished(bool)));
    }
}

void BaseJsCommHandle::SetUrl(const QString &url)
{
    QString weburl = url;
    if( (weburl.indexOf("file://") != 0)&&(weburl.indexOf("http") != 0))
    {
        weburl = QString("file:///%1").arg(url);
    }
    m_webBaseView->setUrl( QUrl(weburl));

}

void BaseJsCommHandle::ReloadPage()
{
    QString jsVal = "reloadWindow()";
    BaseJsCommHandle::CallJs(jsVal);
}

void BaseJsCommHandle::R_WebLoadfinished(bool isOk)
{
    LoadFinished();
    LoadFinished(isOk);
}

void BaseJsCommHandle::IsLoadFinished()
{
    m_isLoadOk = true;

}

void BaseJsCommHandle::GotoUrl(const QString &url)
{
    if (!url.isEmpty())
    {
        QDesktopServices::openUrl(QUrl(url));
    }
}

void BaseJsCommHandle::SystemOpenUrl(const QString &url, const QString &type)
{
    Q_UNUSED(type);
    if (!url.isEmpty())
    {
        QDesktopServices::openUrl(QUrl(url));
    }
}

void BaseJsCommHandle::CallJs(const QString &js)
{
    if (m_webBaseView)
    {
        if (m_isLoadOk)
        {
            m_webBaseView->page()->runJavaScript(js);

        }

    }
}


QWebEngineView *BaseJsCommHandle::GetCurrentView()
{
    return m_webBaseView;
}

QString BaseJsCommHandle::GetTextNum(QString body)
{
    QString text = "";
    QString num  = UtilsHelper::GetNumberByString(body);
    if (!num.isEmpty())
    {
        AlertCopyTip(text, num);
        QClipboard *board = QApplication::clipboard();
        board->setText(num);
    }
    return num;
}

void BaseJsCommHandle::JsCloseWnd()
{
    Q_EMIT S_closeWnd();
}


void BaseJsCommHandle::SizeChanged(int width, int height)
{
    Q_UNUSED(width);
    Q_UNUSED(height);
    return;
}
