#pragma once
#include "utils/DefineHelper.h"

#include <QObject>

class  UILoginData : public QObject
{
    Q_OBJECT
public:
    UILoginData(void);
    UILoginData(const UILoginData& info);
    UILoginData(UILoginData* pinfo);
    ~UILoginData(void);

public:
    Property_BOOL(IsAutoLogin, isAutoLogin);
    Property_BOOL(IsRemember, isRemember);
    Property_Int(UserState, userState);
    Property_QString(UserName, userNamer);
    Property_QString(Password, password);
    Property_Int(LoginType, loginType); //为0表示密码方式，为1表示token方式

private:
};
