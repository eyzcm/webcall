#pragma once
#include <QObject>
#include <qstring>

class  IBaseProtocolAgent:public QObject
{
    Q_OBJECT
public:
    IBaseProtocolAgent(void);
    /*virtual void GetInitData(const QString &key, const QString &url)                                                                                                                          = 0;
    virtual void PostHttpOrderExtendData(const QString &url, const QString &body, const QString &vid, const QString &chatId)                                                                  = 0;
    virtual void PostHttpCardOrderExtendData(const QString &url, const QString &body, const QString &vid, const QString &staticId, const QString &chatId)                                     = 0;
    virtual void PostHttpExtendData(const QString &url, const QString &body, const QString &vid, const QString &chatId)                                                                       = 0;
    virtual void PostHttpsData(const QString &url, const QString &data, const QString &filePath, const QString &vid, const QString &chatId, const QString &username, const QString &password) = 0;
    virtual void PostHttpChatExtendData(const QString &url, const QString &body, const QString &vid, const QString &chatId)                                                                   = 0;
    virtual void PostHttpCardInfoExtendData(const QString &url, const QString &body, const QString &vid, const QString &chatId)                                                               = 0;
    virtual void DoGetSetting()                                                                                                                                                               = 0;
    virtual void DoGetOtherSetting(QString customId, QString commonkey)                                                                                                                       = 0;
    virtual void DoSaveSetting(const QString &body)                                                                                                                                           = 0;
    virtual void DoSaveOtherSetting(QString key, QString body)                                                                                                                                = 0;
    */

    ~IBaseProtocolAgent(void);
};
