#pragma once
#include "platdata_global.h"
#include <QObject>
#include <QStringList>

//更新包下载状态
enum EDOWNLOADSTATUS
{
    EDOWNLOADSTATUS_NOTSTARTED         = 0, //未开始
    EDOWNLOADSTATUS_DOWNLOADING        = 1, //下载中
    EDOWNLOADSTATUS_DOWNLOADSUCCESSFUL = 2, //下载成功
    EDOWNLOADSTATUS_DOWNLOADFAILED     = 3  //下载失败
};

class PLATDTA_EXPORT UINewVersionInfo : public QObject
{
    Q_OBJECT

public:
    UINewVersionInfo(void);
    UINewVersionInfo(UINewVersionInfo *info);
    ~UINewVersionInfo(void);

public:
    QString Version() const
    {
        return m_version;
    }
    void Version(QString val)
    {
        m_version = val;
    }

    QString DownloadUrl() const
    {
        return m_downloadUrl;
    }
    void DownloadUrl(QString val)
    {
        m_downloadUrl = val;
    }

    QString Local_filepath() const
    {
        return m_Local_filepath;
    }
    void Local_filepath(QString val)
    {
        m_Local_filepath = val;
    }

    QString Update_description() const
    {
        return m_Update_description;
    }
    void Update_description(QString val)
    {
        m_Update_description = val;
    }

    EDOWNLOADSTATUS DownloadStatus() const
    {
        return m_Download_Status;
    }
    void DownloadStatus(EDOWNLOADSTATUS val)
    {
        m_Download_Status = val;
    }

private:
    QString         m_version;
    QString         m_downloadUrl;
    QString         m_Local_filepath;  //本地更新包地址
    EDOWNLOADSTATUS m_Download_Status; //下载状态
    QString         m_Update_description;
};
