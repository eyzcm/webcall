#pragma once
#include "utils/Plat_EnumDefine.h"
#include "MBaseData.h"
#include "data/CommInfo.h"
#include "data/UIVisitorInfo.h"
#include <QMap>
#include <QObject>
#include "platdata_global.h"

class PLATDTA_EXPORT MMonitorVisitorData : public MBaseData
{
    Q_OBJECT
public:
    MMonitorVisitorData(void);
    MMonitorVisitorData(const MMonitorVisitorData& o);
    ~MMonitorVisitorData(void);
    void InitVisitorInfo(const UIVisitorInfo& info, QString name, QString siteName);

    bool IsFilter(QString searchKey, CommInfo filterInfo);

    bool IsContainByKey(QString strKey, QString val);

public:
    QString Name() const;
    void    Name(QString val);
    QString Vid() const;
    void    Vid(QString val);
    QString Userid() const;
    void    Userid(QString val);

    QString GetUIVisitorInfoValueByKey(QString strKey) const;
    void    SetUIVisitorInfoValueByKey(QString strKey, QString strVal);

    EVISITORSTATUS VisitorStatus() const;
    void           VisitorStatus(EVISITORSTATUS val);

    QMap<QString, QString> VisitorInfoMap() const;
    void                   VisitorInfoMap(QMap<QString, QString> val);

private:
    QString        m_name;
    QString        m_userid;
    QString        m_vid;
    EVISITORSTATUS m_visitorStatus;

    QMap<QString, QString> m_visitorInfoMap;
};
