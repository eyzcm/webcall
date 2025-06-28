#include "UIBackgroundColorWidget.h"
#include "service/DbUiWebcallService.h"
#include "data/UIShowSettingInfo.h"

#define BACKGROUP_COLOR_DEF \
    "QRadioButton\
{\
        background-color: rgba(%1);\
}"

UIBackgroundColorWidget::UIBackgroundColorWidget(QWidget *parent /*= 0*/)
{
    ui.setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint);
    m_chatOpacity = 100;
    m_chatColor   = "#F7F7F7";
    m_editOpacity = 100;
    m_editColor   = "#F7F7F7";
    init();
}

QString UIBackgroundColorWidget::QColorToQString(QColor color, int apl)
{
    QString strR     = QString::number(color.red());
    QString strG     = QString::number(color.green());
    QString strB     = QString::number(color.blue());
    QString strA     = QString::number(apl);
    QString strColor = strR + "," + strG + "," + strB + "," + strA;
    return strColor;
}

UIBackgroundColorWidget::~UIBackgroundColorWidget()
{
}

void UIBackgroundColorWidget::init()
{
    this->setWindowIcon(QIcon(":/Resources/Image/backgroup_color_head.png"));

    connect(ui.m_colorSelect, SIGNAL(s_changeColor(QString)), this, SLOT(R_changeColor(QString)));
    connect(ui.m_colorSelect, SIGNAL(s_changeOpacity(int)), this, SLOT(R_changeOpacity(int)));
    connect(ui.m_reset_btn, SIGNAL(clicked()), this, SLOT(R_Reset()));
    connect(ui.m_canclebtn, SIGNAL(clicked()), this, SLOT(R_Close()));
    connect(ui.m_savebtn, SIGNAL(clicked()), this, SLOT(R_Save()));
    connect(ui.m_chatbackcolor, SIGNAL(clicked(bool)), this, SLOT(R_setChatChecked(bool)));
    connect(ui.m_editbackcolor, SIGNAL(clicked(bool)), this, SLOT(R_setEditChecked(bool)));

    UIShowSettingInfo *showInfo = gUIPersonalConfigYLService->ShowSettingInfo();
    if (showInfo)
    {
        m_chatColor   = showInfo->ChatAreaBackColor();
        m_chatOpacity = showInfo->ChatAreaOpacity();
        m_editColor   = showInfo->UeditAreaBackColor();
        m_editOpacity = showInfo->UeditAreaOpacity();
        ui.m_chatbackcolor->setChecked(true);
        ui.m_colorSelect->LoadColor(m_chatColor);
        ui.m_colorSelect->LoadOpacity(m_chatOpacity);
        ui.m_chatbackcolor->setStyleSheet(QString(BACKGROUP_COLOR_DEF).arg(QColorToQString(m_chatColor, m_chatOpacity * 255 / 100)));
        ui.m_editbackcolor->setStyleSheet(QString(BACKGROUP_COLOR_DEF).arg(QColorToQString(m_editColor, m_editOpacity * 255 / 100)));
        showInfo->deleteLater();
    }
}

void UIBackgroundColorWidget::R_Reset()
{
    m_chatOpacity = 100;
    m_chatColor   = "#F7F7F7";
    m_editOpacity = 100;
    m_editColor   = "#F7F7F7";
    ui.m_chatbackcolor->setChecked(true);
    ui.m_colorSelect->LoadColor(m_chatColor);
    ui.m_colorSelect->LoadOpacity(m_chatOpacity);

    ui.m_chatbackcolor->setStyleSheet(QString(BACKGROUP_COLOR_DEF).arg(QColorToQString(m_chatColor, m_chatOpacity * 255 / 100)));
    ui.m_editbackcolor->setStyleSheet(QString(BACKGROUP_COLOR_DEF).arg(QColorToQString(m_editColor, m_editOpacity * 255 / 100)));
}

void UIBackgroundColorWidget::R_Close()
{
    this->hide();
}

void UIBackgroundColorWidget::R_Save()
{
    UIShowSettingInfo *showInfo = gUIPersonalConfigYLService->ShowSettingInfo();
    if (showInfo)
    {
        showInfo->ChatAreaBackColor(m_chatColor);
        showInfo->ChatAreaOpacity(m_chatOpacity);
        showInfo->UeditAreaBackColor(m_editColor);
        showInfo->UeditAreaOpacity(m_editOpacity);
        gUIPersonalConfigYLService->ShowSettingInfo(showInfo);
        showInfo->deleteLater();
    }
    R_Close();
}

void UIBackgroundColorWidget::R_changeColor(QString color)
{
    if (ui.m_chatbackcolor->isChecked())
    {
        m_chatColor = color;

        ui.m_chatbackcolor->setStyleSheet(QString(BACKGROUP_COLOR_DEF).arg(QColorToQString(m_chatColor, m_chatOpacity * 255 / 100)));
    }
    else
    {
        m_editColor = color;

        ui.m_editbackcolor->setStyleSheet(QString(BACKGROUP_COLOR_DEF).arg(QColorToQString(m_editColor, m_editOpacity * 255 / 100)));
    }
}
void UIBackgroundColorWidget::R_changeOpacity(int val)
{
    if (ui.m_chatbackcolor->isChecked())
    {
        m_chatOpacity = val;

        ui.m_chatbackcolor->setStyleSheet(QString(BACKGROUP_COLOR_DEF).arg(QColorToQString(m_chatColor, m_chatOpacity * 255 / 100)));
    }
    else
    {
        m_editOpacity = val;

        ui.m_editbackcolor->setStyleSheet(QString(BACKGROUP_COLOR_DEF).arg(QColorToQString(m_editColor, m_editOpacity * 255 / 100)));
    }
}

void UIBackgroundColorWidget::R_setChatChecked(bool checked)
{
    bool bval = checked;
    if (bval)
    {
        ui.m_colorSelect->LoadColor(this->m_chatColor);
        ui.m_colorSelect->LoadOpacity(this->m_chatOpacity);
    }
}
void UIBackgroundColorWidget::R_setEditChecked(bool checked)
{
    bool bval = checked;
    if (bval)
    {
        ui.m_colorSelect->LoadColor(this->m_editColor);
        ui.m_colorSelect->LoadOpacity(this->m_editOpacity);
    }
}
