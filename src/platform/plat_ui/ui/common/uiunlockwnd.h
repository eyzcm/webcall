#ifndef UIUnlockWnd_H
#define UIUnlockWnd_H

#include "ui/common/UIYL_BaseWnd.h"
#include "ui_UIUnlockWnd.h"
#include <QCloseEvent>
#include <QWidget>
#include "plat_ui_global.h"

class PLAT_UI_EXPORT UIUnlockWnd : public UIYL_BaseWnd
{
    Q_OBJECT

public:
    UIUnlockWnd(QWidget *parent = 0);
    ~UIUnlockWnd();
    virtual void CustomInitWnd(BaseWnd *baseWnd);

protected:
    void keyPressEvent(QKeyEvent *arg);

protected slots:
    virtual void closeEvent(QCloseEvent *e);

private slots:
    void R_textChange(QString text);
    void R_OnUnlockBtnClick();
    void R_ClearBtnClick();
    void R_DispBtnClick(bool val);

signals:
    void S_CloseWin();

private:
    Ui::UIUnlockWnd ui;
    bool            m_isClose;
};

#endif // UIUnlockWnd_H
