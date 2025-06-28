#ifndef VIEW_INTERFACE_H
#define VIEW_INTERFACE_H

#include "interface/easyliaomainwnd.h"
#include "interface/ibasemainwnd.h"

#include <QObject>
#include <QToolButton>
#include "plat_ui_global.h"

class PLAT_UI_EXPORT View_interface:public QObject
{
    Q_OBJECT
public:
    View_interface();

    virtual EasyLiaoMainWnd * CreateMainWnd() = 0;
    virtual void ClearMainWnd( EasyLiaoMainWnd * wnd) = 0;

    virtual IBaseMainWnd * CreateBaseMainWnd() = 0;
    virtual void ClearBaseMainWnd(IBaseMainWnd * wnd) = 0;


    virtual void DoGotoUrl(QString url) = 0;
    virtual void addNavigationBar_button(QToolButton *webbut) = 0;
    virtual void OpenAIWindow(QString key, QString json) = 0;

    virtual void ShowNotifyCommonBox(QObject * obj,const QString& title, const QString& text, const QString& url, const QString& btnTxt, const QString& cateGory, bool isCloseEnable,bool isIe) = 0;
    virtual void ShowNotifyCommonBox(const QString& title, const QString& text, const QString& url, const QString& btnTxt)= 0;


};

#endif // VIEW_INTERFACE_H
