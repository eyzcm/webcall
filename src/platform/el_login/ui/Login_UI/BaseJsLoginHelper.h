#ifndef BaseJsLoginHelper_H
#define BaseJsLoginHelper_H

#include "ui/Login_UI/UI_WebView_Login.h"

#include "data/CommInfo.h"
#include <QObject>
#include <QStringList>


class BaseJsLoginHelper : public QObject
{
    Q_OBJECT

public:
    BaseJsLoginHelper();
    ~BaseJsLoginHelper();

public:
	UI_WebView_Login *GetCurrentView();
    void                        SetWebView(UI_WebView_Login *obj, const QString &objName);
    virtual void                SetUrl(const QString &url);
    virtual void                LoadFinished()
    {
    }
    virtual void LoadFinished(bool isl);


    void ReloadPage();
    void DoDrump();
    void ReSetToken(QString token);
    void SizeChanged(int width, int height);
    void DoScrollBottom();

signals:
    void S_closeWnd();

private slots:
    void R_WebLoadfinished(bool isOk);

public slots:
    void JsCloseWnd();
    void OnTest(const QString &arg);
    void OnLog(const QString &arg);


    virtual void IsLoadFinished();

    void SystemOpenUrl(const QString &url, const QString &type);

    void    GotoUrl(const QString &url);



    void R_PromptWidget(QString msg);
    void R_MessageBox(QString msg, QString rtype);   // msg 提示框信息  rtype类型.需要将该值传回给js


protected:
    virtual void CallJs(const QString &js);

protected:
	UI_WebView_Login *m_webBaseView;
    QWebChannel                *m_channel;
    bool                        m_isLoadOk;
    QString                     m_objName;
    QStringList                 m_cacheCmdList;

    QString                     m_V;
    //设备型号
        QString m_device;
        QString m_userAgent;
};

#endif // BaseJsLoginHelper_H
