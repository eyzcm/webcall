#include "uisidebartreewidget.h"

#include <QWidget>
#include <QFile>

UISidebarTreeWidget::UISidebarTreeWidget(QWidget *parent)
    : QWidget(parent), m_stackedWidget(new QStackedWidget(this)), m_mainLayout(new QVBoxLayout(this))
{
    QFile file("://Resources/css/SidebarTree.css");
    if(file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
        file.close();
    }
    setupUi();
}

void UISidebarTreeWidget::setupUi() {
    // 初始化布局，添加顶部按钮布局
    m_buttonLayout = new QHBoxLayout();
    m_mainLayout->addLayout(m_buttonLayout);
    m_mainLayout->addWidget(m_stackedWidget);

    setLayout(m_mainLayout);
}

void UISidebarTreeWidget::AddButtonWithWidget(const QString &buttonText, QWidget *widget, const std::function<void()> &callback ) {
    QPushButton *newButton = new QPushButton(buttonText, this);

    m_buttonWidgetMap[newButton] = widget;
    m_stackedWidget->addWidget(widget);

    // 将新按钮添加到布局中
    m_buttonLayout->insertWidget(m_buttonLayout->count() - 1, newButton);

    connect(newButton, &QPushButton::clicked, [this, newButton, callback]() {
        m_stackedWidget->setCurrentWidget(m_buttonWidgetMap[newButton]);
        if (callback) {
            callback();  // 执行外部传入的回调函数
        }
    });
}
