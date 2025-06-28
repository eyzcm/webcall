#include "UINewVersionInfo.h"

UINewVersionInfo::UINewVersionInfo(void)
{
    m_Download_Status = EDOWNLOADSTATUS_NOTSTARTED;
}

UINewVersionInfo::~UINewVersionInfo(void)
{
}

UINewVersionInfo::UINewVersionInfo(UINewVersionInfo *info)
{
    if (info)
    {
        Version(info->Version());
        DownloadUrl(info->DownloadUrl());
        DownloadStatus(info->DownloadStatus());
        Local_filepath(info->Local_filepath());
        Update_description(info->Update_description());
    }
}
