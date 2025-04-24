#include "UI_Wnd_OutputLog2.h"
#include "control/CommonSignalService.h"
#include "protocol/platprotocolservice.h"
#include "service/DbUiWebcallService.h"
#include <QIcon>

UI_Wnd_OutputLog2::UI_Wnd_OutputLog2(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    setWindowTitle(tr("日志输出"));
    setWindowIcon(QIcon(":/Resources/Image/easy.ico"));
    m_count            = 0;
    m_cacheText        = "";
    m_isDisp           = false;
    m_initLoad         = 0;
    m_maxLogCacheCount = 500;
    m_maxLogTextLen    = 1000 * 1000;

    //协议日志
    connect(CommonSignalService::GetInstance(), SIGNAL(S_showText(QString)), this, SLOT(R_showText(QString)));
}

UI_Wnd_OutputLog2::~UI_Wnd_OutputLog2()
{
}

void UI_Wnd_OutputLog2::AddText(QString text)
{
    if (m_initLoad == 0)
    {
        m_initLoad         = 1;
        QString buf        = gUISystemConfigManager->GetSystemConfigByKey("maxLogCacheCount", "500");
        m_maxLogCacheCount = buf.toInt();
        buf                = gUISystemConfigManager->GetSystemConfigByKey("maxLogTextLen", "1000");
        m_maxLogTextLen    = buf.toInt() * 1000;
    }

    m_filterstr = ui.lineEdit->text();
    if (!m_filterstr.isEmpty())
        if (!text.contains(m_filterstr))
            return;
    m_count++;
    if ((m_count > m_maxLogCacheCount) || (m_cacheText.length() > m_maxLogTextLen))
    {
        ClearText();
    }
    m_cacheText += text;
    if (m_isDisp)
        ui.m_panel_output->AddText(text);
}

void UI_Wnd_OutputLog2::ClearText()
{
    ui.m_panel_output->ClearText();
    m_cacheText = "";
    m_count     = 0;
}

void UI_Wnd_OutputLog2::ShowType(bool isShow)
{
    if (isShow)
    {
        ui.m_panel_output->ClearText();
        ui.m_panel_output->AddText(m_cacheText);
        this->show();
    }
    m_isDisp = isShow;
}

void UI_Wnd_OutputLog2::R_showText(QString str)
{
    QDateTime current_time   = QDateTime::currentDateTime();
    QString   StrCurrentTime = current_time.toString("yyyy-MM-dd hh:mm:ss");

    QString text = QString("%1--%2\n").arg(StrCurrentTime).arg(str);

    AddText(text);
}

void UI_Wnd_OutputLog2::closeEvent(QCloseEvent *e)
{
    m_isDisp = false;
    QWidget::closeEvent(e);
}

