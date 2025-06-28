#include "logincmd.h"

#include "protocol/serviceaddressmanager.h"

#include <utils/UtilsHelper.h>


LoginCmd::LoginCmd() : TcpCmd()
{
    m_userName     = "";
    m_passWord     = "";
    m_loginUrl     = "";
    m_port         = "";
    m_loginStatus  = E_ONLINE;
    m_isFriendData = 0;
    m_force = false;
}

LoginCmd::~LoginCmd()
{
}

void LoginCmd::Init(QString userName, QString password, QString loginUrl, QString port)
{
    m_userName = userName;
    m_passWord = password;
    m_loginUrl = loginUrl;
    m_port     = port;
}

void LoginCmd::MakeCmd()
{
    Body("OK");

    QString strUriFormat = "";
    if (m_loginType == 0)
        strUriFormat = "/Auth?password=%1&user=%2&receipt=1&s=%3&uf=%4";
    if (m_loginType == 1)
        strUriFormat = "/Auth?token=%1&user=%2&receipt=1&s=%3&uf=%4";
    if( m_force )
            strUriFormat.append("&forceType=1");
    if( !m_version.isEmpty() ) strUriFormat += "&version="+m_version;

    QString strTempUri = strUriFormat.arg(m_passWord).arg(m_userName).arg(m_loginStatus).arg(m_isFriendData);

    Uri(strTempUri);

    AddHeader("clientIp", UtilsHelper::GetClientIp());
    AddHeader("clientMac", UtilsHelper::GetClientMac());

}

void LoginCmd::PreProcessMsg(TcpMessagePtr msg)
{
    if (msg == nullptr)
    {
        Q_EMIT S_Finished(msg);
        return;
    }

    if (msg->GetHeaderByName("Error") == "301")
    {
        if (msg->Uri() != "/Auth")
        {
            QString serverList         = msg->GetHeaderByName("Servers");
            QString monitor_serverList = msg->GetHeaderByName("Monitor-Servers");
            ServiceAddressManager::GetInstance()->Clear();
            ServiceAddressManager::GetInstance()->SetServiceList(serverList, 0);
            ServiceAddressManager::GetInstance()->SetServiceList(monitor_serverList, 1);


            if (msg)
            {
                msg->deleteLater();
                msg = nullptr;
            }
        }
        else
        {
            Q_EMIT S_Finished(msg);
        }
    }
    else
    {
        Q_EMIT S_Finished(msg);
    }
}

void LoginCmd::R_connected()
{
    SendRequestToServer();
}

void LoginCmd::LoginStatus(EPRESENCESTATUS status)
{
    m_loginStatus = status;
}

EPRESENCESTATUS LoginCmd::LoginStatus()
{
    return m_loginStatus;
}

void LoginCmd::IsFriendData(int friendType)
{
    m_isFriendData = friendType;
}

int LoginCmd::IsFriendData()
{
    return m_isFriendData;
}

void LoginCmd::LoginType(int type)
{
    m_loginType = type;
}

void LoginCmd::IsForce( bool forcelogin)
{
    m_force = forcelogin;
}
