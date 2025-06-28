#include "dbcacheservice.h"
#include "DatabaseFactory.h"
#include "plat_db_global.h"
#include "utils/UtilsHelper.h"
#include "LocalCache_Model.h"
#include "service/util_service.h"

DbCacheService* DbCacheService::m_instance = nullptr;


DbCacheService* DbCacheService::GetInstance()
{
    if (m_instance == NULL)
    {
        m_instance = new DbCacheService();
    }
    return m_instance;
}

DbCacheService::DbCacheService()
{

    m_init = 0;

}

DbCacheService::~DbCacheService()
{
    m_filedb.close();
    m_memdb.close();

}

void DbCacheService::Init( QString loginId )
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

		
		LocalCache_Model::LoadDb(m_filedb);
		LocalCache_Model::DeleteAllMemData( );
		QList<LocalCache *> list = LocalCache_Model::QueryInitList();
		foreach (LocalCache * item, list)
		{
			UpdateCacheTime(item->Name(), item->SaveTime(), 0 );

		}

        m_init = 1;
    }


}

void DbCacheService::SaveCacheData( const QString key,const QString value,int saveType ,int saveTime )
{
    LocalCache_Model model;
    LocalCache_Model::LoadDb(m_filedb);
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

QString DbCacheService::GetCacheData(const QString key )
{
    QString retvalue;
    LocalCache_Model model;
    LocalCache_Model::LoadDb(m_filedb);
    LocalCache * item = model.QueryListByName(key);
    if(item ) retvalue = item->Value() ;


    return retvalue;
}

void DbCacheService::UpdateCacheTime( QString key,int saveTime,int type )
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

void DbCacheService::R_CacheTimeout()
{
    Yl_SingleTimer * timer = (Yl_SingleTimer *)sender();
    if(m_cacheValue.contains( timer) )
    {
        //删除key
        QString key = m_cacheValue[timer];
        if( LocalCache_Model::DeleteData(key) )
            Q_EMIT s_removeCacheData(key);
    }
}


void DbCacheService::RemoveCacheData(const QString key)
{
    if( LocalCache_Model::DeleteData(key) )
        Q_EMIT s_removeCacheData(key);
}
