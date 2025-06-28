#ifndef VM_AUTHLOGINCMD_H
#define VM_AUTHLOGINCMD_H


#include "protocol/visitormonitortcpcmd.h"

class  VM_AuthLoginCmd : public VisitorMonitorTcpCmd
{
    Q_OBJECT

public:
    VM_AuthLoginCmd();
    ~VM_AuthLoginCmd();
    void         SetKey(const QString& key);
    virtual void MakeCmd();

public:
    Property_QString(CompanyId, companyId);
    Property_QString(UserId, userId);

private:
    QString m_key;
};

#endif // VM_AUTHLOGINCMD_H
