#include "gframework.h"
#include <QRegExp>
#include "ConstStringHelper.h"
#include <QDebug>
#include <QDomDocument>

#include "RequestKeepLive.h"
#include "utilshelper.h"
#include "DefineHelper.h"
#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"  // support for loading levels from the environment variable
#include "spdlog/fmt/ostr.h" // support for user defined types

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"


extern std::shared_ptr<spdlog::logger> g_rotating_logger;
extern  std::shared_ptr<spdlog::logger> g_rotating_protocollogger;
extern  std::shared_ptr<spdlog::logger> g_rotating_errorlogger;
extern std::shared_ptr<spdlog::logger> g_rotating_testlogger;
extern std::shared_ptr<spdlog::logger> g_rotating_debuglogger;
extern std::shared_ptr<spdlog::logger> g_rotating_scriptlogger;
extern std::shared_ptr<spdlog::logger> g_rotating_webcallinterfacelogger;

GFramework* GFramework::m_instance = nullptr;

GFramework::GFramework()
	: QObject()
{

    
	m_subtimer = nullptr;

	m_InitCef = false;

    


}

GFramework::~GFramework()
{

	if (m_subtimer)
	{
		m_subtimer->stop();
	}
}

GFramework* GFramework::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new GFramework;
	}
	return m_instance;
}

void GFramework::Init()
{
	m_dispThread = new DispathThread;
	connect( m_dispThread, SIGNAL( ReceiveCmdData( QString ) ), this, SLOT( R_OnReceiveCmdTask( const QString& ) ) );
	CreateTimer();


}


void GFramework::R_OnReceiveCmdTask(QString cmd)
{
	//Sender：ddd；Receiver：sss，sss；Body:uuu;
	do
	{
		QRegExp reg( "^Sender\\s*:(.*);\\s*Receiver\\s*:(.*);\\s*Body\\s*:(.*)" );
		if ( reg.indexIn( cmd  ) != -1 )
		{
			QString Sender = reg.cap( 1 ).trimmed();
			QString Receiver = reg.cap( 2 ).trimmed();

			if ( Sender.isEmpty() )
			{
				break;
			}

            if (
                Receiver.compare( ConstStringHelper::Log_Console, Qt::CaseInsensitive ) == 0
				)
			{
				QString Body = reg.cap( 3 ).trimmed();
				QString data = QString::fromUtf8( QByteArray::fromBase64( Body.toUtf8() ) );

				QString info = QString( "RevProtocol:%1\n" ).arg( data );
				qDebug()<<info;			
				DoReceive(Sender, data);
			}
		}
	}
	while ( false );
}

void GFramework::SendCmd(QString sender, QString receiver, QString body)
{
	QString content = body.toUtf8().toBase64();
	QString result = ConstStringHelper::ProtocolFormat.arg( sender ).arg( receiver ).arg( content );
	STDPRINTF(result.toUtf8().data());
}

void GFramework::DoReceive(QString sender, QString body)
{
	if (sender == ConstStringHelper::EasyLiao)
	{

		QString type = GetProtocolType(body);

		if (StringCompare(type, "LogInfo"))
		{
			//输出
			QDomDocument doc;
			doc.setContent(body);
			QDomElement docElem = doc.documentElement();
			auto m = Convert2Map(docElem);

			QString msg = m["msg"];
			QString filename = m["filename"];
			QString line = m["line"];
			QString type = m["type"];
			
			if( type == "protocol")
				g_rotating_protocollogger->info(msg.toStdString().c_str());
            else if( type == "interface")
                g_rotating_webcallinterfacelogger->info(msg.toStdString().c_str());
            else if (type == "error")
			{
				QString logtext = QString("filename=%1\r\nline=%2\r\n%3").arg(filename, line, msg);
				g_rotating_errorlogger->info(logtext.toStdString().c_str());
				

			}
            else if (type == "test")
            {
                QString logtext = QString("filename=%1\r\nline=%2\r\n%3").arg(filename, line, msg);
                g_rotating_testlogger->info(logtext.toStdString().c_str());
				g_rotating_testlogger->flush();


            }
            else if (type == "debug")
            {
                QString logtext = QString("filename=%1\r\nline=%2\r\n%3").arg(filename, line, msg);
                g_rotating_debuglogger->info(logtext.toStdString().c_str());

            }
            else if (type == "script")
            {
                QString logtext = QString("filename=%1\r\nline=%2\r\n%3").arg(filename, line, msg);
                g_rotating_scriptlogger->info(logtext.toStdString().c_str());

            }
            else
			{
				QString logtext = QString("filename=%1\r\nline=%2\r\n%3").arg(filename, line, msg);
				g_rotating_logger->info( logtext.toStdString().c_str());
			}

		}
		else if (StringCompare(type, ConstStringHelper::BizKeepAlive))
		{
			Do_KeepAlive(body);
		}
		else
		{
			Q_ASSERT(false);
		}

	}
	else if (sender == ConstStringHelper::RouteMonitor)
	{
		//监控进程协议


	}
	else
	{
		//do nothing;

	}
}

QString GFramework::GetProtocolType( const QString& body )
{
	QDomDocument doc;
	doc.setContent(body);	
	QDomElement docElem = doc.documentElement();
	QString type = docElem.tagName().trimmed();
	return type;
}

void GFramework::Do_KeepAlive(const QString& body)
{
	/*if (m_subtimer)
	{
		m_subtimer->start(15*1000);
	}*/
}

void GFramework::CreateTimer()
{
	
	if (m_subtimer == nullptr)
	{
		m_subtimer = new QTimer(this);
		connect( m_subtimer, SIGNAL( timeout() ), this, SLOT( SubKeepAlive() ) );
	}
	m_subtimer->start( 15 * 1000 );
}

void GFramework::SubKeepAlive()
{
	RequestKeepLive req;	
	QString body = req.MakeXml();
	UtilsHelper::Execute(body);
}


QMap<QString, QString> GFramework::Convert2Map(const QDomElement& ele)
{
	QDomNode n = ele.firstChild();
	QMap<QString, QString> valuemap;
	while ( !n.isNull() )
	{
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if ( !e.isNull() )
		{
			QString strTagName = e.tagName().trimmed();
			QString strValue = e.text().trimmed();
			valuemap[strTagName] = strValue;
		}
		n = n.nextSibling();
	}

	return valuemap;
}

