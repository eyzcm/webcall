#pragma once

#include <QMap>
#include <QString>

class  CommInfo
{
public:
    CommInfo(void);
    ~CommInfo(void);
    CommInfo(const CommInfo &);
    CommInfo(CommInfo *info);
    static CommInfo        NullInfo;
    void                   AddCommInfo(const QMap<QString, QString> &mapInfo);
    int                    GetInfoSize() const;
    QString                GetValueByKey(const QString &key) const;
    QString                GetValueByKeyHump(const QString &key) const;
    void                   SetValueByKey(const QString &key, const QString &val);
    QMap<QString, QString> InfoMap() const
    {
        return m_infoMap;
    }
    // CZC:判断是否存在 key
    bool HasKey(QString key);

private:
    QMap<QString, QString> m_infoMap;
};


