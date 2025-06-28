#ifndef UILockWnd_H
#define UILockWnd_H

#include "ui/common/UIYL_BaseWnd.h"
#include "ui_UILockWnd.h"
#include <QCloseEvent>
#include <QWidget>
#include "plat_ui_global.h"

class PLAT_UI_EXPORT UILockWnd : public UIYL_BaseWnd
{
    Q_OBJECT

public:
    UILockWnd(QWidget *parent = 0);
    ~UILockWnd();
    void CustomInitWnd(BaseWnd *baseWnd);

protected:
    void keyPressEvent(QKeyEvent *arg);

protected slots:
    virtual void closeEvent(QCloseEvent *e);

private slots:
    void R_textChange(QString text);
    void R_OnLockBtnClick();
    void R_OnCloseBtnClick();
    void R_ClearBtnClick();
    void R_DispBtnClick(bool val);

signals:
    void S_CloseWin();
    void S_LockScreen_Clicked();
    void S_Closed();

private:
    Ui::UILockWnd ui;
};

#endif // UILockWnd_H
