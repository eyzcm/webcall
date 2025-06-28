#include "UIYL_BaseWnd.h"
#include "ui/BaseWnd.h"

UIYL_BaseWnd::UIYL_BaseWnd(QWidget *parent) : QWidget(parent)
{
    m_baseWnd    = nullptr;
    m_customInit = 0;
    Init();
}

UIYL_BaseWnd::~UIYL_BaseWnd() {}

void UIYL_BaseWnd::Init()
{
    m_baseWnd = new BaseWnd();
}

void UIYL_BaseWnd::show()
{
    LoadCustom();
    if (m_baseWnd)
        m_baseWnd->ShowCenter();
    else
        QWidget::show();
}

void UIYL_BaseWnd::showNormal()
{
    LoadCustom();
    if (m_baseWnd)
    {
        m_baseWnd->ShowCenter();
    }
    else
        QWidget::showNormal();
}

void UIYL_BaseWnd::close()
{
    if (m_baseWnd)
    {
        Q_EMIT S_HideWndAndDelCenterWidget();
    }
    else
        QWidget::close();
}
void UIYL_BaseWnd::LoadCustom()
{
    if ((m_customInit == 0) && (m_baseWnd))
    {
        m_customInit = 1;
        m_baseWnd->AddContentWidget(this);
        CustomInitWnd(m_baseWnd);
    }
}

void UIYL_BaseWnd::resize(int w, int h)
{
    if (m_baseWnd)
    {
        m_baseWnd->resize(w, h);
    }
}
