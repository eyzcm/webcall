#include "KillProcessHelper.h"
#ifdef Q_OS_WIN32
#include <windows.h>

#endif
KillProcessHelper::KillProcessHelper() : QObject()
{
}

KillProcessHelper::~KillProcessHelper()
{
}

void KillProcessHelper::AdjustPrivilege()
{
#ifdef Q_OS_WIN32    
    HANDLE hToken;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
    {
        TOKEN_PRIVILEGES tp;
        tp.PrivilegeCount           = 1;
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid))
        {
            AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
        }
        CloseHandle(hToken);
    }
#endif    
}

void KillProcessHelper::DoExit(Q_PID dPid, int ncode)
{
#ifdef Q_OS_WIN32
    AdjustPrivilege();
    // YGLOG::qLOG->Log4Message( "KillProcessHelper", QString( "exit process code: %1,current process pid:%2" ).arg( ncode ).arg( ( qlonglong )dPid ) );
    TerminateProcess(OpenProcess(PROCESS_ALL_ACCESS, FALSE, dPid->dwProcessId), ncode);
#endif
}
