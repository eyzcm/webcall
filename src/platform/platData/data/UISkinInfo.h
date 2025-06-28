#pragma once
#include "QMap"
#include "platdata_global.h"
#include <QObject>

class PLATDTA_EXPORT UISkinInfo : public QObject
{
    Q_OBJECT
public:
    UISkinInfo(void);
    UISkinInfo(const UISkinInfo& info);
    UISkinInfo(UISkinInfo* info);
    ~UISkinInfo(void);

public:
    QString Name() const;
    void    Name(QString val);

    QString Key() const;
    void    Key(QString val);

    QString FileName() const;
    void    FileName(QString val);

    int     GroupId() const;
    QString GroupName() const;

    void GroupId(int gid);
    void GroupName(QString name);

    void                   LoadMapInfo(const QMap<QString, QString>& mapInfo, bool ch = true);
    QMap<QString, QString> InfoMap() const;

private:
    int     m_groupId;
    QString m_groupName;
    QString m_key;
    QString m_name;
    QString m_filename;
};
