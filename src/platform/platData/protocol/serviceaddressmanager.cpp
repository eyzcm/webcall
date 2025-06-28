#include "serviceaddressmanager.h"
#include "utils/utilshelper.h"
#include <QDateTime>
#include <QFileInfo>
#include <QStringList>
#include <data/UILoginData.h>
#include <QDir>

ServiceAddressManager::ServiceAddressManager() : QObject()
{
}

ServiceAddressManager* ServiceAddressManager::m_instance = NULL;

ServiceAddressManager::~ServiceAddressManager()
{
}

ServiceAddressManager* ServiceAddressManager::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new ServiceAddressManager;
    }
    return m_instance;
}

void ServiceAddressManager::Init(QString  uid)
{
	

    {
		m_serviceList.clear();
		m_monitorServiceList.clear();

        QString      userid    = "";

        if (!uid.isEmpty())
        {
            userid = "/" + uid;
        }
		m_uid = uid;

        QString   serviceFileName   = QString("%1/%2").arg(UtilsHelper::GetAppdataCacheDir() + userid).arg("Service.conf");
        QString   systemFileName    = QString("%1/%2").arg(UtilsHelper::GetRunAppDir() + userid).arg("system.conf");
        QString   wxserviceFileName = QString("%1/%2").arg(UtilsHelper::GetAppdataCacheDir() + userid).arg("wxService.conf");
        QFileInfo serviceInfo(serviceFileName);
        QFileInfo systemInfo(systemFileName);
        QFileInfo wxserviceInfo(wxserviceFileName);

        QDateTime serviceLastModified   = serviceInfo.lastModified();
        QDateTime systemLastModified    = systemInfo.lastModified();
        QDateTime wxserviceLastModified = wxserviceInfo.lastModified();

        if (serviceLastModified > systemLastModified)
        {
            LoadServiceListFile(0);
            LoadServiceListFile(1);
        }
        if (wxserviceLastModified > systemLastModified)
        {
            LoadServiceListFile(2);
        }
    }
}

void ServiceAddressManager::Clear()
{
    ClearAddressInfoList(true);
    ClearAddressInfoList(false);
}

void ServiceAddressManager::ResetServiceList(int isMonitor)
{
    if (isMonitor == 1)
    {
        int max = m_monitorAddressInfolist.count();
        for (int i = 0; i < max; i++)
        {
            m_monitorAddressInfolist[i]->m_isUser    = false;
            m_monitorAddressInfolist[i]->m_randomNum = UtilsHelper::GetRandNumber(9999);
        }
    }
    else if (isMonitor == 0)
    {
        int max = m_addressInfolist.count();
        for (int i = 0; i < max; i++)
        {
            m_addressInfolist[i]->m_isUser    = false;
            m_addressInfolist[i]->m_randomNum = UtilsHelper::GetRandNumber(9999);
        }
    }
    else if (isMonitor == 2)
    {
    }
}

void ServiceAddressManager::SetServiceList(QString strServiceList, int isMonitor)
{
    if (strServiceList.isEmpty())
    {
        return;
    }
    if (isMonitor == 1)
    {
        m_monitorServiceList = strServiceList;
    }
    else if (isMonitor == 0)
    {
        m_serviceList = strServiceList;
    }
    else if (isMonitor == 2)
    {
        m_wxserviceList = strServiceList;
    }
    ParseServiceList(strServiceList, isMonitor);
    SaveServiceListFile(strServiceList, isMonitor);
}

addressinfo* ServiceAddressManager::GetAddressInfo()
{
    for (int i = 0; i < m_addressInfolist.count(); i++)
    {
        if (m_addressInfolist[i] && !m_addressInfolist[i]->m_isUser)
        {
            bool isFind = true;
            for (int j = i + 1; j < m_addressInfolist.count(); j++)
            {
                if (m_addressInfolist[j] && !m_addressInfolist[j]->m_isUser)
                {
                    if (m_addressInfolist[i]->m_randomNum > m_addressInfolist[j]->m_randomNum)
                    {
                        isFind = false;
                        break;
                    }
                }
            }
            if (isFind)
            {
                m_addressInfolist[i]->m_isUser = true;
                return m_addressInfolist[i];
            }
        }
    }

    return NULL;
}

