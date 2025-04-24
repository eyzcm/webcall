#include "UI_Panel_CardSavePrompt.h"
#include "utils/EnumDefine.h"

void UI_Panel_CardSavePrompt::SetPromptType(bool success)
{
    if (success)
    {
        ui.label_icon->setStyleSheet("border-image: url(:/YL/Resources/YL/Card/icon_cardSaveSuccess.png);");

        ui.label_msg->setText(tr("名片保存成功！"));
        ui.label_msg->setStyleSheet("color:#22a52c; font: 14px \""+tr("微软雅黑")+"\";");
    }
    else
    {
        ui.label_icon->setStyleSheet("border-image: url(:/YL/Resources/YL/Card/icon_cardSaveFailure.png);");

        ui.label_msg->setText(tr("名片保存失败！"));
        ui.label_msg->setStyleSheet("color:#fa2b3f; font: 14px \""+tr("微软雅黑")+"\";");
    }
}

UI_Panel_CardSavePrompt::UI_Panel_CardSavePrompt(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);

    m_isHide = false;
    m_timer  = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(R_Timout()));
}

UI_Panel_CardSavePrompt::~UI_Panel_CardSavePrompt()
{
}

void UI_Panel_CardSavePrompt::show()
{
    if (parentWidget())
    {
        this->setGeometry(0, 0, parentWidget()->width(), parentWidget()->height());
    }

    m_isHide = false;

    m_timer->stop();
    m_timer->start(ToastPromptTime_Sec * 1000);
    if (!m_isHide)
    {
        QWidget::show();
    }
}

void UI_Panel_CardSavePrompt::hide()
{
    m_isHide = true;
    m_timer->stop();

    if (m_isHide)
    {
        QWidget::hide();
    }
}

void UI_Panel_CardSavePrompt::R_Timout()
{
    m_isHide = true;
    m_timer->stop();
    if (m_isHide)
    {
        this->setVisible(false);
    }
}
