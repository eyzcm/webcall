#include "UI_LoginTransitionsPage.h"
#include <QBuffer>
#include <QDataStream>
#include <QDebug>

UI_LoginTransitionsPage::UI_LoginTransitionsPage(QObject *parent) 
{
    m_LoginTransitionPage = new UI_LoginTransitionPage();
    m_LoginTransitionPage->show();
    m_LoginTransitionPage->move(m_LoginTransitionPage->x() - m_LoginTransitionPage->width() / 2, m_LoginTransitionPage->y() - m_LoginTransitionPage->height() / 2); //移动到所在屏幕中间
}

UI_LoginTransitionsPage::~UI_LoginTransitionsPage()
{
    if (m_LoginTransitionPage)
    {
        m_LoginTransitionPage->deleteLater();
        m_LoginTransitionPage = nullptr;
    }
}

void UI_LoginTransitionsPage::SendSchedule(QString percentage, QString msg)
{
    m_LoginTransitionPage->UpdateProgressBarValue(percentage.toInt(), msg);
}
