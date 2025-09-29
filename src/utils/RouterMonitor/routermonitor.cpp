#include "RMAgent.h"
#include "routermonitor.h"

#include <QCoreApplication>


RouterMonitor * gInterface = new RouterMonitor();

DllLoadInterface * GetDllInterface()
{
	return gInterface;
}
DllLoadInterface * RouterMonitor_lib_GetDllInterface()
{
    return gInterface;
}


RouterMonitor::RouterMonitor()
{
}


QString RouterMonitor::GetModuleName()
{
    return "RouterMonitor";
};

void RouterMonitor::Init( int argc ,  char * argv[]  )
{

    QString strCmd = "";
    for (int i = 0; i < argc; i++)
    {
        if (QString(argv[i]) == "logout")
        {
            strCmd = argv[i];
        }
    }

    RMAgent::instance()->start(strCmd);

}

bool RouterMonitor::CheckVersion( QString version )
{
    return false;
}
