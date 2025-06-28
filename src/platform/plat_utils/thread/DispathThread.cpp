#include "DispathThread.h"
#include "utils/ConstStringHelper.h"
#include <QTextStream>
#ifdef _MSC_VER
#include <Windows.h>
#endif
#include <qdebug.h>

DispathThread::DispathThread() : QThread()
{
    qDebug()<<"DispathThread"<<endl;
    
    moveToThread(this);
}

DispathThread::~DispathThread()
{
}

void DispathThread::run()
{
    Q_EMIT      InitTimer();
    QTextStream cin(stdin, QIODevice::ReadOnly);
    while (!cin.atEnd())
    {
        QString line = cin.readLine();
        line         = QString::fromUtf8(line.toLocal8Bit());
        auto pos     = line.indexOf(ConstStringHelper::troy);
        if (pos < 0)
            continue;
        line = line.remove(0, ConstStringHelper::troy.length()).toUtf8();

        Q_EMIT ReceiveCmdData(line);
    }
    // exec();
}
