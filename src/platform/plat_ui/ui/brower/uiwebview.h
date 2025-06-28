#ifndef UIWEBVIEW_H
#define UIWEBVIEW_H

#include <QObject>
#include <QUrl>
#include <QString>
#include <QWebEngineCallback>

class IWebView : public QObject {
    Q_OBJECT

public:
    explicit IWebView(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~IWebView() {}

    // 加载URL接口
    virtual void LoadUrl(const QUrl& url) = 0;

    // 获取当前页面的标题
    virtual QString GetTitle() const = 0;

    // 导航控制
    virtual void GoBack() = 0;
    virtual void GoForward() = 0;
    virtual void Reload() = 0;
    virtual void Stop() = 0;
    virtual void runJavaScript(const QString & script) = 0;
    virtual void runJavaScript(const QString & script, const QWebEngineCallback<const QVariant &> &resultCallback) = 0;

signals:

    void LoadFinished(bool success);

    void TitleChanged(const QString& title);
    void UrlChanged(const QUrl& url);
};

#endif // UIWEBVIEW_H
