#include "UIMenuUserStateButton.h"
#include "ui_UIMenuUserStateButton.h"


UIMenuUserStateButton::UIMenuUserStateButton(QWidget* parent) : QWidget(parent)
    , ui(new Ui::UIMenuUserStateButton)
{
    ui->setupUi(this);
    m_menu = nullptr;
    // SetContentStyle("");
    // SetMainBtnStyle("");
    connect(ui->m_mainbtn, SIGNAL(S_MoveOnButton(bool)), this, SIGNAL(S_MoveOnButton(bool)));
    connect(ui->m_arrawBtn, SIGNAL(S_MoveOnButton(bool)), this, SIGNAL(S_MoveOnButton(bool)));

    connect(ui->m_mainbtn, SIGNAL(clicked()), this, SIGNAL(S_MainBtnClicked()));
    connect(ui->m_arrawBtn, SIGNAL(clicked()), this, SIGNAL(S_ArrawBtnClicked()));
}

UIMenuUserStateButton::~UIMenuUserStateButton()
{
}

void UIMenuUserStateButton::SetMainBtnStyle(const QString& style)
{
    ui->m_mainbtn->setStyleSheet(style);
}

void UIMenuUserStateButton::SetArrawBtnStyle(const QString& style)
{
    ui->m_arrawBtn->setStyleSheet(style);
}

void UIMenuUserStateButton::SetContentStyle(const QString& style)
{
    ui->widget->setStyleSheet(style);
}

void UIMenuUserStateButton::SetMenuButtonStyle(const QString& style)
{
    ui->m_bkPanelButton->setStyleSheet(style);
}

void UIMenuUserStateButton::SetMainBtn(const QString& iconUri, const QString& text, Qt::ToolButtonStyle toolButtonStyle)
{
    if (text.isEmpty())
    {
        ui->m_mainbtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }
    else
    {
        if (iconUri.isEmpty())
        {
            ui->m_mainbtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
        }
        else
        {
            ui->m_mainbtn->setToolButtonStyle(toolButtonStyle);
        }
    }
    ui->m_mainbtn->setIcon(QIcon(iconUri));
    ui->m_mainbtn->setText(text);
}

void UIMenuUserStateButton::SetArrawBtnIcon(const QString& iconUri)
{
    ui->m_arrawBtn->setIcon(QIcon(iconUri));
}

void UIMenuUserStateButton::SetMenu(QMenu* menu)
{
    m_menu = menu;
}

void UIMenuUserStateButton::ShowMenu()
{
    if (m_menu)
    {
        QPoint pt = ui->m_bkPanelButton->mapToGlobal(ui->m_bkPanelButton->pos());
        QPoint pos;
        pos.setX(pt.x());
        pos.setY(pt.y() + this->height());
        m_menu->exec(pos);
    }
}

void UIMenuUserStateButton::SetArrawBtnVisible(bool isVisible)
{
    ui->m_arrawBtn->setVisible(isVisible);
}

void UIMenuUserStateButton::SetMainBtnIconSize(int width, int height)
{
    ui->m_mainbtn->setIconSize(QSize(width, height));
}
