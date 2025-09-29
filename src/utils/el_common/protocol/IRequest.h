#ifndef IREQUEST_H
#define IREQUEST_H


#include <QObject>
#define RequestObjectName(x)     \
public:                          \
    virtual QString ObjectName() \
    {                            \
        return x;                \
    }

class IRequest : public QObject
{
    Q_OBJECT
public:
    IRequest(void);
    ~IRequest(void);

public:
    void SetReq(QString val)
    {
        m_req = val;
    }
    QString GetReq()
    {
        return m_req;
    }

    virtual QString MakeXml()    = 0;
    virtual QString ObjectName() = 0;

private:
    QString m_req;
};
#endif // IREQUEST_H
