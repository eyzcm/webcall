#ifndef USERCENTERHTTPAGENT_H
#define USERCENTERHTTPAGENT_H

#include <QObject>
#include "plat_permission_global.h"

class PLAT_PERMISSION_EXPORT UserCenterHttpAgent : public QObject
{
    Q_OBJECT
public:


    static UserCenterHttpAgent* GetInstance();
    virtual ~UserCenterHttpAgent();

    UserCenterHttpAgent();

    void Init(QString url,QString token );

    void GetProductList();
    void GetMenuList();
    void GetDeptList();
    void GetUserList(int pageNo=1,int pageSize = 100);

private slots:
    void R_GetProductListFinished(QString code, QString reason, QString body );
    void R_GetMenuListFinished(QString code, QString reason, QString body);
    void R_GetDeptListFinished(QString code, QString reason, QString body);
    void R_GetUserListFinished(QString code, QString reason, QString body);

private:
    QString GetAiUrl(QString url);
    QString MakeUri(QString url, QString cmdTag);
    QString MakeClientUri(QString url, QString cmdTag);


signals:

private:
    static UserCenterHttpAgent* m_instance;
    QString m_baseUrl;
	QString m_token;

    int m_currentUserPage ;

};


#define gUserCenterHttpAgent UserCenterHttpAgent::GetInstance()
#endif // USERCENTERHTTPAGENT_H
