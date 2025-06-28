#include "UI_Widget_Lock.h"
#include "ui/common/uiunlockwnd.h"

UI_Widget_Lock::UI_Widget_Lock(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    UIUnlockWnd *pLock = new UIUnlockWnd();
    pLock->show();
    connect(pLock, SIGNAL(S_CloseWin()), this, SLOT(R_CloseWin()));
}

UI_Widget_Lock::~UI_Widget_Lock()
{
}

void UI_Widget_Lock::R_CloseWin()
{
    this->close();
}
