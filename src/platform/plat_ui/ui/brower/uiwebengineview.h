#ifndef UIWEBENGINEVIEW_H
#define UIWEBENGINEVIEW_H

#include "UIWebView.h"
#include <QWebEngineView>

class UIWebEngineView : public IWebView {
    Q_OBJECT

private:
    QWebEngineView* m_webView;

public:
    explicit UIWebEngineView(QWidget* parent = nullptr);

    ~UIWebEngineView() override ;


    void LoadUrl(const QUrl& url) override ;


    QString GetTitle() const override ;


    void GoBack() override ;


    void GoForward() override ;


    void Reload() override ;


    void Stop() override ;

    void runJavaScript(const QString & script);
    void runJavaScript(const QString & script, const QWebEngineCallback<const QVariant &> &resultCallback);

};

#endif // UIWEBENGINEVIEW_H
