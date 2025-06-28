#ifndef PLATDBSERVICE_H
#define PLATDBSERVICE_H
#include <qobject.h>
#include "CacheDbInterface.h"
#include "plat_interface_global.h"

class PLAT_INTERFACE_EXPORT PlatDbService:public QObject
{

    Q_OBJECT
public:
    ~PlatDbService(){};
    static PlatDbService *GetInstance();


    CacheDbInterface * GetCacheDbServer( );
    CacheDbInterface * GetWebCacheDbServer( );


    void AddCacheDbServer(CacheDbInterface * service,int type = 0);

private:
    PlatDbService();
    class Garbo
    {
    public:
        ~Garbo();
    };
    static PlatDbService *m_instance;

    CacheDbInterface * m_cacheDb;
    CacheDbInterface * m_webcacheDb;



public:

};


#define GCACHEDBSERVICE PlatDbService::GetInstance()->GetCacheDbServer()
#define GWEBCACHEDBSERVICE PlatDbService::GetInstance()->GetWebCacheDbServer()

#endif // PLATDBSERVICE_H
