#pragma once
#include "utils/DefineHelper.h"


class  LoginData
{
public:
    LoginData(void);
    ~LoginData(void);
    Property_BOOL(IsAutoLogin, isAutoLogin);
    Property_BOOL(IsRemember, isRemember);
    Property_Int(UserState, userState);
    Property_QString(UserName, userNamer);
    Property_QString(Password, password);
};
