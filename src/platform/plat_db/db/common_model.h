#ifndef Common_Model_H
#define Common_Model_H
#include <qtSql>
#include "data/tableitem.h"

typedef enum _Common_STATUS{
    S_NONE,
    S_OPEN,
    S_CREATETABLE,
    S_CACHE_OK
}CACHE_STATUS;


class Common_Model:public TableItem
{
public:
    Common_Model();


    static void LoadDb( QSqlDatabase db );

    static bool CreateTable(void);
    static bool DeleteData(QString name ,QString version);
	static bool DeleteAllMemData();
    static QList<TableItem *> QueryInitList();


private:
    // 判断数据表是否存在
    static  bool IsTableExist();

    bool Insert();
    bool Update();
    bool HasRecord();
    QList<TableItem *> QueryList(const QString where);
    TableItem * QueryListByName(const QString name,const QString version );

public:
    static  QString m_tableName;

private:


    static CACHE_STATUS   m_status;
    static QSqlDatabase m_db;
};



#endif // Common_Model_H
