#ifndef REQUESTKEEPLIVE_H
#define REQUESTKEEPLIVE_H

#include "IRequest.h"

class RequestKeepLive : public IRequest
{
    Q_OBJECT
public:
    RequestKeepLive(void);
    ~RequestKeepLive(void);
    RequestObjectName("RequestKeepLive");
    virtual QString MakeXml();
};
#endif // REQUESTKEEPLIVE_H
