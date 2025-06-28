#include "stdafx.h"
#include "jswebdbcacheserver.h"
#include "db/platdbservice.h"
#include <utils/jsonhelper.h>

QMap<int,QString> g_ErrorMap{{PARAMERTER_ERROR, "parameter error！"}};

JsWebDbCacheServer::JsWebDbCacheServer()
{


}


QString  JsWebDbCacheServer::process(const QString & jsonstr)
{

    Json::Value val;
    bool isOk = JsonHelper::GetJsonValueFromBody(jsonstr, val);
    if( isOk )
    {
        QMap<QString, QString> tempMap;
        bool                   isParse = JsonHelper::ParseValueToMap(val, tempMap);
        if( isParse )
        {
            if((tempMap.find("key") == tempMap.end() )||(tempMap.find("cmd") == tempMap.end()))
            {
                return returnError( PARAMERTER_ERROR,g_ErrorMap[PARAMERTER_ERROR]);
            }
             if( tempMap["cmd"] == "set")
             {
                 QString value = (tempMap.find("value") != tempMap.end())?tempMap["value"]:"";
                 GWEBCACHEDBSERVICE->SaveCacheData(tempMap["key"], value,1,-1);
                 return returnOK("");


             }else if( tempMap["cmd"] == "get")
             {
               QString retdata = GWEBCACHEDBSERVICE->GetCacheData(tempMap["key"]);
               return returnOK(retdata);


             }else if( tempMap["cmd"] == "del")
             {
                 GWEBCACHEDBSERVICE->RemoveCacheData(tempMap["key"]);
                return returnOK("");
             }else if( tempMap["cmd"] == "delByFilter")
             {
                 GWEBCACHEDBSERVICE->RemoveCacheDataByFilter(tempMap["key"]);
                return returnOK("");
             }else if( tempMap["cmd"] == "clear")
             {
                 GWEBCACHEDBSERVICE->ClearCacheData();
                return returnOK("");
             }else if( tempMap["cmd"] == "filter")
             {
                 QMap<QString,QString> map = GWEBCACHEDBSERVICE->GetCacheListByFilter(tempMap["key"]);
				 
                return returnOK(JsonHelper::GetBodyByMap(map));

             }else
             {
                 return returnError( PARAMERTER_ERROR,g_ErrorMap[PARAMERTER_ERROR]);
             }

        }
    }
    return returnError( PARAMERTER_ERROR,g_ErrorMap[PARAMERTER_ERROR]);



}

QString JsWebDbCacheServer::returnError(int code ,const QString errormsg)
{
    Json::Value val;
    val["code"] = code;
    val["msg"] = errormsg.toStdString();
    return val.toStyledString().c_str();
}

QString JsWebDbCacheServer::returnOK( QString value )
{
    Json::Value val;
    val["code"] = 200;
    val["data"] = value.toStdString();
	return val.toStyledString().c_str();
}
