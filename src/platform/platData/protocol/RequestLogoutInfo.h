#ifndef REQUESTLOGOUTINFO_H
#define REQUESTLOGOUTINFO_H

#include <protocol/IRequest.h>
#include "platdata_global.h"





class PLATDTA_EXPORT RequestLogoutInfo : public IRequest
{
    Q_OBJECT
public:
    RequestLogoutInfo(void);
    ~RequestLogoutInfo(void);
    RequestObjectName("RequestLogoutInfo");
    virtual QString MakeXml();
};
#endif // REQUESTLOGOUTINFO_H
