#include "UISidebarWidget.h"
#include <QScrollArea>
#include <QIcon>
#include <QAction>
#include <functional>

#include <QDebug>

UISidebarWidget::UISidebarWidget(QWidget *parent)
    : QWidget(parent), m_layout(new UIVBoxLayout(this)), m_moreButton(new QPushButton(this)),
      m_exitButton(new QPushButton(this)), m_moreMenu(new QMenu(this))
{
    // 设置布局


    // 初始化动态按钮容器
    m_dynamicButtonContainer = new QWidget(this);
    m_dynamicButtonContainer->setLayout(new QVBoxLayout());
    m_layout->addWidget(m_dynamicButtonContainer);

    // 初始化更多按钮
    m_moreButton->setText("...");
    m_moreButton->setToolTip("More");
    connect(m_moreButton, &QPushButton::clicked, this, &UISidebarWidget::OnMoreButtonClicked);
    m_layout->addWidget(m_moreButton);

    // 初始化退出按钮
    m_exitButton->setText("Exit");
    m_exitButton->setIcon(QIcon(":/Resources/Image/a_quit.png")); // 示例图标路径
    m_layout->addWidget(m_exitButton);
    //El_MenuNavInterface * widget = gSessionService->GetMenuNavWnd();
    //qDebug()<<widget<<endl;

    // 更新更多按钮状态
    UpdateMoreButtonVisibility();



}

void UISidebarWidget::AddToolButton(const QString &iconPath, const QString &tooltip, std::function<void()> onClickCallback)
{
    QPushButton *button = new QPushButton(m_dynamicButtonContainer);
    button->setIcon(QIcon(iconPath));
    button->setToolTip(tooltip);
    m_dynamicButtonContainer->layout()->addWidget(button);

    // 连接信号槽
    connect(button, &QPushButton::clicked, [=]() {
        int index = m_dynamicButtonContainer->layout()->indexOf(button);
        emit S_ToolButtonClicked(index);

        if (onClickCallback) {
            onClickCallback(); // 调用传入的回调函数
        }
    });

    UpdateMoreButtonVisibility();
}

void UISidebarWidget::RemoveToolButton(int index)
{
    if (index >= 0 && index < m_dynamicButtonContainer->layout()->count()) {
        QLayoutItem *item = m_dynamicButtonContainer->layout()->takeAt(index);
        delete item->widget();
        delete item;

        UpdateMoreButtonVisibility();
    }
}

void UISidebarWidget::OnMoreButtonClicked()
{
    m_moreMenu->exec(m_moreButton->mapToGlobal(QPoint(0, m_moreButton->height())));
}

void UISidebarWidget::UpdateMoreButtonVisibility()
{
    // 计算当前容器内的实际高度
    int totalHeight = 0;
    for (int i = 0; i < m_dynamicButtonContainer->layout()->count(); ++i) {
        QWidget *widget = m_dynamicButtonContainer->layout()->itemAt(i)->widget();
        totalHeight += widget->sizeHint().height();
    }

    int containerHeight = m_dynamicButtonContainer->height();

    // 根据高度决定是否显示更多按钮
    if (totalHeight > containerHeight) {
        m_moreButton->show();

        // 更新更多菜单内容
        m_moreMenu->clear();
        for (int i = m_dynamicButtonContainer->layout()->count() - 1; i >= 0; --i) {
            QWidget *widget = m_dynamicButtonContainer->layout()->itemAt(i)->widget();
            if (totalHeight > containerHeight) {
                totalHeight -= widget->sizeHint().height();
                QAction *action = m_moreMenu->addAction(widget->toolTip());
                connect(action, &QAction::triggered, [=]() {
                    emit S_ToolButtonClicked(i);
                });
            }
        }
    } else {
        m_moreButton->hide();
    }
}
