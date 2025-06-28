#include "UI_Btn_Enter.h"

UI_Btn_Enter::UI_Btn_Enter(QWidget* parent) : QWidget(parent)
{
    ui.setupUi(this);
    m_menu = nullptr;

    connect(ui.m_mainbtn, SIGNAL(S_MoveOnButton(bool)), this, SIGNAL(S_MoveOnButton(bool)));
    connect(ui.m_arrawBtn, SIGNAL(S_MoveOnButton(bool)), this, SIGNAL(S_MoveOnButton(bool)));

    connect(ui.m_mainbtn, SIGNAL(clicked()), this, SIGNAL(S_MainBtnClicked()));
    connect(ui.m_arrawBtn, SIGNAL(clicked()), this, SIGNAL(S_ArrawBtnClicked()));
}

UI_Btn_Enter::~UI_Btn_Enter()
{
}

void UI_Btn_Enter::SetMainBtnStyle(const QString& style)
{
    ui.m_mainbtn->setStyleSheet(style);
}

void UI_Btn_Enter::SetArrawBtnStyle(const QString& style)
{
    ui.m_arrawBtn->setStyleSheet(style);
}

void UI_Btn_Enter::SetContentStyle(const QString& style)
{
    ui.m_bkPanelEnter->setStyleSheet(style);
}

void UI_Btn_Enter::SetMenuButtonStyle(const QString& style)
{
    // QString style1 = "image: url(:/Resources/Image/a_press.png)";
    ui.m_bkPanelEnter->setStyleSheet(style);
}

void UI_Btn_Enter::SetMainBtn(const QString& iconUri, const QString& text, Qt::ToolButtonStyle toolButtonStyle)
{
    if (text.isEmpty())
    {
        ui.m_mainbtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }
    else
    {
        if (iconUri.isEmpty())
        {
            ui.m_mainbtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
        }
        else
        {
            ui.m_mainbtn->setToolButtonStyle(toolButtonStyle);
        }
    }
    ui.m_mainbtn->setIcon(QIcon(iconUri));
    ui.m_mainbtn->setText(text);
}

void UI_Btn_Enter::SetArrawBtnIcon(const QString& iconUri)
{
    ui.m_arrawBtn->setIcon(QIcon(iconUri));
}

void UI_Btn_Enter::SetMenu(QMenu* menu)
{
    m_menu = menu;
}

void UI_Btn_Enter::ShowMenu()
{
    if (m_menu)
    {
        QPoint pt = ui.m_bkPanelEnter->mapToGlobal(ui.m_bkPanelEnter->pos());
        QPoint pos;
        pos.setX(pt.x());
        pos.setY(pt.y() + this->height());
        m_menu->exec(pos);
    }
}

void UI_Btn_Enter::SetArrawBtnVisible(bool isVisible)
{
    ui.m_arrawBtn->setVisible(isVisible);
}

void UI_Btn_Enter::SetMainBtnIconSize(int width, int height)
{
    ui.m_mainbtn->setIconSize(QSize(width, height));
}
