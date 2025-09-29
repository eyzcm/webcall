#include "utilshelper.h"
#include "ConstStringHelper.h"
#include "gframework.h"

#include <QDir>
#include <QProcess>





UtilsHelper::UtilsHelper(QObject *parent)
	: QObject(parent)
{

}

UtilsHelper::~UtilsHelper()
{

}

void UtilsHelper::Execute(QString body)
{
	body = body.toUtf8().toBase64();
    QString result = ConstStringHelper::ProtocolFormat.arg( ConstStringHelper::Log_Console ).arg( ConstStringHelper::RouteMonitor).arg( body );
	STDPRINTF(result.toUtf8().data());
}


void UtilsHelper::Execute(QString receiver, QString body)
{
	body = body.toUtf8().toBase64();
    QString result = ConstStringHelper::ProtocolFormat.arg( ConstStringHelper::Log_Console ).arg(receiver).arg( body );
	STDPRINTF(result.toUtf8().data());
}

QString UtilsHelper::GetAppdataLogDir()
{
    QString logpath = GetAppdataCacheDir();
    logpath.replace("\\", "/");
    QString pathDir = QString("%1/%2").arg(logpath).arg("imlog");
    QDir    dir;
    if (!dir.exists(pathDir))
    {
        dir.mkdir(pathDir);
    }
    return pathDir;
}

QString UtilsHelper::GetAppdataCacheDir()
{
    QString appdataPath = QProcessEnvironment::systemEnvironment().value("APPDATA");
    appdataPath.replace("\\", "/");
    QString pathDir = QString("%1/%2").arg(appdataPath).arg("ECIM");
    QDir    dir;
    if (!dir.exists(pathDir))
    {
        dir.mkdir(pathDir);
    }
    return pathDir;
}
