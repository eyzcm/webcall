#ifndef TCPRESPONSE_H
#define TCPRESPONSE_H


#include "tcpmessage.h"
#include "platdata_global.h"

class PLATDTA_EXPORT TcpResponse : public TcpMessage
{
    Q_OBJECT

public:
    TcpResponse();
    ~TcpResponse();

private:
};

#endif // TCPRESPONSE_H
