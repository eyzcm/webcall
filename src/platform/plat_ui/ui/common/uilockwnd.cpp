#include "uilockwnd.h"

#include "baseMainWnd.h"

#include <QRegExpValidator>
#include <ui/BaseWnd.h>

UILockWnd::UILockWnd(QWidget *parent) : UIYL_BaseWnd(parent)
{
    ui.setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(windowFlags() & ~Qt::WindowMinMaxButtonsHint);
    this->setFixedSize(this->width(), this->height());
    this->setWindowIcon(QIcon(":/YL/Resources/YL/TopRight/icon_lock.png"));
    this->setWindowModality(Qt::ApplicationModal);

    connect(ui.m_passwordLine, SIGNAL(textChanged(QString)), this, SLOT(R_textChange(QString)));

    connect(ui.m_lockBtn, SIGNAL(clicked()), this, SLOT(R_OnLockBtnClick()));
    connect(ui.m_closeBtn, SIGNAL(clicked()), this, SLOT(R_OnCloseBtnClick()));
    connect(ui.m_cleartext_btn, SIGNAL(clicked()), this, SLOT(R_ClearBtnClick()));
    connect(ui.m_disppass_cbx, SIGNAL(clicked(bool)), this, SLOT(R_DispBtnClick(bool)));

    // ui.m_closeBtn->setVisible(false);
    QRegExp           regExp("[0-9]{0,6}");                         //创建了一个模式
    QRegExpValidator *pattern = new QRegExpValidator(regExp, this); //创建了一个表达式
    ui.m_passwordLine->setValidator(pattern);
    ui.m_lineEdit_bak->setValidator(pattern);


    {
        if (gUISystemConfigManager->LockPassword().isEmpty() == false)
        {
            ui.m_passwordLine->setText(gUISystemConfigManager->LockPassword());
            ui.m_passwordLine->setFocus();
        }
        else
        {
            ui.m_lineEdit_bak->setFocus();
        }
    }

    ui.horizontalLayout_5->layout()->removeWidget(ui.m_lineEdit_bak);
    ui.m_lineEdit_bak->move(-100, -100);
}

UILockWnd::~UILockWnd()
{
}

void UILockWnd::closeEvent(QCloseEvent *e)
{
    e->accept();
}

void UILockWnd::R_OnLockBtnClick()
{
    ui.m_errormsg->setText("");
    QString password = ui.m_passwordLine->text().trimmed();
    if (password.isEmpty() == false)
    {

        {
            if (password != gUISystemConfigManager->LockPassword())
            {
                gUISystemConfigManager->LockPassword(password);

            }

        }

        Q_EMIT S_LockScreen_Clicked();

        this->hide();
        this->close();
    }
    else
    {
        ui.m_errormsg->setText(tr("密码不能为空"));
    }
}

void UILockWnd::keyPressEvent(QKeyEvent *arg)
{
    if (arg->key() == Qt::Key_Return || arg->key() == Qt::Key_Enter)
    {
        if (ui.m_passwordLine->hasFocus())
        {
            R_OnLockBtnClick();
        }
    }
}

void UILockWnd::R_OnCloseBtnClick()
{
    this->hide();
    close();
}

void UILockWnd::R_DispBtnClick(bool val)
{
    if (val)
    {
        ui.m_passwordLine->setEchoMode(QLineEdit::Password);
    }
    else
    {
        ui.m_passwordLine->setEchoMode(QLineEdit::Normal);
    }
    QString text = ui.m_passwordLine->text();
    if (text.isEmpty() == false)
    {
        ui.m_passwordLine->setFocus();
    }
}

void UILockWnd::R_ClearBtnClick()
{
    ui.m_passwordLine->setText("");
    ui.m_lineEdit_bak->setText("");
    ui.m_lineEdit_bak->setFocus();
}

void UILockWnd::CustomInitWnd(BaseWnd *baseWnd)
{
    if (baseWnd)
    {
        QString style = "{color: #343434;}";
        baseWnd->SetTitleTextStyle(style);

        baseWnd->SetWndIcon(":/YL/Resources/YL/TopRight/icon_lock.png");
        baseWnd->SetTitlePanelIcon(":/YL/Resources/YL/TopRight/icon_lock.png");
        baseWnd->SetWndTitle(tr("锁屏"));
        baseWnd->HideMaxBtn();
        baseWnd->HideMinBtn();

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

void UILockWnd::R_textChange(QString text)
{
    if (text.isEmpty())
    {
        ui.m_lineEdit_bak->setText("");
        ui.m_lineEdit_bak->setFocus();
    }
}
