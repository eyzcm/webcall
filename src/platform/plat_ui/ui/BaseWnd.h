#ifndef BaseWnd_H
#define BaseWnd_H


#include <QMenu>
#include <QSystemTrayIcon>
#include <QTimer>

#include "plat_ui_global.h"

QT_BEGIN_NAMESPACE
namespace Ui { class BaseWndClass; }
QT_END_NAMESPACE


#include "ui/MyWidgetBase.h"
#include "utils/Plat_EnumDefine.h"

class PLAT_UI_EXPORT BaseWnd : public MyWidgetBase
{
    Q_OBJECT

public:
    BaseWnd(QWidget *parent = 0);
    ~BaseWnd();


protected:
    virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);
    virtual void closeEvent(QCloseEvent *event);
    virtual void showEvent(QShowEvent *event);

public:
    void SetWndTitle(QString title);
    void DoSetWindowTitle(QString title);
    void SetWndIcon(const QString &iconUrl);
    void AddContentWidget(QWidget *wnd, bool isVisible = true);
    void RemoveCenterWidget();
    void DeleteCenterWidget();

    QWidget *ContentWidget();

    bool HasCenterWidget();
    void AddExtendBtnWidget(QWidget *wnd, bool isVisible);
    void SetTitleStyle(const QString &style);
    void SetCommonBtnDefault();
    void SetCommonButtonStyle(QString type, QString btnName);
    void SetTitleTextStyle(const QString &style);
    void SetBackPanelIcon(const QString &iconUrl);
    void SetTitlePanelIcon(const QString &iconUrl);
    void WndClear();
    void SetSystrayIconTooltip(QString tip);
    void SetDeleteOnClose(bool isOk);
    void SetUserStateByKeepAlive(EPRESENCESTATUS state);
    void SetMinBtn(bool isVisible);
    void SetCloseBtn(bool isVisible);
    void SetMaxBtn(bool isVisible);
    void SetBottomPanel(bool isVisible);
    void SetTitle(bool visible);
    void SetWndModel();
    void InitSystemTrayIcon(bool isLogined);
    void HideMaxBtn();
    void HideMinBtn();
    void HideCloseBtn();
    void SetModal();
    void ShowMaxWnd();
    void SetTitleIconVisible(bool isVisible);
    void SetTitleTextVisible(bool isVisible);
    void SetTitlePanelVisible(bool isVisible);
    int  GetCloseType() const;
    void SetCloseType(int val);

    void arouseBaseWnd();

    QRect getWorkarea(); //获取工作区域大小
    int   getmargin();

signals:
    void S_OnSetState(QString state);
    void S_OnQuit();
    void S_OnClose();
    void S_ActivateShortcut(const quint32 keyCode, const quint32 modifiers);
    void S_SetPanelScale();
    void S_DoubleClickTrayIcon();

private:
    void CreateTrayIcon(bool isLogined);
    void CreateTrayAction(QMenu *menu, const QString &iconUri, const QString &text, const QString &data);
    void SetMargin(int margin);

private slots:

    void R_OnMouseDoubleClicked(QMouseEvent *event);
    void R_OnMouseMoveEvent(QMouseEvent *event);
    void R_OnMouseReleaseEvent(QMouseEvent *event);
    void R_OnResetRegion(const QPoint &cursorGlobalPoint);
    void R_OnCloseBtn_Clicked();
    void R_OnMinBtn_Clicked();
    void R_OnMaxBtn_Clicked();
    void R_OnStoreBtn_Clicked();

    void R_BubbleTipShowMessage(QString title, QString msg);
    void R_OnTrayMenuClicked();
    void R_OnIconActivated(QSystemTrayIcon::ActivationReason reason);




public slots:
    void R_OnSetWndTitle(QString title)
    {
        SetWndTitle(title);
    }
    void R_OnOpenMainWndWnd();

private:
    Ui::BaseWndClass * ui;
    bool                 m_IsMaxisize;
    bool                 m_isCanMax;
    QWidget             *m_centerWidget;
    int                  m_closeType;
    QSystemTrayIcon     *m_systrayIcon;
    QMenu               *m_trayIconMenu;
    ELOGINSTATUS         m_loginStatus;
    EPRESENCESTATUS      m_currentPresenceStatus;
    QTimer              *m_twinkleTimer;
    bool                 m_isTwinkle;
    QRect                m_rect;
    QPoint               m_poit;
    bool                 m_firstpoit;
    bool                 m_Update_reminder; //启动首次更新强制提醒
};

#endif // BaseWnd_H
