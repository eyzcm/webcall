#ifndef BaseJsCommHandle_H
#define BaseJsCommHandle_H



#include "data/CommInfo.h"
#include <QObject>
#include <QStringList>
#include "plat_ui_global.h"

class PLAT_UI_EXPORT BaseJsCommHandle : public QObject
{
    Q_OBJECT

public:
    BaseJsCommHandle();
    ~BaseJsCommHandle();

public:
    QWebEngineView *GetCurrentView();
    void                        SetWebView(QWebEngineView *obj, const QString &objName, bool toaddJavaScript = true);
    void                        addJavaScript();
    virtual void                SetUrl(const QString &url);
    virtual void                LoadFinished()
    {
    }
    virtual void LoadFinished(bool isl)
    {
        Q_UNUSED(isl);
    }
    virtual void AlertCopyTip(QString text, QString num)
    {
    }
    void ReloadPage();

    void SizeChanged(int width, int height);
    void isLoadstatus(bool s)
    {
        m_isLoadOk = s;
    }


signals:
    void S_closeWnd();

private slots:
    void R_WebLoadfinished(bool isOk);

public slots:
    void    JsCloseWnd();
    QString GetTextNum(QString body);


    void    IsLoadFinished();
    void    SystemOpenUrl(const QString &url, const QString &type);
    void    GotoUrl(const QString &url);




protected:
    virtual void CallJs(const QString &js);

protected:
    QWebEngineView *m_webBaseView;
    bool                        m_isLoadOk;
    QString                     m_objName;
};

#endif // BaseJsCommHandle_H