addressinfo* ServiceAddressManager::GetMonitorAddressInfo()
{
    for (int i = 0; i < m_monitorAddressInfolist.count(); i++)
    {
        if (m_monitorAddressInfolist[i] && !m_monitorAddressInfolist[i]->m_isUser)
        {
            bool isFind = true;
            for (int j = i + 1; j < m_monitorAddressInfolist.count(); j++)
            {
                if (m_monitorAddressInfolist[j] && !m_monitorAddressInfolist[j]->m_isUser)
                {
                    if (m_monitorAddressInfolist[i]->m_randomNum > m_monitorAddressInfolist[j]->m_randomNum)
                    {
                        isFind = false;
                        break;
                    }
                }
            }
            if (isFind)
            {
                m_monitorAddressInfolist[i]->m_isUser = true;
                return m_monitorAddressInfolist[i];
            }
        }
    }

    return NULL;
}

void ServiceAddressManager::ParseServiceList(QString strServiceList, int isMonitor)
{
    if (strServiceList.isEmpty())
    {
        return;
    }
    ClearAddressInfoList(isMonitor);
    //解析serverList 串。
    // 120.27.44.236:1890
    if ((isMonitor == 0) || (isMonitor == 1))
    {
        QStringList list = strServiceList.split(";");
        Q_FOREACH (QString item, list)
        {
            if (!item.isEmpty())
            {
                QStringList itemList = item.split(":");
                if (itemList.count() >= 2)
                {
                    AddAddressInfo(itemList[0], itemList[1], isMonitor);
                }
            }
        }
    }
    else if (isMonitor == 2)
    {
        m_wxserviceList = strServiceList;
    }
    ResetServiceList(isMonitor);
}

void ServiceAddressManager::SaveServiceListFile(QString strServicelist, int isMonitor)
{
	QString      userid = "";
	
	if ( !m_uid.isEmpty() )
	{
		userid = "/" + m_uid;
	}

	QString dirName = UtilsHelper::GetAppdataCacheDir() + userid;
	QDir    dir;
	if (!dir.exists(dirName))
	{
		dir.mkdir(dirName);
	}
	QString fileName = "";
	if (isMonitor == 1)
	{
		fileName = QString("%1/%2").arg(dirName, "MonitorService.conf");
	}
	else if (isMonitor == 0)
	{
		fileName = QString("%1/%2").arg(dirName, "Service.conf");
	}
	else if (isMonitor == 2)
	{
		fileName = QString("%1/%2").arg(dirName, "wxService.conf");
	}
	UtilsHelper::SaveFile(fileName, strServicelist);
}

QString ServiceAddressManager::GetWxServerAddress()
{
    return m_wxserviceList;
}

void ServiceAddressManager::LoadServiceListFile(int isMonitor)
{
	QString      userid = "";

	if (!m_uid.isEmpty())
	{
		userid = "/" + m_uid;
	}

    QString fileName = "";
    if (isMonitor == 1)
    {
        fileName             = QString("%1/%2").arg(UtilsHelper::GetAppdataCacheDir()+ userid).arg("MonitorService.conf");
        m_monitorServiceList = UtilsHelper::LoadFile(fileName);
        ParseServiceList(m_monitorServiceList, isMonitor);
    }
    else if (isMonitor == 0)
    {
        fileName      = QString("%1/%2").arg(UtilsHelper::GetAppdataCacheDir()+ userid).arg("Service.conf");
        m_serviceList = UtilsHelper::LoadFile(fileName);
        ParseServiceList(m_serviceList, isMonitor);
    }
    else if (isMonitor == 2)
    {
        fileName        = QString("%1/%2").arg(UtilsHelper::GetAppdataCacheDir()+ userid).arg("wxService.conf");
        m_wxserviceList = UtilsHelper::LoadFile(fileName);
        ParseServiceList(m_wxserviceList, isMonitor);
    }
}

void ServiceAddressManager::AddAddressInfo(QString strHost, QString strPort, int isMonitor)
{
    addressinfo* info = new addressinfo;
    info->m_host      = strHost;
    info->m_port      = strPort;
    if (isMonitor == 1)
    {
        m_monitorAddressInfolist.push_back(info);
    }
    else if (isMonitor == 0)
    {
        m_addressInfolist.push_back(info);
    }
    else if (isMonitor == 2)
    {
        ;
    }
}

void ServiceAddressManager::ClearAddressInfoList(int isMonitor)
{
    if (isMonitor == 1)
    {
        Q_FOREACH (addressinfo* info, m_monitorAddressInfolist)
        {
            if (info)
            {
                delete info;
                info = NULL;
            }
        }
        m_monitorAddressInfolist.clear();
    }
    else if (isMonitor == 0)
    {
        Q_FOREACH (addressinfo* info, m_addressInfolist)
        {
            if (info)
            {
                delete info;
                info = NULL;
            }
        }
        m_addressInfolist.clear();
    }
    else if (isMonitor == 2)
    {
        m_wxserviceList = "";
    }
}
