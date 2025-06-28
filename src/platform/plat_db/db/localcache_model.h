#ifndef LOCALCACHE_MODEL_H
#define LOCALCACHE_MODEL_H
#include <qtSql>
#include "data/localcache.h"
typedef enum _CACHE_STATUS{
    S_NONE,
    S_OPEN,
    S_CREATETABLE,
    S_CACHE_OK
}CACHE_STATUS;


class LocalCache_Model:public LocalCache
{
public:
    LocalCache_Model();


    static void LoadDb( QSqlDatabase db );

    static bool CreateTable(void);
    static bool DeleteData(QString name );
	static bool DeleteAllMemData();
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



#endif // LOCALCACHE_MODEL_H
