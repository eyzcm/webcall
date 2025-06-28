#ifndef UIWEBVIEWINTERFACE_H
#define UIWEBVIEWINTERFACE_H


#include <QEvent>
#include <QIcon>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPointer>
#include <QWebEngineCookieStore>
#include <QWebEngineDownloadItem>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWebEngineView>
#include <QtWebChannel>


class CustomCertificateWebEnginePage : public QWebEnginePage
{
public:
	CustomCertificateWebEnginePage();
    virtual bool certificateError(const QWebEngineCertificateError &certificateError);
};

class UiWebViewInterface : public QWebEngineView
{
    Q_OBJECT
public:

    UiWebViewInterface(QWidget *parent = nullptr);
    ~UiWebViewInterface();
    virtual void    setUrl(const QUrl &url);
    QString GetAppdataCacheDir();
    void    clearCache(); //清理缓存
    int     m_Tabnum;  //页签编号
    QString m_loadUrl; //加载的url
    //  signals:
    //    void loadStarted();                     //页面加载时发出
    //    void loadProgress(int progress);        //页面加载进度
    //    void loadFinished(bool ok);             //页面加载结束，ok表示加载成功或失败
    //    void iconUrlChanged(const QUrl &url);   //网页图标URL改变时发出
    //    void selectionChanged();                //网页中选择内容改变时发出
    //    void titleChanged(const QString &title);//网页标题改变时发出
    //    void iconChanged(const QIcon &icon);    //网页图标改变时发出
    //    void urlChanged(const QUrl &url);       //加载的URL改变时发出
    //    void renderProcessTerminated(QWebEnginePage::RenderProcessTerminationStatus terminationStatus, int exitCode);//进程非正常退出时发出，terminationStatus表示退出状态，exitCode表示退出状态码
protected:
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type) override;

signals:
    void html(QString sHtml);

protected slots:
    void loadFinishedOK(bool l);
    void webDownload(QWebEngineDownloadItem *item);
    void on_finished();
    void on_downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void on_iconChanged(QIcon icon);
    void R_gotoNewWindow();
    void OnRenderProcessTerminated(QWebEnginePage::RenderProcessTerminationStatus, int);

signals:

};

#endif // UIWEBVIEWINTERFACE_H
