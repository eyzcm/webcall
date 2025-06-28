#ifndef HTTPPLATCACHEAGENT_H
#define HTTPPLATCACHEAGENT_H

#include <QObject>
#include "utils/UISingleton.h"

class HttpPlatCacheAgent : public QObject,public UISingleton<HttpPlatCacheAgent>
{

    Q_OBJECT

    friend class UISingleton<HttpPlatCacheAgent>;
public:
    explicit HttpPlatCacheAgent(QObject *parent = nullptr);


    void    LoadData();
    QString GetCacheDataByKey(QString key);
    void    SaveCacheDataByKey(QString key, QString value);

    QString DataColleagueVersion() const;
    void DataColleagueVersion(QString val);
    void GetColleagueList();


    QString ApiColleagueVersion() const;
    void    ApiColleagueVersion(QString val);

    QString BodyColleague() const;
    void    BodyColleague(QString val);

private slots:
    void R_GetColleagueList();

signals:


private:
    QString m_dataColleagueVersion;
    QString m_apiColleagueVersion;

    QString m_bodyColleague;

    int                    m_timecount;

#ifdef _TEST
public:
#else
private:
#endif
    QString m_userId;



};

#endif // HTTPPLATCACHEAGENT_H
