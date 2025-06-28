#include "uicustomtitlebarwidget.h"
#include <QMouseEvent>
#include <QApplication>
#include <QStyle>

UICustomTitleBarWidget::UICustomTitleBarWidget(QWidget *parent)
    : QWidget(parent), m_isMaximized(false)
{
    // Set layout
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    // Title Label
    m_titleLabel = new QLabel("Custom Title Bar", this);
    layout->addWidget(m_titleLabel);

    // Minimize Button
    m_minimizeButton = new QPushButton(this);
    m_minimizeButton->setFixedSize(24,24);
    m_minimizeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarMinButton));
    connect(m_minimizeButton, &QPushButton::clicked, this, &UICustomTitleBarWidget::OnMinimizeButtonClicked);
    layout->addWidget(m_minimizeButton);

    // Maximize/Restore Button
    m_maximizeRestoreButton = new QPushButton(this);
    m_maximizeRestoreButton->setFixedSize(24,24);
    UpdateMaximizeRestoreButton();
    connect(m_maximizeRestoreButton, &QPushButton::clicked, this, &UICustomTitleBarWidget::OnMaximizeRestoreButtonClicked);
    layout->addWidget(m_maximizeRestoreButton);

    // Close Button
    m_closeButton = new QPushButton(this);
    m_closeButton->setFixedSize(24,24);
    m_closeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    connect(m_closeButton, &QPushButton::clicked, this, &UICustomTitleBarWidget::OnCloseButtonClicked);
    layout->addWidget(m_closeButton);

    setLayout(layout);
}

void UICustomTitleBarWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPos() - parentWidget()->frameGeometry().topLeft();
        event->accept();
    }
}

void UICustomTitleBarWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        parentWidget()->move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void UICustomTitleBarWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    OnMaximizeRestoreButtonClicked();
}

void UICustomTitleBarWidget::OnMinimizeButtonClicked()
{
    emit SignalMinimize();
}

void UICustomTitleBarWidget::OnMaximizeRestoreButtonClicked()
{
    m_isMaximized = !m_isMaximized;
    UpdateMaximizeRestoreButton();
    emit SignalMaximizeRestore();
}

void UICustomTitleBarWidget::OnCloseButtonClicked()
{
    emit SignalClose();
}

void UICustomTitleBarWidget::UpdateMaximizeRestoreButton()
{
    if (m_isMaximized) {
        m_maximizeRestoreButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarNormalButton));
    } else {
        m_maximizeRestoreButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarMaxButton));
    }
}
