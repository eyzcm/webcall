#include "DispathThread.h"
#include <QTextStream>
#include "ConstStringHelper.h"

DispathThread::DispathThread()
    : QThread()
{
    start();
    moveToThread( this );
}

DispathThread::~DispathThread()
{

}

void DispathThread::run()
{
    QTextStream cin( stdin, QIODevice::ReadOnly );    
    while ( !cin.atEnd() )
    {
        QString line = cin.readLine();
		line = QString::fromUtf8(line.toLocal8Bit());
		auto pos = line.indexOf( ConstStringHelper::Troy);
		if ( pos < 0 )
			continue;
		line =  line.remove( 0, ConstStringHelper::Troy.length() ).toUtf8();
		emit ReceiveCmdData(line);
	}
}


