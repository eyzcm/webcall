#include "uiwebengineview.h"

UIWebEngineView::UIWebEngineView(QWidget* parent ) : IWebView(parent)
{
        m_webView = new QWebEngineView(parent);

        // 连接信号
        connect(m_webView, &QWebEngineView::loadFinished, this, &UIWebEngineView::LoadFinished);
        connect(m_webView, &QWebEngineView::titleChanged, this, &UIWebEngineView::TitleChanged);
        connect(m_webView, &QWebEngineView::urlChanged, this, &UIWebEngineView::UrlChanged);
}

UIWebEngineView::~UIWebEngineView()
{
        delete m_webView;
}

void UIWebEngineView::LoadUrl(const QUrl& url)
{
        m_webView->load(url);
}

QString UIWebEngineView::GetTitle() const
{
        return m_webView->title();
}

void UIWebEngineView::GoBack()
{
        m_webView->back();
}

void UIWebEngineView::GoForward()
{
        m_webView->forward();
}

void UIWebEngineView::Reload()
{
        m_webView->reload();
}

void UIWebEngineView::Stop()
{
        m_webView->stop();
}

void UIWebEngineView::runJavaScript(const QString & script)
{
    m_webView->page()->runJavaScript( script);
}

void UIWebEngineView::runJavaScript(const QString & script, const QWebEngineCallback<const QVariant &> &resultCallback)
{
    m_webView->page()->runJavaScript( script, resultCallback);
}

