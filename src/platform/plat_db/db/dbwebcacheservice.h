#ifndef DBWEBCACHESERVICE_H
#define DBWEBCACHESERVICE_H

#include <QtSql>
#include "db/cachedbinterface.h"
#include "utils/UISingletonTimer.h"


class DbWebCacheService:public CacheDbInterface
{
    Q_OBJECT
public:
    static DbWebCacheService* GetInstance();

    DbWebCacheService();
    ~DbWebCacheService();

    static DbWebCacheService * m_instance;

public:
    virtual void Init( QString loginId );
    virtual void    SaveCacheData( const QString key, const QString value, int saveType = 0, int saveTime = -1 );
    virtual QString GetCacheData(  const QString key ) ;
    virtual void RemoveCacheData(const QString key);
    virtual void RemoveCacheDataByFilter(const QString key);
    virtual  void ClearCacheData();
    virtual  QMap<QString,QString>  GetCacheListByFilter(const QString prekey) ;

private slots:
    void R_CacheTimeout();


private:

    void UpdateCacheTime( QString key,int saveTime ,int type);
    QString m_tablename;

    QSqlDatabase m_memdb;
    QSqlDatabase m_filedb;

    QMap<Yl_SingleTimer*, QString> m_cacheValue;


    int     m_init;

};
#endif // DBWEBCACHESERVICE_H
