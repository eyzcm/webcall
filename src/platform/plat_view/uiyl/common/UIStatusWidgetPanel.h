#pragma once

#include <QMenu>
#include <QProcess>
#include <QWidget>
#include "ui/BaseWnd.h"
#include "plat_view_global.h"

namespace Ui {
    class UIStatusWidgetPanel;
}


class PLAT_VIEW_EXPORT UIStatusWidgetPanel : public QWidget
{
    Q_OBJECT

public:
    UIStatusWidgetPanel(QWidget *parent = 0);
    ~UIStatusWidgetPanel();

private:
    void CreateAction(QMenu *menu, const QString &iconUri, const QString &text, const QString &data, int flag = 0);
    void InitState();

signals:
    void S_DoQuit();

private slots:
    void R_Received_KeepAlive_State(EPRESENCESTATUS state);
    void R_LoginStateChanged(ELOGINSTATUS oldStatus, ELOGINSTATUS newStatus);
    void R_OnStateMenuClicked();
    void R_ResultOK(bool isOK);
    void R_ResultOK_Logout(bool isOK);
    void R_ProcessCallStarted();
    void R_OnUserStateBtnClick();

public:
    void DoQuit();
    void DoLogout();
    void DoCloudlogin();
    void DoSetState(QString data);

private:
    Ui::UIStatusWidgetPanel * ui;
};
