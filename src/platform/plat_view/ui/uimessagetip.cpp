#include "uimessagetip.h"
#include "ui_uimessagetip.h"

UIMessageTip::UIMessageTip(QWidget *parent) : QWidget(parent)
  , ui(new Ui::UIMessageTip)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    m_timer = new QTimer;
    connect(m_timer, SIGNAL(timeout()), this, SLOT(R_OnTimeOut()));
    m_timer->setInterval(30 * 1000);
    m_timer->start();
    connect(ui->m_btnOk, SIGNAL(clicked()), this, SLOT(R_OKClicked()));
}

UIMessageTip::~UIMessageTip()
{
    if (m_timer)
    {
        m_timer->deleteLater();
        m_timer = nullptr;
    }
}

void UIMessageTip::SetContentTip(QString tip)
{
    ui->m_tipContent->setText(tip);
}

void UIMessageTip::R_OKClicked()
{
    Q_EMIT S_Closed();
}

void UIMessageTip::R_OnTimeOut()
{
    Q_EMIT S_Closed();
}

void UIMessageTip::SetTimerInterval(int val)
{
    if (m_timer)
    {
        m_timer->setInterval(val);
        m_timer->start();
    }
}
