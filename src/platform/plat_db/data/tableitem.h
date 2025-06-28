#ifndef TableItem_H
#define TableItem_H
#include <qobject.h>

class  TableItem
{
public:
    TableItem();

    QString Name() const
    {
        return m_name;
    }

    void Name(const QString name )
    {
        m_name =name;
    }


    QString Version() const
    {
        return m_version;
    }

    void Version(const QString value )
    {
        m_version = value;
    }
    QString Itemsjson() const
    {
        return m_itemsjson;
    }

    void Itemsjson(const QString value )
    {
        m_itemsjson = value;
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
    QString m_version;
    QString m_itemsjson;
    int m_saveTime;  //保存时间，以秒为单位
    int m_saveType; //0 内存 1 持久


};

#endif // TableItem_H
