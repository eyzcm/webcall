#ifndef TCPREQUEST_H
#define TCPREQUEST_H


#include "tcpmessage.h"
#include "platdata_global.h"

class PLATDTA_EXPORT TcpRequest : public TcpMessage
{
    Q_OBJECT

public:
    TcpRequest();
    ~TcpRequest();

private:
};

#endif // TCPREQUEST_H
