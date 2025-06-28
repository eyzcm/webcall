#ifndef UIWEBPAGEVIEW_H
#define UIWEBPAGEVIEW_H

#include <QWebEngineView>
#include <QWebEngineDownloadItem>


class UIWebPageView : public QWebEngineView
{
    Q_OBJECT

public:
    explicit UIWebPageView(QWidget *parent = nullptr);

    // 加载URL
    void LoadUrl(const QString &url);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
    void OnDownloadRequested(QWebEngineDownloadItem *download);

private:
    QMenu *m_contextMenu;
};

#endif // UIWEBPAGEVIEW_H
