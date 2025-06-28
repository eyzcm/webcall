#pragma once
#include <QMap>
#include <QObject>
#include "platdata_global.h"

class PLATDTA_EXPORT MBaseData : public QObject
{
    Q_OBJECT
public:
    MBaseData(void);
    MBaseData(const MBaseData& o);
    ~MBaseData(void);

public:
    void    AddExtendData(QString key, QString val);
    QString GetExtendData(QString key) const;

    QMap<QString, QString> ExtendInfo() const
    {
        return m_extendInfo;
    }
    void ExtendInfo(QMap<QString, QString> val)
    {
        m_extendInfo = val;
    }

protected:
    QMap<QString, QString> m_extendInfo;
};
