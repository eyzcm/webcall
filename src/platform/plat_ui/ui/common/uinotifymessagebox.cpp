#include "uinotifymessagebox.h"
#include <QDesktopServices>
#include <QUrl>

#include <ui_uinotifymessagebox.h>

UINotifyMessageBox::UINotifyMessageBox(QWidget* parent,QString token) : QWidget(parent)
  , ui(new Ui::UINotifyMessageBox)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

    m_category      = "";
    m_isCloseEnable = true;
    m_isCrm_UrlType = false;
    connect(ui->m_btnOk, SIGNAL(clicked()), this, SLOT(R_OKClicked()));

    m_timer = new QTimer;
    connect(m_timer, SIGNAL(timeout()), this, SLOT(R_OnTimeOut()));
    m_timer->setInterval(30 * 1000);
    m_timer->start();
    

}

UINotifyMessageBox::~UINotifyMessageBox()
{
    if (m_timer)
    {
        m_timer->deleteLater();
        m_timer = nullptr;
    }
}

void UINotifyMessageBox::SetContentTip(QString tip)
{
    ui->m_tipContent->setText(tip);
}

void UINotifyMessageBox::SetLinkBtnText(QString txt)
{
    ui->m_btnOk->setText(txt);
}

void UINotifyMessageBox::SetLinkUrl(const QString& url)
{
    if (!url.isEmpty())
    {
        m_url = url;
    }
}

void UINotifyMessageBox::SetCategory(const QString& category)
{
    m_category = category;
}

void UINotifyMessageBox::SetUrlType(bool isCrm)
{
    m_isCrm_UrlType = isCrm;
}

void UINotifyMessageBox::IsCloseEnable(bool iscloseEnable)
{
    m_isCloseEnable = iscloseEnable;
    if (!m_isCloseEnable)
    {
        if (m_timer)
        {
            m_timer->stop();
        }
    }
}

QString UINotifyMessageBox::GetCategory()
{
    return m_category;
}

void UINotifyMessageBox::R_OKClicked()
{
    m_timer->stop();
    if (!m_url.isEmpty())
    {
        Q_EMIT S_OpenCrm(m_url);
        Q_EMIT S_GotoUrlBegin(m_category);
        Q_EMIT S_Closed();
    }
}

void UINotifyMessageBox::R_OnTimeOut()
{
    Q_EMIT S_GotoUrlBegin(m_category);
    Q_EMIT S_Closed();
    m_timer->stop();
    this->deleteLater();
}
