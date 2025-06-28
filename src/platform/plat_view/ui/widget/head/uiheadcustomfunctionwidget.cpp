#include "UIHeadCustomFunctionWidget.h"
#include <QFile>

UIHeadCustomFunctionWidget::UIHeadCustomFunctionWidget(QWidget *parent)
    : QWidget(parent),
      m_functionLayout(new QHBoxLayout(this))
{
    m_functionLayout->setContentsMargins(0, 0, 0, 0);
    m_functionLayout->setSpacing(5);  // 设置按钮之间的间距
    setLayout(m_functionLayout);

    // 加载样式表
    LoadStylesheet();
}

UIHeadCustomFunctionWidget::~UIHeadCustomFunctionWidget()
{
    qDeleteAll(m_functionButtonMap);
    m_functionButtonMap.clear();
}

void UIHeadCustomFunctionWidget::AddFunctionButton(const QString &buttonName, const QIcon &icon, const QString &buttonText)
{
    if (!m_functionButtonMap.contains(buttonName)) {
        QPushButton *button = new QPushButton(icon, buttonText, this);
        m_functionLayout->addWidget(button);
        m_functionButtonMap.insert(buttonName, button);
    }
}

void UIHeadCustomFunctionWidget::RemoveFunctionButton(const QString &buttonName)
{
    if (m_functionButtonMap.contains(buttonName)) {
        QPushButton *button = m_functionButtonMap.value(buttonName);
        m_functionLayout->removeWidget(button);
        m_functionButtonMap.remove(buttonName);
        delete button;
    }
}

QPushButton* UIHeadCustomFunctionWidget::GetFunctionButton(const QString &buttonName) const
{
    return m_functionButtonMap.value(buttonName, nullptr);
}

void UIHeadCustomFunctionWidget::LoadStylesheet()
{
    QFile file(":/stylesheets/UIHeadCustomFunctionWidget.css");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        setStyleSheet(styleSheet);
        file.close();
    }
}
