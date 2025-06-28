#ifndef JSWEBDBCACHESERVER_H
#define JSWEBDBCACHESERVER_H
#include "utils/UISingleton.h"



enum _ERROR_DATA{
    OK  = 200,
    PARAMERTER_ERROR  = 601,
};

class JsWebDbCacheServer:public UISingleton<JsWebDbCacheServer>
{
    friend class UISingleton<JsWebDbCacheServer>;

public:
    JsWebDbCacheServer();

    QString  process(const QString & jsonstr);

private:
    QString returnError(int code,const QString errormsg);
    QString returnOK( QString value );

};

#endif // JSWEBDBCACHESERVER_H
