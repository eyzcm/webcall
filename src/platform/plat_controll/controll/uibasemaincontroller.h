#ifndef UIBASEMAINCONTROLLER_H
#define UIBASEMAINCONTROLLER_H

#include "plat_controll_global.h"
#include "utils/UISingleton.h"
#include "utils/Plat_EnumDefine.h"

#include <QObject>
#include <QString>
#include <QTimer>

class IBaseMainWnd;
class EasyLiaoMainWnd;

class  PLAT_CONTROLL_EXPORT UIBaseMainController : public QObject, public UISingleton<UIBaseMainController>
{
    Q_OBJECT
    friend class UISingleton<UIBaseMainController>; //因为基类在GetInstance()中 new T() 时会调用到A的构造函数


public:
    UIBaseMainController();
    virtual ~UIBaseMainController();

public:
    void Init();
    void ShowMainWnd();
    void CreateBaseUIMainWnd();
    void CreateMainWnd();
    void InitConnectMainSignal(IBaseMainWnd *wnd);
    void CloseMainWindow();

    void SetWindowTitle(QString txt);
    void SetSkinImage(QString imgIcon);

    IBaseMainWnd*    BaseUIMainWnd() const;
    EasyLiaoMainWnd* EasyLiaoMainUI() const;

    void InitRegisterHotKey();
    void RegisterHotKeyByVal(QString strHotKey, int strAtomId);
    void CreateGlobalAddAtom();

    void SetRobotState();

    void ShowQuitMessageTip();
    void SetUserState(EPRESENCESTATUS state);

protected:
    unsigned int GetModifyKeyByHotKey(QString strHotKey);
    unsigned int GetKeyByShotKey(QString strHotKey);

private slots:
    void R_OnTimer();
    void R_ShowSettingChanged();
    void R_SetMainWndTop(bool isTopWnd);
    void R_OnSetMainWndVisible(bool isShow);
    void R_DoMainWndQuit();
    void R_DoStateChange(EPRESENCESTATUS fromstate, EPRESENCESTATUS tostate);
    void R_StateChangeTimer();
    void R_Received_KeepAlive_State(EPRESENCESTATUS state);
    void R_DoSetState(EPRESENCESTATUS state);
    void R_OnQuit();
    void R_ResultOK(bool isOk);
    void R_StopTwinkleTip();
    void R_DoubleClickTrayIcon();
    void R_QuitApplication();

protected:
    IBaseMainWnd*    m_baseuiMainWnd;
    EasyLiaoMainWnd* m_mainWnd;
    QString          m_currentUserId;
    int              m_hotKeyScreenId;
    int              m_hotKeyShowMainWndId;
    int              m_isDisconnect;
    QTimer*          m_disconnectShowTimer;
    QTimer*          m_StateChangeTimer;
};

#endif // UIBASEMAINCONTROLLER_H
