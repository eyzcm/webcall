#include "uiunlockwnd.h"

#include "ui/Common/BaseMainWnd.h"

#include <QRegExpValidator>
#include "ui/BaseWnd.h"

UIUnlockWnd::UIUnlockWnd(QWidget *parent) : UIYL_BaseWnd(parent)
{
    ui.setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags((windowFlags() & ~Qt::WindowMinMaxButtonsHint) & ~Qt::WindowCloseButtonHint);
    this->setFixedSize(this->width(), this->height());
    this->setWindowIcon(QIcon(":/YL/Resources/YL/TopRight/icon_unlock.png"));
    this->setWindowModality(Qt::ApplicationModal);

    m_isClose = false;
    connect(ui.m_unlockBtn, SIGNAL(clicked()), this, SLOT(R_OnUnlockBtnClick()));
    connect(ui.m_cleartext_btn, SIGNAL(clicked()), this, SLOT(R_ClearBtnClick()));
    connect(ui.m_disppass_cbx, SIGNAL(clicked(bool)), this, SLOT(R_DispBtnClick(bool)));
    connect(ui.m_passwordLine, SIGNAL(textChanged(QString)), this, SLOT(R_textChange(QString)));

    QRegExp           regExp("[0-9]{0,6}");                         //创建了一个模式
    QRegExpValidator *pattern = new QRegExpValidator(regExp, this); //创建了一个表达式
    ui.m_passwordLine->setValidator(pattern);
    ui.m_lineEdit_bak->setValidator(pattern);

    ui.m_disppass_cbx->setChecked(true);
    ui.m_passwordLine->setEchoMode(QLineEdit::Password);

    ui.m_lineEdit_bak->setFocus();

    ui.horizontalLayout_5->layout()->removeWidget(ui.m_lineEdit_bak);
    ui.m_lineEdit_bak->move(-100, -100);
}

UIUnlockWnd::~UIUnlockWnd() {}

void UIUnlockWnd::closeEvent(QCloseEvent *e)
{
    if (m_isClose)
    {
        e->accept();
    }
    else
    {
        e->ignore();
    }
}

void UIUnlockWnd::R_OnUnlockBtnClick()
{
    QString password = ui.m_passwordLine->text().trimmed();


    {
        if (password == gUISystemConfigManager->LockPassword())
        {
            m_isClose = true;
            this->close();
            Q_EMIT S_CloseWin();
        }
        else
        {
            ui.m_errormsg->setText(tr("请输入正确密码解锁！"));
        }
    }
}

void UIUnlockWnd::keyPressEvent(QKeyEvent *arg)
{
    if (arg->key() == Qt::Key_Return || arg->key() == Qt::Key_Enter)
    {
        if (ui.m_passwordLine->hasFocus())
        {
            R_OnUnlockBtnClick();
        }
    }
}

void UIUnlockWnd::R_DispBtnClick(bool val)
{
    if (val)
    {
        ui.m_passwordLine->setEchoMode(QLineEdit::Password);
    }
    else
    {
        ui.m_passwordLine->setEchoMode(QLineEdit::Normal);
    }
}

void UIUnlockWnd::R_ClearBtnClick()
{
    ui.m_passwordLine->setText("");
    ui.m_lineEdit_bak->setFocus();
}

void UIUnlockWnd::CustomInitWnd(BaseWnd *baseWnd)
{
    if (baseWnd)
    {
        baseWnd->SetWndIcon(":/YL/Resources/YL/TopRight/icon_unlock.png");
        baseWnd->SetTitlePanelIcon(":/YL/Resources/YL/TopRight/icon_unlock.png");
        baseWnd->SetWndTitle(tr("解锁"));
        baseWnd->HideMaxBtn();
        baseWnd->HideMinBtn();
        baseWnd->HideCloseBtn();

        baseWnd->SetTitleIconVisible(true);
        baseWnd->SetTitleTextVisible(true);
        baseWnd->SetWndModel();
        baseWnd->SetCloseType(1);
    }
    QString text = ui.m_passwordLine->text();
    if (text.isEmpty())
    {
        ui.m_lineEdit_bak->setFocus();
    }
}

void UIUnlockWnd::R_textChange(QString text)
{
    if (text.isEmpty())
    {
        ui.m_lineEdit_bak->setFocus();
    }
}
