#include "UILoginData.h"

UILoginData::UILoginData(void)
{
    this->LoginType(0);
}

UILoginData::UILoginData(const UILoginData& info)
{
    this->IsAutoLogin(info.IsAutoLogin());
    this->IsRemember(info.IsRemember());
    this->UserState(info.UserState());
    this->UserName(info.UserName());
    this->Password(info.Password());
    this->LoginType(info.LoginType());
}

UILoginData::UILoginData(UILoginData* pinfo)
{
    if (pinfo == nullptr)
        return;

    this->IsAutoLogin(pinfo->IsAutoLogin());
    this->IsRemember(pinfo->IsRemember());
    this->UserState(pinfo->UserState());
    this->UserName(pinfo->UserName());
    this->Password(pinfo->Password());
    this->LoginType(pinfo->LoginType());
}

UILoginData::~UILoginData(void)
{
}
