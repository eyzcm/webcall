#ifndef IBASEMAINWND_H
#define IBASEMAINWND_H

#include "utils/Plat_EnumDefine.h"
#include "plat_ui_global.h"
#include <qobject.h>
#include <ui/MyWidgetBase.h>

class PLAT_UI_EXPORT IBaseMainWnd : public MyWidgetBase
{
    Q_OBJECT

public:
    IBaseMainWnd(QWidget *parent = nullptr);

    virtual void     DoOnOpenMainWndWnd()                                  = 0;
    virtual void     SetWndTitle(QString title)                            = 0;
    virtual void     DoSetWindowTitle(QString title)                       = 0;
    virtual void     SetWndIcon(const QString &iconUrl)                    = 0;
    virtual void     AddContentWidget(QWidget *wnd, bool isVisible = true) = 0;
    virtual void     RemoveCenterWidget()                                  = 0;
    virtual void     DeleteCenterWidget()                                  = 0;
    virtual QWidget *ContentWidget()                                       = 0;
    virtual QWidget *Widget()                                              = 0;
    virtual bool     HasCenterWidget()                                     = 0;
    virtual void     AddExtendBtnWidget(QWidget *wnd, bool isVisible)      = 0;
    virtual void     SetTitleStyle(const QString &style)                   = 0;
    virtual void     SetCommonBtnDefault()                                 = 0;
    virtual void     SetCommonButtonStyle(QString type, QString btnName)   = 0;
    virtual void     SetTitleTextStyle(const QString &style)               = 0;
    virtual void     SetBackPanelIcon(const QString &iconUrl)              = 0;
    virtual void     SetTitlePanelIcon(const QString &iconUrl)             = 0;
    virtual void     WndClear()                                            = 0;
    virtual void     SetSystrayIconTooltip(QString tip)                    = 0;
    virtual void     SetDeleteOnClose(bool isOk)                           = 0;
    virtual void     SetUserStateByKeepAlive(EPRESENCESTATUS state)        = 0;
    virtual void     SetMinBtn(bool isVisible)                             = 0;
    virtual void     SetCloseBtn(bool isVisible)                           = 0;
    virtual void     SetMaxBtn(bool isVisible)                             = 0;
    virtual void     SetTitle(bool visible)                                = 0;
    virtual void     SetWndModel()                                         = 0;
    virtual void     InitSystemTrayIcon(bool isLogined)                    = 0;
    virtual void     HideMaxBtn()                                          = 0;
    virtual void     HideMinBtn()                                          = 0;
    virtual void     HideCloseBtn()                                        = 0;
    virtual void     SetModal()                                            = 0;
    virtual void     ShowMaxWnd()                                          = 0;
    virtual void     SetTitleIconVisible(bool isVisible)                   = 0;
    virtual void     SetTitleTextVisible(bool isVisible)                   = 0;
    virtual void     SetTitlePanelVisible(bool isVisible)                  = 0;
    virtual int      GetCloseType() const                                  = 0;
    virtual void     SetCloseType(int val)                                 = 0;
    virtual void     StartTwinkle()                                        = 0;
    virtual void     StopTwinkle()                                         = 0;
    virtual void     arouseBaseMainWnd()                                   = 0;
    virtual QRect    getWorkarea()                                         = 0; //获取工作区域大小
    virtual int      getmargin()                                           = 0;
    virtual void     setOnTopHint(bool on)                                 = 0;
    virtual bool     getOnTopHint()                                        = 0;

signals:
    void S_OnQuit();
    void S_OnClose();
    void S_ActivateShortcut(const quint32 keyCode, const quint32 modifiers);
    void S_SetPanelScale();
    void S_DoubleClickTrayIcon();
};

#endif // IBASEMAINWND_H
