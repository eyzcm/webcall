#ifndef CACHEDBINTERFACE_H
#define CACHEDBINTERFACE_H
#include <QObject>
#include <QString>
#include <QMap>
#include "utils/Plat_EnumDefine.h"
#include "plat_interface_global.h"

class PLAT_INTERFACE_EXPORT CacheDbInterface : public QObject
{
    Q_OBJECT
public:
    CacheDbInterface();
    virtual void Init( QString loginId ){};
    virtual void SaveCacheData( const QString key,const QString value,int saveType = 0,int saveTime = -1 ){ };
    virtual  QString GetCacheData(const QString key) {return "";};
	virtual  void RemoveCacheData(const QString key) {  };
    virtual  void ClearCacheData() {  };
    virtual  QMap<QString,QString>  GetCacheListByFilter(const QString prekey) {return QMap<QString,QString>();};
    virtual void RemoveCacheDataByFilter(const QString key){};
	

signals:
    void s_removeCacheData(const QString key );
    void s_updateCacheData(const QString key, const QString value);
};

#endif // CACHEDBINTERFACE_H
