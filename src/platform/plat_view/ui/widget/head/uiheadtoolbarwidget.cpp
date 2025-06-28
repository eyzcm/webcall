#include "UIHeadToolbarWidget.h"
#include <QFile>

UIHeadToolbarWidget::UIHeadToolbarWidget(QWidget *parent)
    : QWidget(parent),
      m_toolbarLayout(new QHBoxLayout(this))
{
    m_toolbarLayout->setContentsMargins(0, 0, 0, 0);
    m_toolbarLayout->setSpacing(5);  // 设置按钮之间的间距
    setLayout(m_toolbarLayout);

    // 加载样式表
    LoadStylesheet();
}

UIHeadToolbarWidget::~UIHeadToolbarWidget()
{
    qDeleteAll(m_buttonMap);
    m_buttonMap.clear();
}

void UIHeadToolbarWidget::AddButton(const QString &buttonName, const QIcon &icon, const QString &buttonText)
{
    if (!m_buttonMap.contains(buttonName)) {
        QPushButton *button = new QPushButton(icon, buttonText, this);
        m_toolbarLayout->addWidget(button);
        m_buttonMap.insert(buttonName, button);
    }
}

void UIHeadToolbarWidget::RemoveButton(const QString &buttonName)
{
    if (m_buttonMap.contains(buttonName)) {
        QPushButton *button = m_buttonMap.value(buttonName);
        m_toolbarLayout->removeWidget(button);
        m_buttonMap.remove(buttonName);
        delete button;
    }
}

QPushButton* UIHeadToolbarWidget::GetButton(const QString &buttonName) const
{
    return m_buttonMap.value(buttonName, nullptr);
}

void UIHeadToolbarWidget::LoadStylesheet()
{
    QFile file(":/stylesheets/UIHeadToolbarWidget.css");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        setStyleSheet(styleSheet);
        file.close();
    }
}
