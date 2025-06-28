#include "el_login.h"

#include <control/LoginController.h>

 typedef void(*DLL_INIT_APP)();

 El_login * gInterface = new El_login();

 DllLoadInterface * GetDllInterface()
 {
	 return gInterface;
 }
 DllLoadInterface * el_login_lib_GetDllInterface()
 {
     return gInterface;
 }

El_login::El_login()
{
    
}

El_login::~El_login()
{

}

void El_login::runLogin(QString strCmd)
{

    LoginController::GetInstance()->ShowLoginWnd(strCmd);

}


QString El_login::GetModuleName()
{
	return "El_login";
};

void El_login::Init(int tcount, char * argv[])
{
	LoginController::GetInstance();

}

bool El_login::CheckVersion(QString version)
{
	return true;
}
