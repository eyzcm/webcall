#ifndef DBCACHESERVICE_H
#define DBCACHESERVICE_H
#include <QtSql>
#include "db/cachedbinterface.h"
#include "utils/UISingletonTimer.h"

class DbCacheService:public CacheDbInterface
{
    Q_OBJECT
public:
    static DbCacheService* GetInstance();

    DbCacheService();
    ~DbCacheService();

    static DbCacheService * m_instance;

public:
    virtual void Init( QString loginId );
    virtual void    SaveCacheData( const QString key, const QString value, int saveType = 0, int saveTime = -1 );
    virtual QString GetCacheData(  const QString key ) ;
	virtual void RemoveCacheData(const QString key);

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

#endif // DBCACHESERVICE_H
