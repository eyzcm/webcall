

#include "stdafx.h"
#include "UIWebTabWidget.h"

UIWebTabWidget::UIWebTabWidget(QWidget *parent) : QTabWidget(parent), m_customMenu(nullptr)
{
    // Tab的显示模式
    setTabsClosable(true);
    setMovable(true);


}

void UIWebTabWidget::AddTab(const QString &title, QWidget *content)
{
    addTab(content, title);
}

void UIWebTabWidget::SetCustomContextMenu(QMenu *menu)
{
    m_customMenu = menu;
}

void UIWebTabWidget::contextMenuEvent(QContextMenuEvent *event)
{
    if (m_customMenu) {
        m_customMenu->exec(event->globalPos());
    } else {
        QTabWidget::contextMenuEvent(event);
    }
}
