#include "UICentralAreaWidget.h"
#include <QVBoxLayout>
#include <QWidget>
#include "ui/widget/center/UIChatAreaWidget.h"
#include "ui/widget/uiinputareawidget.h"

UICentralAreaWidget::UICentralAreaWidget(QWidget *parent)
    : QWidget(parent)
{
    SetupLayout();
    Init();


}

void UICentralAreaWidget::Init()
{


}


void UICentralAreaWidget::SetupLayout()
{
    m_centralSplitter = new QSplitter(Qt::Vertical, this);

    QWidget *conversationArea = CreateConversationArea();
    QWidget *inputArea = CreateInputArea();

    inputArea->setMinimumHeight(0);
    inputArea->setMaximumHeight(400);
    m_centralSplitter->addWidget(conversationArea);
    m_centralSplitter->addWidget(inputArea);

    QList<int> sizes;
    sizes << 600 << 300 ;
    m_centralSplitter->setSizes(sizes);


    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_centralSplitter);
    setLayout(mainLayout);
}

QWidget* UICentralAreaWidget::CreateConversationArea()
{

    QWidget *conversationArea = new UIChatAreaWidget(this);
    conversationArea->setObjectName("m_chatAreaWidget");
    return conversationArea;
}

QWidget* UICentralAreaWidget::CreateInputArea()
{
    // Placeholder for the input area component
    QWidget *inputArea = new UIInputAreaWidget(this);
    inputArea->setObjectName("m_inputArea");
    return inputArea;
}
