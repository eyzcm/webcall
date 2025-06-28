#include "UI_UserCheckWnd.h"


UI_UserCheckWnd::UI_UserCheckWnd(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    this->setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::WindowMinimizeButtonHint);
    setAttribute(Qt::WA_DeleteOnClose);
    connect(ui.m_userCheckPanel, SIGNAL(S_closeWnd()), this, SLOT(R_closeWindow()));
    this->setAttribute(Qt::WA_TranslucentBackground, true);
}

UI_UserCheckWnd::~UI_UserCheckWnd()
{
}

void UI_UserCheckWnd::closeEvent(QCloseEvent *event)
{
    Q_EMIT S_CloseWidget_Assistant();
}

void UI_UserCheckWnd::R_closeWindow()
{
    this->hide();
    this->close();
}

void UI_UserCheckWnd::Init(int type, QString url, int isPwdSafed)
{
    ui.m_userCheckPanel->Init(type, url, isPwdSafed);
    ui.m_userCheckPanel->InitData();
}
