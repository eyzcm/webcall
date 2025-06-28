#include "plat_db.h"
#include "db/databasefactory.h"
#include "db/dbcacheservice.h"
#include "db/platdbservice.h"
#include "db/dbwebcacheservice.h"



typedef void(*DLL_INIT_APP)();

Plat_db::Plat_db()
{
}


Plat_db * gInterface = new Plat_db();

DllLoadInterface * GetDllInterface()
{

    return gInterface;

}
DllLoadInterface * plat_db_lib_GetDllInterface()
{

    return gInterface;

}



QString Plat_db::GetModuleName()
{
    return "Plat_db";
};

void Plat_db::Init( int tcount ,  char * argv[]  )
{

    DatabaseFactory::getInstance()->config(DatabaseFactory::DBSetting("QSQLITE", ":memory:"),MEMDB);
    PlatDbService::GetInstance()->AddCacheDbServer(DbCacheService::GetInstance(),0);
    PlatDbService::GetInstance()->AddCacheDbServer(DbWebCacheService::GetInstance(),1);

}

bool Plat_db::CheckVersion( QString version )
{
    return false;
}
