#include "UI_Wnd_OutputLog.h"
#include "protocol/platprotocolservice.h"
#include <QIcon>

UI_Wnd_OutputLog::UI_Wnd_OutputLog(QWidget *parent) : QWidget(parent)
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
    connect(gIProtocolLoginAgent, SIGNAL(S_HttpReqLog(QString, QString)), this, SLOT(R_HttpReqLog(QString, QString)));
    connect(gIProtocolLoginAgent, SIGNAL(S_HttpResultFinishedLog(QString, QString, QString)), this, SLOT(R_HttpResultFinishedLog(QString, QString, QString)));
    connect(gIProtocolLoginAgent, SIGNAL(S_TcpReqLog(QString, QString)), this, SLOT(R_TcpReqLog(QString, QString)));
    connect(gIProtocolLoginAgent, SIGNAL(S_TcpResultLog(QString, QString)), this, SLOT(R_TcpResultLog(QString, QString)));

}

UI_Wnd_OutputLog::~UI_Wnd_OutputLog()
{
}

void UI_Wnd_OutputLog::AddText(QString text)
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

void UI_Wnd_OutputLog::ClearText()
{
    ui.m_panel_output->ClearText();
    m_cacheText = "";
    m_count     = 0;
}

void UI_Wnd_OutputLog::ShowType(bool isShow)
{
    if (isShow)
    {
        ui.m_panel_output->ClearText();
        ui.m_panel_output->AddText(m_cacheText);
        this->show();
    }
    m_isDisp = isShow;
}

void UI_Wnd_OutputLog::closeEvent(QCloseEvent *e)
{
    m_isDisp = false;
    QWidget::closeEvent(e);
}


void UI_Wnd_OutputLog::R_HttpResultFinishedLog(QString code, QString reason, QString body)
{
    QDateTime current_time   = QDateTime::currentDateTime();
    QString   StrCurrentTime = current_time.toString("yyyy-MM-dd hh:mm:ss");
    QString   text           = QString("--httpresultdata--%1--%2\n%3\n%4\n").arg(StrCurrentTime).arg(code).arg(reason).arg(body);

    AddText(text);
}

void UI_Wnd_OutputLog::R_HttpAiResultFinishedLog(QString code, QString reason, QString body)
{
}

void UI_Wnd_OutputLog::R_HttpReqLog(QString urlstr, QString body)
{
    QDateTime current_time   = QDateTime::currentDateTime();
    QString   StrCurrentTime = current_time.toString("yyyy-MM-dd hh:mm:ss");

    QString text = QString("--httpreqdata--%1--\n%2\n%3\n").arg(StrCurrentTime).arg(urlstr).arg(body);

    AddText(text);
}

void UI_Wnd_OutputLog::R_TcpReqLog(QString msgtype, QString body)
{
    QDateTime current_time   = QDateTime::currentDateTime();
    QString   StrCurrentTime = current_time.toString("yyyy-MM-dd hh:mm:ss");

    QString text = QString("--tcpreqdata--%1----%2----\n%3\n").arg(StrCurrentTime).arg(msgtype).arg(body);

    AddText(text);
}

void UI_Wnd_OutputLog::R_TcpResultLog(QString msgtype, QString body)
{
    QDateTime current_time   = QDateTime::currentDateTime();
    QString   StrCurrentTime = current_time.toString("yyyy-MM-dd hh:mm:ss");

    QString text = QString("--tcpresultdata--%1----%2----\n%3\n").arg(StrCurrentTime).arg(msgtype).arg(body);

   AddText(text);
}
