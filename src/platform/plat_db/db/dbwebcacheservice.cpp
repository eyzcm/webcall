#include "dbwebcacheservice.h"

#include "DatabaseFactory.h"
#include "plat_db_global.h"
#include "utils/UtilsHelper.h"
#include "LocalWebCache_Model.h"
#include "service/util_service.h"

DbWebCacheService* DbWebCacheService::m_instance = nullptr;


DbWebCacheService* DbWebCacheService::GetInstance()
{
    if (m_instance == NULL)
    {
        m_instance = new DbWebCacheService();
    }
    return m_instance;
}

DbWebCacheService::DbWebCacheService()
{

    m_init = 0;

}

DbWebCacheService::~DbWebCacheService()
{
    m_filedb.close();
    m_memdb.close();

}

void DbWebCacheService::Init( QString loginId )
{
    if( m_init == 0 )
    {
        m_memdb = DatabaseFactory::getInstance()->getDatabase(MEMDB);
        if( !m_memdb.open())
        {
            //出错
            qDebug() << "open memory error:";
        }
        QString path = UtilsHelper::GetCacheDir(loginId);
        path += "\\persondb.db";
        m_filedb = DatabaseFactory::getInstance()->config(DatabaseFactory::DBSetting("QSQLITE", path),"persondb")->getDatabase("persondb");
        if( !m_filedb.open())
        {
            //出错
            GERRORLOG(QString("open database  %1 error: %2").arg(path));

        }


        LocalWebCache_Model::LoadDb(m_filedb);
        LocalWebCache_Model::DeleteAllMemData( );
        QList<LocalCache *> list = LocalWebCache_Model::QueryInitList();
        foreach (LocalCache * item, list)
        {
            UpdateCacheTime(item->Name(), item->SaveTime(), 0 );

        }

        m_init = 1;
    }


}

void DbWebCacheService::SaveCacheData( const QString key,const QString value,int saveType ,int saveTime )
{
    LocalWebCache_Model model;
    LocalWebCache_Model::LoadDb(m_filedb);
    bool ret = false;
    int  updateType = 0;
    model.Name(key);
    model.Value(value);
    model.SaveTime(saveTime);

    if( !model.HasRecord())
    {
        ret = model.Insert();
        updateType = 0;
    }else
    {
        ret = model.Update();
        updateType = 1;
    }

    if (ret)
    {
        if( saveTime > 0  )
        {
            UpdateCacheTime(  key, saveTime,updateType );
        }
        Q_EMIT s_updateCacheData(key, value);
    }


}

QString DbWebCacheService::GetCacheData(const QString key )
{
    QString retvalue;
    LocalWebCache_Model model;
    LocalWebCache_Model::LoadDb(m_filedb);
    LocalCache * item = model.QueryListByName(key);
    if(item ) retvalue = item->Value() ;


    return retvalue;
}

void DbWebCacheService::UpdateCacheTime( QString key,int saveTime,int type )
{
    if( type == 0 )
    {
        Yl_SingleTimer * timer = new Yl_SingleTimer();
        timer->setSingleShot(true);
        timer->setInterval(saveTime*1000  );
        m_cacheValue.insert(timer,key);
        connect(timer, SIGNAL(timeout()), this, SLOT(R_CacheTimeout()));

        timer->start();
    }else
    {
        for (auto itor = m_cacheValue.begin(); itor != m_cacheValue.end(); itor++)
        {
            if( itor.value() == key )
            {
                Yl_SingleTimer * timer = itor.key();
                timer->stop();
                timer->setSingleShot(true);
                timer->setInterval(saveTime*1000  );
                timer->start();

            }

        }



    }

}

void DbWebCacheService::R_CacheTimeout()
{
    Yl_SingleTimer * timer = (Yl_SingleTimer *)sender();
    if(m_cacheValue.contains( timer) )
    {
        //删除key
        QString key = m_cacheValue[timer];
        if( LocalWebCache_Model::DeleteData(key) )
            Q_EMIT s_removeCacheData(key);
    }
}


void DbWebCacheService::RemoveCacheData(const QString key)
{
    if( LocalWebCache_Model::DeleteData(key) )
        Q_EMIT s_removeCacheData(key);
}


void DbWebCacheService::RemoveCacheDataByFilter(const QString key)
{
    if( LocalWebCache_Model::DeleteDataByFiter(key) )
        Q_EMIT s_removeCacheData(key);
}


void DbWebCacheService::ClearCacheData()
{
     LocalWebCache_Model::ClearData();
}

QMap<QString,QString>  DbWebCacheService::GetCacheListByFilter(const QString prekey)
{
    QMap<QString,QString> map;
    LocalWebCache_Model model;
     QList<LocalCache *> list =  model.QueryList( QString("name like '%1'").arg(prekey) );
     for( LocalCache * info : list )
     {
         if( info )
         {
             map.insert(info->Name(),info->Value());
         }

     }
     return map;

}


