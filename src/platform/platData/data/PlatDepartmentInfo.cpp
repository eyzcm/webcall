#include "PlatDepartmentInfo.h"

PlatDepartmentInfo::PlatDepartmentInfo(void)
{

}

PlatDepartmentInfo::~PlatDepartmentInfo(void)
{

}


PlatDepartmentInfo::PlatDepartmentInfo( PlatDepartmentInfo *info)
{
	if(info)
	{
		PlatDepartmentId(info->PlatDepartmentId());
		  
    
		Id(info->Id());
		 
		Name(info->Name());
		 
		SiteId(info->SiteId());
		 
		ParentId(info->ParentId());
		 
		RelUserCount(info->RelUserCount());
		
		

	}
	
}

  
    
 
QString PlatDepartmentInfo::Id() const
{
	return m_id; 
}

void PlatDepartmentInfo::Id(QString val)
{ 
	m_id = val; 
}

 
 
QString PlatDepartmentInfo::Name() const
{
	return m_name; 
}

void PlatDepartmentInfo::Name(QString val)
{ 
	m_name = val; 
}

 
 
int PlatDepartmentInfo::SiteId() const
{
	return m_siteId; 
}

void PlatDepartmentInfo::SiteId(int val)
{ 
	m_siteId = val; 
}

 
 
QString PlatDepartmentInfo::ParentId() const
{
	return m_parentId; 
}

void PlatDepartmentInfo::ParentId(QString val)
{ 
	m_parentId = val; 
}

 
 
int PlatDepartmentInfo::RelUserCount() const
{
	return m_relUserCount; 
}

void PlatDepartmentInfo::RelUserCount(int val)
{ 
	m_relUserCount = val; 
}


   
   
void PlatDepartmentInfo::LoadMapInfo(const QMap<QString, QString>& mapInfo)
{
	for (auto itor = mapInfo.begin(); itor != mapInfo.end();itor++)
	{
	  
    if( itor.key() == "id")
		{
            Id( itor.value());
		}else 
	  if( itor.key() == "name")
		{
            Name( itor.value());
		}else 
	  if( itor.key() == "siteId")
		{
            SiteId( itor.value().toInt());
		}else 
	  if( itor.key() == "parentId")
		{
            ParentId( itor.value());
		}else 
	  if( itor.key() == "relUserCount")
		{
			RelUserCount( itor.value().toInt());
        }
	}
}

QMap<QString, QString>  PlatDepartmentInfo::InfoMap() const
{
	 QMap<QString, QString> mapInfo ;
	 
	   
    
     mapInfo.insert("id", QString("%1").arg(Id()));
     mapInfo.insert("name", QString("%1").arg(Name()));
     mapInfo.insert("siteId", QString("%1").arg(SiteId()));
     mapInfo.insert("parentId", QString("%1").arg(ParentId()));
	 mapInfo.insert("relUserCount", QString("%1").arg(RelUserCount()));
	
	 
	 return mapInfo;
}

   
