#ifndef LOCALWEBCACHE_MODEL_H
#define LOCALWEBCACHE_MODEL_H


#include <qtSql>
#include "data/localcache.h"
#include "db/localcache_model.h"

class LocalWebCache_Model:public LocalCache
{
public:
    LocalWebCache_Model();


    static void LoadDb( QSqlDatabase db );

    static bool CreateTable(void);
    static bool DeleteData(QString name );
    static bool DeleteDataByFiter(QString name );
    static bool DeleteAllMemData();
    static bool ClearData();
    static QList<LocalCache *> QueryInitList();

    // 判断数据表是否存在
    static  bool IsTableExist();

    bool Insert();
    bool Update();

    bool HasRecord();



    QList<LocalCache *> QueryList(const QString where);
    LocalCache * QueryListByName(const QString name);

public:
    static  QString m_tableName;

private:


    static CACHE_STATUS   m_status;
    static QSqlDatabase m_db;
};

#endif // LOCALWEBCACHE_MODEL_H
