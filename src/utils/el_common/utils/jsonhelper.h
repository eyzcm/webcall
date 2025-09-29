#ifndef JSONHELPER_H
#define JSONHELPER_H

#include <QMap>
#include <QObject>

#include "include/json/value.h"

class JsonHelper : public QObject
{
    Q_OBJECT

public:
    JsonHelper();
    ~JsonHelper();
    static void    Read();
    static void    Write();
    static QString ChangeToHump(QString key);
    static QString GetValueByKeyHump(QString key, const Json::Value &val);
    static QString GetValueByKey(QString key, const Json::Value &val);
    static bool    GetJsonValueFromBody(const QString &body, Json::Value &val);
    static bool    JsonValueIsArray(const Json::Value &val);
    static bool    ParseValueToMap(const Json::Value &val, QMap<QString, QString> &retMap);
    static bool    ParseValueToMapEx(const Json::Value &val, QMap<QString, QString> &retMap);
    static bool    ParseValueToMapByString(QString val, QMap<QString, QString> &retMap);
    static QString GetBodyByMap(const QMap<QString, QString> srcMap);

};

#endif // JSONHELPER_H
