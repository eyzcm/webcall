#ifndef HTTPUPLOADDUMPFILECMD_H
#define HTTPUPLOADDUMPFILECMD_H


#include "protocol/httpcmd.h"
#include "plat_interface_global.h"

class PLAT_INTERFACE_EXPORT HttpUploadDumpFileCmd : public HttpCmd
{
    Q_OBJECT

public:
    HttpUploadDumpFileCmd();
    ~HttpUploadDumpFileCmd();

public:
    Property_QString(FileName, fileName);
    Property_QString(DumpUploadURL, dumpUploadURL);
    Property_QString(CompanyId, companyId);

public:
    void Upload();

private:
    void MakeUri();
};

#endif // HTTPUPLOADDUMPFILECMD_H
