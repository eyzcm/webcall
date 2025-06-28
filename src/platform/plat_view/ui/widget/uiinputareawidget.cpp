#include "UIInputAreaWidget.h"
#include <QFile>
#include <QTextStream>


UIInputAreaWidget::UIInputAreaWidget(QWidget *parent) : QWidget(parent)
{
    SetupUI();
    LoadStylesheet();
}

void UIInputAreaWidget::SetupUI()
{
    // Initialize layouts
    m_toolBarLayout = new QHBoxLayout();
    m_mainLayout = new QVBoxLayout(this);
    m_buttonContainerLayout = new QHBoxLayout();
    m_textInputLayout = new QVBoxLayout();

    // Initialize components
    m_richTextEdit = new QTextEdit(this);
    m_sendButton = new QPushButton("Send", this);
    m_styleToggleButton = new QPushButton("Toggle Style", this);

    // Setup toolbar layout
    m_buttonContainerLayout->addWidget(new QPushButton("Button1", this)); // Example button
    m_buttonContainerLayout->addStretch();
    m_toolBarLayout->addLayout(m_buttonContainerLayout);
    m_toolBarLayout->addWidget(m_styleToggleButton);

    // Setup text input area
    m_textInputLayout->addLayout(m_toolBarLayout);
    m_textInputLayout->addWidget(m_richTextEdit);


    // Setup send button layout
    QHBoxLayout* sendButtonLayout = new QHBoxLayout();
    sendButtonLayout->addStretch();
    sendButtonLayout->addWidget(m_sendButton);

    // Combine layouts into the main layout
    m_mainLayout->addLayout(m_textInputLayout);
    m_mainLayout->addLayout(sendButtonLayout);

    // Connect the send button click signal to the slot
    connect(m_sendButton, &QPushButton::clicked, this, &UIInputAreaWidget::OnSendButtonClicked);
}

void UIInputAreaWidget::LoadStylesheet()
{
    QFile file("://Resources/css/UIInputAreaWidget.css");
    if (file.open(QFile::ReadOnly))
    {
        QTextStream stream(&file);
        setStyleSheet(stream.readAll());
        file.close();
    }
}

void UIInputAreaWidget::SetSendButtonCallback(const std::function<void()>& callback)
{
    m_sendButtonCallback = callback;
}

void UIInputAreaWidget::OnSendButtonClicked()
{
    if (m_sendButtonCallback)
    {
        m_sendButtonCallback();
    }
}
