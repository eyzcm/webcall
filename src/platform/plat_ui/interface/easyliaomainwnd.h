#ifndef EASYLIAOMAINWND_H
#define EASYLIAOMAINWND_H

#include "data/Im_Message.h"

#include "interface/ibasemainwnd.h"
#include "ui_easyliaomainwnd.h"
#include "plat_ui_global.h"
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
class EasyLiaoMainWnd;
}
QT_END_NAMESPACE

class PLAT_UI_EXPORT EasyLiaoMainWnd : public QWidget
{
    Q_OBJECT

public:
    EasyLiaoMainWnd(QWidget *parent = 0);
    ~EasyLiaoMainWnd();

public:
    virtual void MainWnd(IBaseMainWnd *wnd)
    {
        m_baseWnd = wnd;
    }
    virtual IBaseMainWnd *MainWnd()
    {
        return m_baseWnd;
    }
    virtual void     SetDisconnectTip(int isDisconnect)                                                                                             = 0;
    virtual void     DoReceiveMessage(const Im_Message &msg)                                                                                        = 0;
    virtual void     DoSaveVisitorInfoFinished(QString isAuto, QString vid, QString staticId)                                                                                                     = 0;
    virtual void     DoCustomerPresenceChanged(QString fromId, EPRESENCESTATUS newStatus, EPRESENCESTATUS oldStatus, CUSTOMER_LOGIN_TYPE loginType) = 0;
    virtual void     ActivateGlobalShortcut(const quint32 keyCode, const quint32 modifier)                                                          = 0;
    virtual QWidget *getCenterwidget()                                                                                                              = 0;

signals:
    void S_OpenMainwnd();
    void S_Closed();
    void S_BubbleTipShowMessage(QString title, QString msg);
    void S_BuildMainWindowFinished();

public slots:
    void R_ActivateShortcut(const quint32 keyCode, const quint32 modifier)
    {
        ActivateGlobalShortcut(keyCode, modifier);
    }

private:
    Ui::EasyLiaoMainWnd *ui;
    IBaseMainWnd        *m_baseWnd;
};

#endif // EASYLIAOMAINWND_H
