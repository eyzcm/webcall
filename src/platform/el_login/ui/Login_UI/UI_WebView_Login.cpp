#include "UI_WebView_Login.h"
#include <QDesktopServices>
#include <QFileDialog>
#include "ui/baseWnd.h"
#include "utils/utilshelper.h"

UI_WebView_Login::UI_WebView_Login(QWidget *parent) : QWebEngineView(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);            //右键发送 [signal] void customContextMenuRequested(const QPoint &pos)
    QNetworkProxyFactory::setUseSystemConfiguration(false); //关掉使用系统代理设定 处理网页加载慢
    page()->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    page()->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    page()->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);

    QNetworkDiskCache *diskCache = new QNetworkDiskCache(this);
    diskCache->setCacheDirectory(GetAppdataCacheDir()); //设置缓存地址

    page()->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true); //开启缓存 需要定期清理缓存
    // page()->setBackgroundColor(Qt::transparent);  //设置网页背景透明,该设置会导致页面无法被其他窗口置顶显示
    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(loadFinishedOK(bool)));
    //connect(page()->profile(), SIGNAL(downloadRequested(QWebEngineDownloadItem *)), this, SLOT(webDownload(QWebEngineDownloadItem *)));

    connect(this, &UI_WebView_Login::titleChanged, this, &UI_WebView_Login::setWindowTitle);
    connect(this, &UI_WebView_Login::iconChanged, this, &UI_WebView_Login::on_iconChanged);

    setAcceptDrops(false); //取消拖拽功能



    QWebEngineProfile *engineProfile = page()->profile();
    engineProfile->clearHttpCache();

    QWebEngineCookieStore *cookie = engineProfile->cookieStore();
    cookie->deleteAllCookies();
}

UI_WebView_Login::~UI_WebView_Login()
{
    if (page())
    {
        page()->deleteLater();
    }
    deleteLater();
}

void UI_WebView_Login::setUrl(const QUrl &url)
{
    m_loadUrl = url.toString();
    qDebug() << "加载网页开始 " << m_loadUrl << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "\n";
    QWebEngineView::setUrl(url); //与setUrl的区别在与,load在数据加载一定程度后,才会显示页面
}

QString UI_WebView_Login::GetAppdataCacheDir()
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


void UI_WebView_Login::loadFinishedOK(bool l)
{
    qDebug() << "加载网页完成 " << l << m_loadUrl << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "\n";
}


void UI_WebView_Login::on_finished()
{
    qDebug() << "UI_WebView_Login::on_finished";
}

void UI_WebView_Login::on_downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug() << "UI_WebView_Login::on_downloadProgress bytesReceived:" << bytesReceived << "  bytesTotal:" << bytesTotal;
}

void UI_WebView_Login::on_iconChanged(QIcon icon)
{
    setWindowIcon(icon);
}
