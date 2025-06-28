#ifndef PLATVIEWUISERVICE_H
#define PLATVIEWUISERVICE_H

#include <QObject>
#include "interface/view_interface.h"
class UI_Widget_MainWindow;
class UINotifyMessageBox;

class PlatViewUiService : public  View_interface
{
    Q_OBJECT
public:
    PlatViewUiService();
    virtual EasyLiaoMainWnd * CreateMainWnd();
    virtual void ClearMainWnd( EasyLiaoMainWnd * wnd) ;

    virtual IBaseMainWnd * CreateBaseMainWnd() ;
    virtual void ClearBaseMainWnd(IBaseMainWnd * wnd) ;


    virtual void DoGotoUrl(QString url) ;
    virtual void addNavigationBar_button(QToolButton *webbut) ;
    virtual void OpenAIWindow(QString key, QString json) ;

    virtual void ShowNotifyCommonBox(QObject * obj,const QString& title, const QString& text, const QString& url, const QString& btnTxt, const QString& cateGory, bool isCloseEnable,bool isIe) ;
    virtual void ShowNotifyCommonBox(const QString& title, const QString& text, const QString& url, const QString& btnTxt);

signals:
private:
    UINotifyMessageBox* ShowNotifyCommonBox(const QString& title, const QString& text, const QString& url, const QString& btnTxt, const QString& cateGory, bool isCloseEnable);

private:
    UI_Widget_MainWindow * m_mainWnd;

};

#endif // PLATVIEWUISERVICE_H
