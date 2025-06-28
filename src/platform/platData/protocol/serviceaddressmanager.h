#ifndef SERVICEADDRESSMANAGER_H
#define SERVICEADDRESSMANAGER_H


#include <QObject>
#include "platdata_global.h"
class UILoginData;
typedef struct S_ADDRESSINFO
{
    QString m_host;
    QString m_port;
    bool    m_isUser;
    int     m_randomNum;
} addressinfo;

class PLATDTA_EXPORT ServiceAddressManager : public QObject
{
    Q_OBJECT

public:
    ~ServiceAddressManager();
    static ServiceAddressManager* GetInstance();

private:
    ServiceAddressManager();
    static ServiceAddressManager* m_instance;

public:
    void Init(QString  uid);
    void Clear();

    void         ResetServiceList(int isMonitor);
    void         SetServiceList(QString strServiceList, int isMonitor);
    addressinfo* GetAddressInfo();
    addressinfo* GetMonitorAddressInfo();
    QString      GetWxServerAddress();

    void ClearAddressInfoList(int isMonitor);

    void AddAddressInfo(QString strHost, QString prot, int isMonitor);

private:
    void ParseServiceList(QString strServiceList, int isMonitor);
    void SaveServiceListFile(QString strServicelist, int isMonitor);
    void LoadServiceListFile(int isMonitor);

private:
    QString             m_serviceList;
    QString             m_wxserviceList;
    QList<addressinfo*> m_addressInfolist;

    QString             m_monitorServiceList;
    QList<addressinfo*> m_monitorAddressInfolist;
	QString			    m_uid;
};

#endif // SERVICEADDRESSMANAGER_H
