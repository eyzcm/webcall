#pragma once
#include "platdata_global.h"
#include <QObject>
#include <QStringList>

class PLATDTA_EXPORT UISiteInfo : public QObject
{
    Q_OBJECT

public:
    UISiteInfo(void);
    UISiteInfo(UISiteInfo *info);
    ~UISiteInfo(void);

public:
    QString SiteId() const
    {
        return m_siteId;
    }
    void SiteId(QString val)
    {
        m_siteId = val;
    }

    QString Name() const
    {
        return m_name;
    }
    void Name(QString val)
    {
        m_name = val;
    }

private:
    QString m_siteId;
    QString m_name;
};
