#include "PlatUserInfo.h"

PlatUserInfo::PlatUserInfo(void)
{

}

PlatUserInfo::~PlatUserInfo(void)
{

}


PlatUserInfo::PlatUserInfo( PlatUserInfo *info)
{
	if(info)
	{
        Id(info->Id());
        UserId(info->UserId());
        DepartmentId(info->DepartmentId());
        UserName(info->UserName());
        PubName(info->PubName());
        BindPhone(info->BindPhone());
	}
	
}

  
    
 
QString PlatUserInfo::Id() const
{
    return m_id;
}

void PlatUserInfo::Id(QString val)
{ 
    m_id = val;
}

 
 
QString PlatUserInfo::UserId() const
{
	return m_userId; 
}

void PlatUserInfo::UserId(QString val)
{ 
	m_userId = val; 
}

 
 
QString PlatUserInfo::DepartmentId() const
{
	return m_departmentId; 
}

void PlatUserInfo::DepartmentId(QString val)
{ 
	m_departmentId = val; 
}

 
 
QString PlatUserInfo::UserName() const
{
	return m_userName; 
}

void PlatUserInfo::UserName(QString val)
{ 
	m_userName = val; 
}

 
 
QString PlatUserInfo::PubName() const
{
	return m_pubName; 
}

void PlatUserInfo::PubName(QString val)
{ 
	m_pubName = val; 
}

 
 
QString PlatUserInfo::BindPhone() const
{
	return m_bindPhone; 
}

void PlatUserInfo::BindPhone(QString val)
{ 
	m_bindPhone = val; 
}


   
   
void PlatUserInfo::LoadMapInfo(const QMap<QString, QString>& mapInfo)
{
	for (auto itor = mapInfo.begin(); itor != mapInfo.end();itor++)
	{
	  
    if( itor.key() == "id")
		{
            Id( itor.value());
		}else 
	  if( itor.key() == "userId")
		{
            UserId( itor.value());
		}else 
	  if( itor.key() == "departmentId")
		{
            DepartmentId( itor.value());
		}else 
	  if( itor.key() == "userName")
		{
            UserName( itor.value());
		}else 
	  if( itor.key() == "pubName")
		{
            PubName( itor.value());
		}else 
	  if( itor.key() == "bindPhone")
		{
            BindPhone( itor.value());
        }
    }
}

QMap<QString, QString>  PlatUserInfo::InfoMap() const
{
     QMap<QString, QString> mapInfo ;
     mapInfo.insert("string", QString("%1").arg(Id()));
     mapInfo.insert("userId", QString("%1").arg(UserId()));
     mapInfo.insert("departmentId", QString("%1").arg(DepartmentId()));
     mapInfo.insert("userName", QString("%1").arg(UserName()));
     mapInfo.insert("pubName", QString("%1").arg(PubName()));
     mapInfo.insert("bindPhone", QString("%1").arg(BindPhone()));
	 return mapInfo;
}

   
