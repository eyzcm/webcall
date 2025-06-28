
#include "Common_Model.h"
#include "service/util_service.h"
#include <qdebug>

QString Common_Model::m_tableName = "yl_commontable";
CACHE_STATUS   Common_Model::m_status = S_NONE;
QSqlDatabase Common_Model::m_db;


Common_Model::Common_Model()
{

}

void Common_Model::LoadDb( QSqlDatabase db )
{
    m_db = db;
    if( !m_db.open()) return ;
    m_status = S_OPEN;
    if( !IsTableExist() )
    {
        if( !CreateTable())
        {
            return ;
        }

    }


    m_status = S_CACHE_OK;
}

bool Common_Model::CreateTable(void)
{
    // 用于执行sql语句的对象
    QSqlQuery sqlQuery(m_db);
    // 构建创建数据库的sql语句字符串
    QString createSql = QString("CREATE TABLE "+m_tableName+" (\
                          name TEXT PRIMARY KEY NOT NULL,\
                          value TEXT ,\
                          itemsjson TEXT ,\
                          saveTime int,\
                          saveType int,\
                          updateTime DateTime)");

    sqlQuery.prepare(createSql);
    // 执行sql语句
    if(!sqlQuery.exec())
    {
        GERRORLOG( QString("create table %1 error: %2").arg(m_tableName).arg(sqlQuery.lastError().text()));
        return false;
    }
    else
    {
        return true;
    }
}

// 判断数据库中数据表是否存在
bool Common_Model::IsTableExist()
{
    QSqlDatabase database = m_db;
    if(database.tables().contains(m_tableName))
    {
        return true;
    }

    return false;
}


bool Common_Model::Insert()
{
    QSqlQuery sqlQuery(m_db);
    sqlQuery.prepare("INSERT INTO "+m_tableName+" VALUES(:name,:value,:itemsjson:saveTime,:saveType,datetime('now'))");
    sqlQuery.bindValue(":name", m_name);
    sqlQuery.bindValue(":value", m_version);
    sqlQuery.bindValue(":itemsjson", m_itemsjson);
    sqlQuery.bindValue(":saveTime", m_saveTime);
    sqlQuery.bindValue(":saveType", m_saveType);
    if(!sqlQuery.exec())
    {
        GERRORLOG( QString("insert data %1 error: %2").arg(m_tableName).arg(sqlQuery.lastError().text()));

        return false;
    }
    return true;
}


bool Common_Model::Update()
{
    if( !HasRecord()) return Insert();
    QSqlQuery sqlQuery(m_db);
    sqlQuery.prepare("UPDATE "+m_tableName+" SET value=?,itemsjson=?,saveTime=?,updateTime=datetime('now'),saveType=? WHERE name=?");
    
    sqlQuery.addBindValue( m_version);
    sqlQuery.addBindValue( m_itemsjson);
    sqlQuery.addBindValue( m_saveTime);
    sqlQuery.addBindValue( m_saveType);
	sqlQuery.addBindValue(m_name);

    if(!sqlQuery.exec())
    {
        GERRORLOG( QString("Error: Fail to update data %1.: %2").arg(m_tableName).arg(sqlQuery.lastError().text()));

        return false;
    }
    return true;

}


// 删除数据
bool Common_Model::DeleteData(QString name,QString version )
{
    QSqlQuery sqlQuery(m_db);

    sqlQuery.exec(QString("DELETE FROM "+m_tableName+" WHERE name = '%1' and version='%2'").arg(name).arg(version));
    if(!sqlQuery.exec())
    {

        GERRORLOG( QString("Error: Fail to  DELETE FROM %1 error: %2").arg(m_tableName).arg(sqlQuery.lastError().text()));
        return false;
    }
    else
    {
        return true;
    }
}

