#ifndef REQUESTRETRYPROCESS_H
#define REQUESTRETRYPROCESS_H
#include "IRequest.h"

class RequestReTryProcess : public IRequest
{
    Q_OBJECT
public:
    RequestReTryProcess(void);
    ~RequestReTryProcess(void);

    RequestObjectName("RequestReTryProcess");
    void SetType(QString val)
    {
        m_type = val;
    }
    virtual QString MakeXml();

private:
    QString m_type;
};
#endif // REQUESTRETRYPROCESS_H
