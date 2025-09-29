#include "jsonhelper.h"

#include "DefineHelper.h"
#include "include/json/reader.h"

JsonHelper::JsonHelper() : QObject()
{
}

JsonHelper::~JsonHelper()
{
}

void JsonHelper::Read()
{
    std::string strValue = "{\"key1\":\"value1\",\"key2\":\"valuesfd\",\"key3\":\"fdg\",\"array\":[{\"key2\":\"value2\"},{\"key2\":\"value3\"},{\"key2\":\"value4\"}]}";

    Json::Reader reader;
    Json::Value  value;

    if (reader.parse(strValue, value))
    {
        QMap<QString, QString> mapTemp;
        ParseValueToMap(value, mapTemp);
        for (Json::ValueIterator itor = value.begin(); itor != value.end(); itor++)
        {
            std::string key  = itor.key().asString();
            QString     temp = QString::fromStdString(key);


            QString tempname = QString::fromStdString(itor.memberName());

        }

        std::string out  = value["key1"].asString();
        QString     temp = QString::fromStdString(out);


        const Json::Value arrayObj = value["array"];

        for (int i = 0; i < arrayObj.size(); i++)
        {
            out = arrayObj[i]["key2"].asString();

        }
    }
}

void JsonHelper::Write()
{
    Json::Value root;
    Json::Value arrayObj;
    Json::Value item;
    for (int i = 0; i < 10; i++)
    {
        item["key"] = i;
        arrayObj.append(item);
    }

    root["key1"]  = "value1";
    root["key2"]  = "value2";
    root["array"] = arrayObj;
    root.toStyledString();
    std::string out = root.toStyledString();


}

QString JsonHelper::ChangeToHump(QString key)
{
    QString newKey = key;
    if (newKey.indexOf("_"))
    {
        QString tempKey;
        int     fromindex = 0;
        int     clen      = newKey.length();

        for (int i = 0, j = 0; i < clen; i++)
        {
            QChar c = newKey[i];
            if (c == '_')
            {
                if (i < (clen - 1))
                {
                    c = newKey[++i];
                    if ((c >= 'a') && (c <= 'z'))
                    {
                        c            = c.toLatin1() - ('a' - 'A');
                        tempKey[j++] = c;
                    }
                }
                else
                    tempKey[j++] = c;
            }
            else
            {
                tempKey[j++] = c;
            }
        }

        newKey = tempKey;
    }
    return newKey;
}

QString JsonHelper::GetValueByKeyHump(QString key, const Json::Value &val)
{
    QString newKey = JsonHelper::ChangeToHump(key);
    if (val[newKey.toStdString().c_str()] == Json::nullValue)
        newKey = key;
    return JsonHelper::GetValueByKey(newKey, val);
}


QString JsonHelper::GetValueByKey(QString key, const Json::Value &val)
{
    QString ret = "";
    if (val[key.toStdString().c_str()] != Json::nullValue)
    {
        switch (val[key.toStdString().c_str()].type())
        {
            case Json::intValue:
                ret = QString::number(val[key.toStdString().c_str()].asInt());
                break;
            case Json::uintValue:
                ret = QString::number(val[key.toStdString().c_str()].asUInt());
                break;
            case Json::realValue:
                ret = QString::number(val[key.toStdString().c_str()].asDouble());
                break;
            case Json::ulongValue:
            {
                ULONG64    vald = val[key.toStdString().c_str()].asUlong();
                qulonglong qval = vald;
                ret             = QString::number(qval);
            }
            break;
            case Json::stringValue:
                ret = QString::fromUtf8(val[key.toStdString().c_str()].asString().c_str());
                break;
            case Json::booleanValue:
                if (val[key.toStdString().c_str()].asBool())
                {
                    ret = "1";
                }
                else
                {
                    ret = "0";
                }
                break;
            default:
                Q_ASSERT(false);
                break;
        }
    }
    return ret;
}

