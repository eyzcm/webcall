#pragma once
#include <QObject>
#include <QStringList>
#include "platdata_global.h"
#include "QMap"

class PLATDTA_EXPORT PlatUserInfo : public QObject
{
	Q_OBJECT

public:
    PlatUserInfo(void);
    PlatUserInfo( PlatUserInfo *info);
    ~PlatUserInfo(void);
public:

    
        QString Id() const;
        void Id(QString val);
 
		QString UserId() const;
		void UserId(QString val);
 
		QString DepartmentId() const;
		void DepartmentId(QString val);
 
		QString UserName() const;
		void UserName(QString val);
 
		QString PubName() const;
		void PubName(QString val);
 
		QString BindPhone() const;
		void BindPhone(QString val);


    void LoadMapInfo(const QMap<QString, QString>& mapInfo);
	  QMap<QString, QString> InfoMap() const ;
private:
	QString m_PlatUserId;
	
	  
     QString m_id; //用户主id
	 QString m_userId; //用户id
	 QString m_departmentId; //部门id
	 QString m_userName; //对内名称
	 QString m_pubName; //对外名称
	 QString m_bindPhone; //手机号
	
   
	
	
};
