#ifndef DBUICACHEINTERFACE_H
#define DBUICACHEINTERFACE_H


#include "plat_interface_global.h"
#include <QMutex>
#include <QObject>
#include "service/DbUiBaseService.h"
class ConstructionData;
class UIVisitorDispname;
class UIChatUserInfo;
class UIChatInfo;
class UIWaitVisitorInfo;
class UIVisitorInfo;
class UICardInfo;

class PLAT_INTERFACE_EXPORT DbUiCacheInterface : public QObject,public DbUiBaseService
{
    Q_OBJECT

public:
    DbUiCacheInterface(DbUiService *mainService){ };
    DbUiCacheInterface(){ };
    virtual ~DbUiCacheInterface(void){ };

      virtual QString      ServiceId( ){return "DbUiCacheInterface";};
     virtual UIVisitorDispname *CreateVisitorDispname(){ return nullptr;};

     virtual int GetVisitorDispnameMap(const QString &datatype, ConstructionData *factory, void *outlist){ return 0;};

     virtual void ClearVisitorDispnameInfo(){ };

     virtual int AddVisitorDispname(UIVisitorDispname *info){ return 0;};

     virtual void *findDispnameById(QString id, const QString &datatype, ConstructionData *factory){ return nullptr;};

     virtual void FreeVisitorDispname(UIVisitorDispname *pobj){ };

     virtual QString GetDisplayName(const UIChatUserInfo *info){ return "";};
     virtual QString GetDisplayName(const QString &uid){ return "";};
     virtual QString GetDisplayName(const UIChatInfo *info){ return "";};
     virtual QString GetDisplayName(const UIVisitorInfo *info){return ""; };
     virtual QString GetDisplayName(const UIWaitVisitorInfo *info){ return "";};

     virtual void UpdataCardInfo(UICardInfo *info){ };

    /**
     * @brief SaveCacheData 保存缓存数据
     * @param key 关键词
     * @param value  值
     * @param saveType  保存方式 0表示内存保存，重启后会丢失 1.在本地文件数据库里保存，重新登录后值还会存在
     * @param saveTime  保存时间，以秒为单位 -1表示永久，大于0表示保存时间长度，到时间会收到s_removeCacheData消息
     */
    virtual void SaveCacheData( const QString key,const QString value,int saveType = 0,int saveTime = -1 ){ };

    /**
     * @brief GetCacheData 获取缓存数据
     * @param key   关键词
     * @return  返回值
     */
    virtual  QString GetCacheData(const QString key) {return "";};

    /**
     * @brief RemoveCacheData 删除缓存数据
     * @param key  关键词
     */
    virtual void RemoveCacheData(const QString key){};

signals:
    /**
     * @brief s_removeCacheData  信号函数，时间到期会发出此信号
     * @param key   关键词
     */
    void s_removeCacheData(const QString key );
    /**
     * @brief s_updateCacheData 数据增加或变更增加此信号函数，时间到期会发出此信号
     * @param key     关键词
     * @param value   值
     */
    void s_updateCacheData(const QString key, const QString value);

private:
   
    
};

#endif //