TableItem * Common_Model::QueryListByName(const QString name,const QString version)
{
	
	QSqlQuery sqlQuery(m_db);
    QString  sqlstr = QString("SELECT name,value,   saveTime ,saveType FROM %1  where   name=? ").arg(name);
    if( version.isEmpty() )
    {
        sqlstr += "order by saveTime desc";
    }else
    {
        sqlstr += "and version=?";
    }

    sqlQuery.prepare(sqlstr);
	
	//QString querystr = "SELECT * FROM " + m_tableName + " where 1 > 0";

	sqlQuery.addBindValue(name);
	
	if (!sqlQuery.exec())
	{
		GERRORLOG(QString("Error: Fail to query table. %1. %2").arg(m_tableName).arg(sqlQuery.lastError().text()));
		
		qDebug() << "Error: Fail to query table. " << m_tableName<<". " << sqlQuery.lastError().text();
	}
	else
	{

		while (sqlQuery.next())
		{
            TableItem * item = new TableItem();
			item->Name(sqlQuery.value(0).toString());
            item->Version(sqlQuery.value(1).toString());
            item->Itemsjson(sqlQuery.value(2).toString());
            item->SaveTime(sqlQuery.value(3).toInt());
			return item;


		}

	}
	return nullptr;

}

QList<TableItem *> Common_Model::QueryList(const QString where)
{
    QList<TableItem *> list;
    QSqlQuery sqlQuery(m_db);
    QString querystr = "SELECT * FROM "+m_tableName +" where 1=1 ";
    if( !where.isEmpty()) querystr += " and "+where;

    sqlQuery.exec(querystr);
    if(!sqlQuery.exec())
    {
        GERRORLOG( QString("Error: Fail to query %1. %2").arg(m_tableName).arg(sqlQuery.lastError().text()));


    }
    else
    {

        while(sqlQuery.next())
        {
            TableItem * item = new TableItem();
            item->Name( sqlQuery.value(0).toString());
            item->Version(sqlQuery.value(1).toString());
            item->Itemsjson(sqlQuery.value(2).toString());
            item->SaveTime(sqlQuery.value(3).toInt());
			list.append(item);

        }

    }
    return list;

}

bool Common_Model::HasRecord()
{
    QSqlQuery sqlQuery(m_db);
    QString querystr = QString("SELECT * FROM "+m_tableName +" where name = '%1'").arg(m_name);


    sqlQuery.exec(querystr);
    if(sqlQuery.exec())
    {
        if(sqlQuery.next()) return true;


    }
    return false;

}

bool Common_Model::DeleteAllMemData()
{
	{
		QSqlQuery sqlQuery(m_db);

		sqlQuery.exec(QString("DELETE FROM " + m_tableName + " WHERE  saveType= 0"));
		if (!sqlQuery.exec())
		{
            GERRORLOG( QString("Error: Fail to  deleted data %1. %2").arg(m_tableName).arg(sqlQuery.lastError().text()));

			
        }
	}
	{
		QSqlQuery sqlQuery(m_db);

		sqlQuery.exec(QString("DELETE FROM " + m_tableName + " WHERE  (strftime('%s','now') - strftime('%s',updateTime)) > saveTime"));
		if (!sqlQuery.exec())
		{
            GERRORLOG( QString("Error: Fail to  deleted data %1. %2").arg(m_tableName).arg(sqlQuery.lastError().text()));

			return false;
		}
		else
        {
			return true;
		}

	}
}

QList<TableItem *> Common_Model::QueryInitList()
{
    QList<TableItem *> list;
	QSqlQuery sqlQuery(m_db);
	QString querystr = "SELECT name,value,    strftime('%s',updateTime)+saveTime -  strftime('%s','now') as vtime ,saveType FROM " + m_tableName + " where saveTime>0 and saveType = 1 and vtime >0 ";
	//QString querystr = "SELECT * FROM " + m_tableName + " where 1 > 0";


	sqlQuery.exec(querystr);
	if (!sqlQuery.exec())
	{
        GERRORLOG( QString("Error: Fail to query table. %1. %2").arg(m_tableName).arg(sqlQuery.lastError().text()));


	}
	else
	{

		while (sqlQuery.next())
		{
            TableItem * item = new TableItem();
			item->Name(sqlQuery.value(0).toString());
            item->Version(sqlQuery.value(1).toString());
            item->Itemsjson(sqlQuery.value(2).toString());
            item->SaveTime(sqlQuery.value(3).toInt());
			list.append(item);


		}

	}
	return list;


}
