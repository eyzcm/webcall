#include "AiControlCmd.h"

AiControlCmd::AiControlCmd(void) : TcpCmd()
{
}

AiControlCmd::~AiControlCmd(void)
{
}

void AiControlCmd::AddExtData(QString name, QString value)
{
    m_exts.insert(name, value);
}

void AiControlCmd::MakeCmd()
{
    QString                        strFormat = "/im/control?type=setExtendInfo&chatId=%1";
    QString                        url       = strFormat.arg(this->ChatId());
    QString                        body      = "";
    QMapIterator<QString, QString> i(m_exts);
    while (i.hasNext())
    {
        if (body.length() > 0)
            body += "&";
        body += i.next().key();
        body += "=";
        body += i.value();
    }

    Uri(url);
    Body(body);
}

void AiControlCmd::SetExtMap(QMap<QString, QString> map)
{
    m_exts = map;
}
