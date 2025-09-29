#ifndef IRESPONSE_H
#define IRESPONSE_H


#include <QObject>
#include <stdio.h>
#include <wchar.h>

#define ResponseObjectName(x)          \
public:                                \
    virtual QString ObjectName() const \
    {                                  \
        return x;                      \
    }

#define STDPRINTF(x) \
    printf(x);       \
    printf("\n");    \
    fflush(stdout);
#define STDERR(x)          \
    fprintf(stderr, x);    \
    fprintf(stderr, "\n"); \
    fflush(stderr);

class IResponse : public QObject
{
    Q_OBJECT
public:
    IResponse(void);
    ~IResponse(void);

public:
    void SetReq(QString val)
    {
        m_req = val;
    }
    QString GetReq()
    {
        return m_req;
    }

public:
    static void     staticExecte(const QString &data);
    virtual void    Execute(QString);
    virtual QString ObjectName() const = 0;
    virtual QString MakeXml()          = 0;

private:
    QString m_req;
};
#endif // IRESPONSE_H
