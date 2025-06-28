#include "stdafx.h"
#include "uiwebpageview.h"

UIWebPageView::UIWebPageView(QWidget *parent) : QWebEngineView(parent)
{
    // 初始化右键菜单
    m_contextMenu = new QMenu(this);

    // 连接下载请求
    connect(this->page()->profile(), &QWebEngineProfile::downloadRequested, this, &UIWebPageView::OnDownloadRequested);
}

void UIWebPageView::LoadUrl(const QString &url)
{
    setUrl(QUrl(url));
}

void UIWebPageView::contextMenuEvent(QContextMenuEvent *event)
{
    m_contextMenu->exec(event->globalPos());
}

void UIWebPageView::OnDownloadRequested(QWebEngineDownloadItem *download)
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save File"), download->path());
    if (!path.isEmpty()) {
        download->setPath(path);
        download->accept();
    }
}
