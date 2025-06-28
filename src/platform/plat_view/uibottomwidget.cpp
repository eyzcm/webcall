#include "UIBottomWidget.h"
#include <QHBoxLayout>

UIBottomWidget::UIBottomWidget(QWidget *parent)
    : QWidget(parent)
{
    // Initialize the three widgets
    m_leftWidget = new QWidget(this);
    m_centerWidget = new QWidget(this);
    m_rightWidget = new QWidget(this);

    // Set fixed widths for the left and right widgets
    m_leftWidget->setFixedWidth(300);
    m_rightWidget->setFixedWidth(500);

    // Create a horizontal layout and add the widgets
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_leftWidget);
    layout->addWidget(m_centerWidget);
    layout->addWidget(m_rightWidget);

    // Set the layout for this widget
    setLayout(layout);
}
