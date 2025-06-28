#include "UIGroupInfo.h"

UIGroupInfo::UIGroupInfo(void)
{
}

UIGroupInfo::~UIGroupInfo(void)
{
}

UIGroupInfo::UIGroupInfo(UIGroupInfo *info)
{
    if (info)
    {
        GroupId(info->GroupId());
        GroupName(info->GroupName());
        MemberIdList(info->MemberIdList());
    }
}
