#include "switchbutton.h"
const QString strStyle = "QToolButton{	\
                                                 border-image: url(:/Resources/Image/toolbtn_normal.png);\
                                                 }\
                                                 QToolButton:hover{\
                                                 border-image: url(:/Resources/Image/toolbtn_over_24_24.png);	\
                                                 }\
                                                 QToolButton:pressed{\
                                                 border-image: url(:/Resources/Image/toolbtn_over_24_24.png);	\
                                                 }";

const QString strCheckStyle = "QToolButton{	\
                                                 border-image: url(:/Resources/Image/toolbtn_over_24_24.png);\
                                                 }\
                                                 QToolButton:hover{\
                                                 border-image: url(:/Resources/Image/toolbtn_over_24_24.png);	\
                                                 }\
                                                 QToolButton:pressed{\
                                                 border-image: url(:/Resources/Image/toolbtn_over_24_24.png);	\
                                                 }";

const QString strDisableStyle = "";

SwitchButton::SwitchButton(QWidget* parent) : QToolButton(parent)
{
    connect(this, SIGNAL(clicked()), this, SLOT(R_clicked()));
    m_isBtnChecked  = false;
    m_checkStyle    = strCheckStyle;
    m_unCheckStyle  = strStyle;
    m_disableStyple = strDisableStyle;
    Init();
}

SwitchButton::~SwitchButton()
{
}

void SwitchButton::Init()
{
    if (IsBtnChecked())
    {
        setStyleSheet(m_checkStyle);
    }
    else
    {
        setStyleSheet(m_unCheckStyle);
    }
}

void SwitchButton::SetCheckStyle(const QString& style)
{
    m_checkStyle = style;
}

void SwitchButton::SetUnCheckStyle(const QString& style)
{
    m_unCheckStyle = style;
}

void SwitchButton::SetDisableStyple(const QString& style)
{
    m_disableStyple = style;
}

void SwitchButton::IsBtnChecked(bool val)
{
    m_isBtnChecked = val;
    if (m_isBtnChecked)
    {
        setStyleSheet(m_checkStyle);
    }
    else
    {
        setStyleSheet(m_unCheckStyle);
    }
}

void SwitchButton::IsBtnEnable(bool val)
{
    m_isBtnEnable = val;
    if (m_isBtnEnable)
    {
        this->setEnabled(true);
        this->IsBtnChecked(false);
    }
    else
    {
        this->setEnabled(false);
        setStyleSheet(m_disableStyple);
    }
}

void SwitchButton::DoClicked()
{
    m_isBtnChecked = !m_isBtnChecked;
    if (IsBtnChecked())
    {
        setStyleSheet(m_checkStyle);
    }
    else
    {
        setStyleSheet(m_unCheckStyle);
    }
}

void SwitchButton::R_clicked()
{
    Q_EMIT S_CheckChange(m_isBtnChecked);
    Q_EMIT S_Clicked();
}