bool JsonHelper::GetJsonValueFromBody(const QString &body, Json::Value &val)
{
    Json::Reader reader;
    std::string  bodyStr = body.toUtf8().data();
    // std::string bodyStr = body.toStdString();
    if (reader.parse(bodyStr, val))
    {
        return true;
    }

    return false;
}

bool JsonHelper::JsonValueIsArray(const Json::Value &val)
{
    if (val.isArray())
    {
        return true;
    }
    return false;
}

bool JsonHelper::ParseValueToMapEx(const Json::Value &val, QMap<QString, QString> &retMap)
{
    for (Json::ValueIterator itor = val.begin(); itor != val.end(); itor++)
    {
        QString key      = QString::fromStdString(itor.memberName());
        QString valRet   = "";
        bool    isInsert = true;
        switch (val[itor.memberName()].type())
        {
            case Json::intValue:
                valRet = QString::number(val[itor.memberName()].asInt());
                break;
            case Json::uintValue:
                valRet = QString::number(val[itor.memberName()].asUInt());
                break;
            case Json::realValue:
                valRet = QString::number(val[itor.memberName()].asDouble());
                break;
            case Json::ulongValue:
            {
                ULONG64    vald = val[itor.memberName()].asUlong();
                qulonglong qval = vald;
                valRet          = QString::number(qval);
            }
            break;
            case Json::stringValue:
                valRet = QString::fromUtf8(val[itor.memberName()].asString().c_str());
                break;
            case Json::booleanValue:
                if (val[itor.memberName()].asBool())
                {
                    valRet = "1";
                }
                else
                {
                    valRet = "0";
                }
                break;
            case Json::objectValue:
            case Json::arrayValue:
                valRet = QString::fromUtf8(val[itor.memberName()].toStyledString().c_str());

                break;
            default:
                isInsert = false;
                break;
        }
        if (isInsert)
        {
            retMap.insert(key, valRet);
        }
    }
    return true;
}

bool JsonHelper::ParseValueToMap(const Json::Value &val, QMap<QString, QString> &retMap)
{
    for (Json::ValueIterator itor = val.begin(); itor != val.end(); itor++)
    {
        QString key      = QString::fromStdString(itor.memberName());
        QString valRet   = "";
        bool    isInsert = true;
        switch (val[itor.memberName()].type())
        {
            case Json::intValue:
                valRet = QString::number(val[itor.memberName()].asInt());
                break;
            case Json::uintValue:
                valRet = QString::number(val[itor.memberName()].asUInt());
                break;
            case Json::realValue:
                valRet = QString::number(val[itor.memberName()].asDouble());
                break;
            case Json::ulongValue:
            {
                ULONG64    vald = val[itor.memberName()].asUlong();
                qulonglong qval = vald;
                valRet          = QString::number(qval);
            }
            break;
            case Json::stringValue:
                valRet = QString::fromUtf8(val[itor.memberName()].asString().c_str());
                break;
            case Json::booleanValue:
                if (val[itor.memberName()].asBool())
                {
                    valRet = "1";
                }
                else
                {
                    valRet = "0";
                }
                break;
            case Json::objectValue:
            {
            }
            break;
            default:
                isInsert = false;
                break;
        }
        if (isInsert)
        {
            retMap.insert(key, valRet);
        }
    }
    return true;
}

bool JsonHelper::ParseValueToMapByString(QString val, QMap<QString, QString> &retMap)
{
    bool        isRet = false;
    Json::Value value;
    bool        isOk = JsonHelper::GetJsonValueFromBody(val, value);
    if (isOk)
    {
        bool isParse = JsonHelper::ParseValueToMap(value, retMap);
        isRet        = isParse;
    }
    return isRet;
}


QString JsonHelper::GetBodyByMap(const QMap<QString, QString> srcMap)
{
    Json::Value            val;

    for (auto iter = srcMap.begin(); iter != srcMap.end(); iter++)
    {
        val[iter.key().toStdString().c_str()] = iter.value().toStdString();
    }

    std::string out = val.toStyledString();
    return QString::fromStdString( out );

}
