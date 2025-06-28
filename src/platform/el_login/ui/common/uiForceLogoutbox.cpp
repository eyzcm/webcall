#include "UIForceLogoutBox.h"
#include <qevent.h>

UIForceLogoutBox::UIForceLogoutBox(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    connect(ui.m_btnOk, SIGNAL(clicked()), this, SLOT(R_OKClicked()));
}

UIForceLogoutBox::~UIForceLogoutBox()
{
}

void UIForceLogoutBox::closeEvent(QCloseEvent *e)
{

    R_OKClicked();
    e->accept();
}

void UIForceLogoutBox::hideEvent(QHideEvent *e)
{

    R_OKClicked();
    e->accept();
}

void UIForceLogoutBox::R_OKClicked()
{
    Q_EMIT S_ResultOK();
}

void UIForceLogoutBox::SetMsgText(QString text)
{
    ui.m_msgtext->setText(text);
}
