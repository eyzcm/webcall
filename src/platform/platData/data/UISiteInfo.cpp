#include "UISiteInfo.h"

UISiteInfo::UISiteInfo(void)
{
}

UISiteInfo::~UISiteInfo(void)
{
}

UISiteInfo::UISiteInfo(UISiteInfo *info)
{
    if (info)
    {
        SiteId(info->SiteId());
        Name(info->Name());
    }
}
