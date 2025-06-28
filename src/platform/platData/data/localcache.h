#ifndef LOCALCACHE_H
#define LOCALCACHE_H
#include <qobject.h>
#include "platdata_global.h"
class PLATDTA_EXPORT LocalCache
{
public:
    LocalCache();

    QString Name() const
    {
        return m_name;
    }

    void Name(const QString name )
    {
        m_name =name;
    }

    QString Value() const
    {
        return m_value;
    }

    void Value(const QString value )
    {
        m_value = value;
    }

    int SaveTime()
    {
        return m_saveTime;
    }

    void SaveTime( int saveTime )
    {
        m_saveTime = saveTime;
    }

    int SaveType()
    {
        return m_saveType;
    }

    void SaveType( int saveType )
    {
        m_saveType = saveType;
    }



protected:
    int m_id;
    QString m_name;
    QString m_value;
    int m_saveTime;  //保存时间，以秒为单位
    int m_saveType; //0 内存 1 持久


};

#endif // LOCALCACHE_H
