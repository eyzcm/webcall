#ifndef BaseMainWnd_H
#define BaseMainWnd_H


#include "plat_ui_global.h"
#include <QMenu>
#include <QSystemTrayIcon>
#include <QTimer>
#include "interface/ibasemainwnd.h"

#include "ui/common/UI_Widget_Lock.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class BaseMainWndClass;
}
QT_END_NAMESPACE

class El_MenuNavInterface;

class PLAT_UI_EXPORT BaseMainWnd : public IBaseMainWnd
{
    Q_OBJECT

public:
    BaseMainWnd(QWidget *parent = 0);
    ~BaseMainWnd();
    static bool m_isLoadLockScreen;

protected:
    virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);
    virtual void closeEvent(QCloseEvent *event);
    virtual void showEvent(QShowEvent *event);

public:
    virtual void DoOnOpenMainWndWnd();
    virtual void SetWndTitle(QString title);
    virtual void DoSetWindowTitle(QString title);
    virtual void SetWndIcon(const QString &iconUrl);
    void         AddContentWidget(QWidget *wnd, bool isVisible = true);
    void         AddMenuNavWidget(El_MenuNavInterface *wnd, bool isVisible = true);
    void         RemoveCenterWidget();
    void         DeleteCenterWidget();
    QWidget     *ContentWidget();
    QWidget     *Widget();
    bool         HasCenterWidget();
    void         AddExtendBtnWidget(QWidget *wnd, bool isVisible);
    void         SetTitleStyle(const QString &style);
    void         SetCommonBtnDefault();
    void         SetCommonButtonStyle(QString type, QString btnName);
    void         SetTitleTextStyle(const QString &style);
    void         SetBackPanelIcon(const QString &iconUrl);
    void         SetTitlePanelIcon(const QString &iconUrl);
    void         WndClear();
    void         SetSystrayIconTooltip(QString tip);
    void         SetDeleteOnClose(bool isOk);
    void         SetUserStateByKeepAlive(EPRESENCESTATUS state);
    void         SetMinBtn(bool isVisible);
    void         SetCloseBtn(bool isVisible);
    void         SetMaxBtn(bool isVisible);
    void         SetTitle(bool visible);
    void         SetWndModel();
    void         InitSystemTrayIcon(bool isLogined);
    void         HideMaxBtn();
    void         HideMinBtn();
    void         HideCloseBtn();
    void         SetModal();
    void         ShowMaxWnd();
    void         SetTitleIconVisible(bool isVisible);
    void         SetTitleTextVisible(bool isVisible);
    void         SetTitlePanelVisible(bool isVisible);
    int          GetCloseType() const;
    void         SetCloseType(int val);
    void         StartTwinkle();
    void         StopTwinkle();
    void         arouseBaseMainWnd();
    QRect        getWorkarea(); //获取工作区域大小
    int          getmargin();
	void		 LoadMenuNavWnd();

    void setOnTopHint(bool on)
    {
        m_OnTopHint = on;
    }
    bool getOnTopHint()
    {
        return m_OnTopHint;
    }

private:
    int  AddContentPage(QWidget *wnd);
    void SwitchContentPage(int index);

signals:

    void S_OnQuit();
    void S_OnClose();
    void S_ActivateShortcut(const quint32 keyCode, const quint32 modifiers);
    void S_SetPanelScale();
    void S_DoubleClickTrayIcon();

private:
    void    CreateTrayIcon(bool isLogined);
    void    CreateTrayAction(QMenu *menu, const QString &iconUri, const QString &text, const QString &data);
    void    SetMargin(int margin);
    QString GetUrl(QString tempUri);

private slots:
	void R_LoadMenuNavWnd();
    void R_clickMenuNav(QString key);
    void R_GetSoftVersionResult(QString code, QString reason, QString body);
    void R_ShowUserCheck();
    void R_OnMouseDoubleClicked(QMouseEvent *event);
    void R_OnMouseMoveEvent(QMouseEvent *event);
    void R_OnMouseReleaseEvent(QMouseEvent *event);
    void R_OnResetRegion(const QPoint &cursorGlobalPoint);
    void R_OnCloseBtn_Clicked();
    void R_OnMinBtn_Clicked();
    void R_OnMaxBtn_Clicked();
    void R_OnStoreBtn_Clicked();
    void R_BtnUpdate_Clicked();
    void R_BubbleTipShowMessage(QString title, QString msg);
    void R_OnTrayMenuClicked();
    void R_StatusChange(EPRESENCESTATUS state);
    void R_OnIconActivated(QSystemTrayIcon::ActivationReason reason);
    void R_OnTwinkleTimeOut();
    void R_TopRight_LockScreen_Clicked();
    void R_Update_package_download_completed(QString code, QString reason, QByteArray body); //更新包下载完成
    void R_aboutToShow();

public slots:
    void R_OnSetWndTitle(QString title)
    {
        SetWndTitle(title);
    }
    void R_OnOpenMainWndWnd();

private:
    Ui::BaseMainWndClass *ui;
    bool                  m_IsMaxisize;
    bool                  m_isCanMax;
    QWidget              *m_centerWidget;
    int                   m_closeType;
    QSystemTrayIcon      *m_systrayIcon;
    QMenu                *m_trayIconMenu;
    ELOGINSTATUS          m_loginStatus;
    EPRESENCESTATUS       m_currentPresenceStatus;
    QTimer               *m_twinkleTimer;
    bool                  m_isTwinkle;
    QRect                 m_rect;
    QPoint                m_poit;
    bool                  m_firstpoit;
    bool                  m_OnTopHint;       //是否置顶
    bool                  m_Update_reminder; //启动首次更新强制提醒
    El_MenuNavInterface  *m_menunav;
    QMap<QString, int>    m_widgetPages;
};

#endif // BaseMainWnd_H
