#include "DbUiUserInfoInterface.h"



void DbUiUserInfoInterface::UserInfo(UIUserInfo* info)
{
}

void DbUiUserInfoInterface::LoginData(UILoginData* info)
{
 
}

UIUserInfo* DbUiUserInfoInterface::UserInfo()
{
 	return NULL;
}

UIUserInfo* DbUiUserInfoInterface::CreateInfo(UIUserInfo* info)
{
    return NULL;
}

UILoginData* DbUiUserInfoInterface::CreateLoginInfo(UILoginData* info)
{
    return NULL;
}

QString DbUiUserInfoInterface::LockPassword()
{
    return NULL;
}
void DbUiUserInfoInterface::LockPassword(QString password)
{

}

void DbUiUserInfoInterface::SetNewToken(QString token)
{

}
