#ifndef LOG_CONSOLE_H
#define LOG_CONSOLE_H
#include "log_console_global.h"

#include <dllloadinterface.h>

extern "C" LOG_CONSOLE_EXPORT DllLoadInterface * GetDllInterface();
extern "C" LOG_CONSOLE_EXPORT DllLoadInterface * log_console_lib_GetDllInterface();


class log_console:public DllLoadInterface
{
public:
    log_console();

    virtual QString GetModuleName();
    virtual void Init( int  ,  char * argv[]  );
    virtual bool CheckVersion( QString version );
};

#endif // LOG_CONSOLE_H
