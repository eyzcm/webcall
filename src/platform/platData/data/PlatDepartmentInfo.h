#pragma once
#include <QObject>
#include <QStringList>
#include "platdata_global.h"
#include "QMap"

class PLATDTA_EXPORT PlatDepartmentInfo : public QObject
{
	Q_OBJECT

public:
    PlatDepartmentInfo(void);
    PlatDepartmentInfo( PlatDepartmentInfo *info);
    ~PlatDepartmentInfo(void);
public:
	QString PlatDepartmentId() const { return m_PlatDepartmentId; }
	void PlatDepartmentId(QString val) { m_PlatDepartmentId = val; }


	
  
    
		QString Id() const;
		void Id(QString val);
 
		QString Name() const;
		void Name(QString val);
 
		int SiteId() const;
		void SiteId(int val);
 
		QString ParentId() const;
		void ParentId(QString val);
 
		int RelUserCount() const;
		void RelUserCount(int val);


    void LoadMapInfo(const QMap<QString, QString>& mapInfo);
	  QMap<QString, QString> InfoMap() const ;
private:
	QString m_PlatDepartmentId;
	
	  
    QString m_id; //部门
	 QString m_name; //名称
	 int m_siteId; //站点id
	 QString m_parentId; //父id
	 int m_relUserCount; //部门用户数
	
   
	
	
};
