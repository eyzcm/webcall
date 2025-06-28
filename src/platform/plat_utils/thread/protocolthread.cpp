#include "protocolthread.h"
#include "utils/EL_Util_Time.h"

#include "utils/jsonhelper.h"
#include <qdebug.h>

ProtocolThread::ProtocolThread()
{

    qDebug()<<"moveToThread ProtocolThread"<<endl;

    moveToThread(this);
}

ProtocolThread::~ProtocolThread()
{
}

void ProtocolThread::run()
{
    setThreadName("ProtocolThread");
    InitConnect();
    exec();
}
