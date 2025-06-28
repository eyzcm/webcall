#ifndef EL_LOGIN_H
#define EL_LOGIN_H

#include "el_login_global.h"
#include "DllLoadInterface.h"

extern "C" EL_LOGIN_EXPORT DllLoadInterface * GetDllInterface();
extern "C" EL_LOGIN_EXPORT DllLoadInterface * el_login_lib_GetDllInterface();


class EL_LOGIN_EXPORT El_login :public DllLoadInterface
{
public:
    El_login();
    ~El_login();

    void runLogin(QString strCmd);
	virtual QString GetModuleName();
	virtual void Init(int, char * argv[]);
	virtual bool CheckVersion(QString version);
};

#endif // EL_LOGIN_H
