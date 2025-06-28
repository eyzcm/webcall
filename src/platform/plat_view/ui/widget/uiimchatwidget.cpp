#include "uiimchatwidget.h"
#include "uiyl/tree/UIYL_TreeWidget.h"
#include "ui/widget/uicontentwidget.h"


UIImChatWidget::UIImChatWidget(QWidget *parent)
    : QWidget(parent),
      m_splitter(new QSplitter(this)),
      m_sidebarTreeWidget(new UIYL_TreeWidget(this)),
      m_centralAreaWidget(new UIContentWidget(this)),
      m_sideTabWidget(new UISideTabWidget(this))
{
    m_centralAreaWidget->setStyleSheet("background-color:#ff0099");
    // 设置 UISidebarTreeWidget 的宽度范围
      m_sidebarTreeWidget->setMinimumWidth(200);
      m_sidebarTreeWidget->setMaximumWidth(400);

      // 设置 UISideTabWidget 的宽度范围
      m_sideTabWidget->setMinimumWidth(300);
      m_sideTabWidget->setMaximumWidth(500);

      // 将三个组件添加到 QSplitter 中
      m_splitter->addWidget(m_sidebarTreeWidget);
      m_splitter->addWidget(m_centralAreaWidget);
      m_splitter->addWidget(m_sideTabWidget);

      // 设置 QSplitter 的初始宽度
      QList<int> sizes;
      sizes << 300 << 600 << 400;  // 初始宽度：左300，中600，右400
      m_splitter->setSizes(sizes);

      // 设置 QSplitter 的布局
      QHBoxLayout *layout = new QHBoxLayout(this);
      layout->addWidget(m_splitter);
      layout->setContentsMargins(0, 0, 0, 0);
      setLayout(layout);

}

UIImChatWidget::~UIImChatWidget()
{
    // 在析构函数中删除指针，释放内存
    delete m_sidebarTreeWidget;
    delete m_centralAreaWidget;
    delete m_sideTabWidget;
}
