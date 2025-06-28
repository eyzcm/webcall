#ifndef REQUESTLOGOFF_H
#define REQUESTLOGOFF_H

#include "protocol/IRequest.h"
#include "platdata_global.h"


class PLATDTA_EXPORT RequestLogoff : public IRequest
{
    Q_OBJECT
public:
    RequestLogoff(void);
    ~RequestLogoff(void);
    RequestObjectName("RequestLogoff");
    virtual QString MakeXml();
};
#endif // REQUESTLOGOFF_H
