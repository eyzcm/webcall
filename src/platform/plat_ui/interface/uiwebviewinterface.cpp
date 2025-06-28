#include "uiwebviewinterface.h"
#include <QDesktopServices>
#include <QFileDialog>
#include "ui/baseWnd.h"
#include "utils/utilshelper.h"
#include "server/platuiservice.h"





CustomCertificateWebEnginePage::CustomCertificateWebEnginePage()
{
}

bool CustomCertificateWebEnginePage::certificateError(const QWebEngineCertificateError &certificateError)
{
    return true;
}

UiWebViewInterface::UiWebViewInterface(QWidget *parent) : QWebEngineView(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setContextMenuPolicy(Qt::CustomContextMenu);            //右键发送 [signal] void customContextMenuRequested(const QPoint &pos)
    setPage(new CustomCertificateWebEnginePage());//忽略证书错误
    QNetworkProxyFactory::setUseSystemConfiguration(false); //关掉使用系统代理设定 处理网页加载慢
    page()->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    page()->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    page()->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);

    QNetworkDiskCache *diskCache = new QNetworkDiskCache(this);
    diskCache->setCacheDirectory(GetAppdataCacheDir()); //设置缓存地址

    page()->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true); //开启缓存 需要定期清理缓存
    // page()->setBackgroundColor(Qt::transparent);  //设置网页背景透明,该设置会导致页面无法被其他窗口置顶显示
    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(loadFinishedOK(bool)));
    connect(page()->profile(), SIGNAL(downloadRequested(QWebEngineDownloadItem *)), this, SLOT(webDownload(QWebEngineDownloadItem *)));

    connect(this, &UiWebViewInterface::titleChanged, this, &UiWebViewInterface::setWindowTitle);
    connect(this, &UiWebViewInterface::iconChanged, this, &UiWebViewInterface::on_iconChanged);
    connect(this, &UiWebViewInterface::renderProcessTerminated, this, &UiWebViewInterface::OnRenderProcessTerminated); //白屏幕,重新加载url

    setAcceptDrops(false); //取消拖拽功能
    m_Tabnum = -1;
    clearCache();
}

UiWebViewInterface::~UiWebViewInterface()
{
    if (page())
    {
        page()->deleteLater();
    }
    deleteLater();
}

void UiWebViewInterface::setUrl(const QUrl &url)
{
    m_loadUrl = url.toString();
    qDebug() << "加载网页开始 " << m_loadUrl << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "\n";
    QWebEngineView::setUrl(url); //与setUrl的区别在与,load在数据加载一定程度后,才会显示页面
}

QString UiWebViewInterface::GetAppdataCacheDir()
{

    QString pathDir = UtilsHelper::GetAppdataCacheDir();
    QDir    dir;
    if (!dir.exists(pathDir))
    {
        dir.mkdir(pathDir);
    }
    pathDir += "/webcache";
    if (!dir.exists(pathDir))
    {
        dir.mkdir(pathDir);
    }
    return pathDir;
}

void UiWebViewInterface::clearCache()
{
    QWebEngineProfile *engineProfile = page()->profile();
    engineProfile->clearHttpCache();

    QWebEngineCookieStore *cookie = engineProfile->cookieStore();
    cookie->deleteAllCookies();
}

QWebEngineView *UiWebViewInterface::createWindow(QWebEnginePage::WebWindowType type)
{
    UiWebViewInterface *newWnd = new UiWebViewInterface(0);
    connect(newWnd, &UiWebViewInterface::loadStarted, newWnd, &UiWebViewInterface::R_gotoNewWindow);
    return newWnd;
}

void UiWebViewInterface::loadFinishedOK(bool l)
{
    qDebug() << "加载网页完成 " << l << m_loadUrl << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "\n";
}

void UiWebViewInterface::webDownload(QWebEngineDownloadItem *item)
{
    if (item->state() == QWebEngineDownloadItem::DownloadRequested) //未知情况会出现一次下载请求出发多次 downloadRequested 信号
    {
        QString webDownloadfilename = item->suggestedFileName();
        webDownloadfilename         = webDownloadfilename.remove(QRegExp("[\\\\/:*?\"<>|]")); //过滤非法文件名  windows现在已知的文件名非法字符有 \ / : * ? " < > |
        //webDownloadfilename         = UtilsViewHelper::GetDownLoadFileDir() + webDownloadfilename;

        QFileInfo filei(webDownloadfilename);
        QString   suffix = filei.suffix();
        if (suffix.isEmpty())
        {
            suffix = "All file(*.*)";
        }
        else
        {
            suffix = QString("%1(*.%1);;All file(*.*)").arg(suffix);
        }
        QString strFileName = QFileDialog::getSaveFileName(0, tr("另存为"), webDownloadfilename, suffix);

        QFileInfo appInfo(strFileName);
        if (!strFileName.isEmpty())
        {
            item->setDownloadDirectory(appInfo.path());
            item->setDownloadFileName(appInfo.fileName());
            connect(item, &QWebEngineDownloadItem::finished, this, &UiWebViewInterface::on_finished);
            connect(item, &QWebEngineDownloadItem::downloadProgress, this, &UiWebViewInterface::on_downloadProgress);
            item->accept();
        }
        else
        {
            item->cancel();
        }
    }
}

void UiWebViewInterface::on_finished()
{
}

void UiWebViewInterface::on_downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
}

void UiWebViewInterface::on_iconChanged(QIcon icon)
{
    setWindowIcon(icon);
}

void UiWebViewInterface::R_gotoNewWindow()
{
    QString strhost = url().host();
    QString strUrl  = page()->requestedUrl().toString();
    if (!strUrl.isEmpty())
    {
        if (strhost.contains("easyliao", Qt::CaseInsensitive))
        {
            gViewService->DoGotoUrl(strUrl);
            //IUIController::GetInstance()->DoGotoUrl(strUrl);
        }
        else //非公司域名网址,使用本地默认浏览器打开
        {
            QDesktopServices::openUrl(QUrl(strUrl));
        }
    }
    this->deleteLater();
}

void UiWebViewInterface::OnRenderProcessTerminated(QWebEnginePage::RenderProcessTerminationStatus, int)
{
    QTimer::singleShot(0, [this] { reload(); });
}

