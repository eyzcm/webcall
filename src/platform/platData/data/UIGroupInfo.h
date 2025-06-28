#pragma once
#define ELDATA_EXPORT
#include <QObject>
#include <QStringList>
#include "platdata_global.h"

class PLATDTA_EXPORT UIGroupInfo : public QObject
{
    Q_OBJECT

public:
    UIGroupInfo(void);
    UIGroupInfo(UIGroupInfo *info);
    ~UIGroupInfo(void);

public:
    QString GroupId() const
    {
        return m_groupId;
    }
    void GroupId(QString val)
    {
        m_groupId = val;
    }

    QString GroupName() const
    {
        return m_groupName;
    }
    void GroupName(QString val)
    {
        m_groupName = val;
    }

    QStringList MemberIdList() const
    {
        return m_memberIdList;
    }
    void MemberIdList(QStringList val)
    {
        m_memberIdList = val;
    }

private:
    QString     m_groupId;
    QString     m_groupName;
    QStringList m_memberIdList;
};
