#ifndef HTTPUPLOADFILECMD_H
#define HTTPUPLOADFILECMD_H


#include "protocol/httpcmd.h"
#include "plat_interface_global.h"

class PLAT_INTERFACE_EXPORT HttpUploadFileCmd : public HttpCmd
{
    Q_OBJECT

public:
    HttpUploadFileCmd();
    ~HttpUploadFileCmd();

public:
    Property_QString(FileName, fileName);
    Property_QString(ChatId, chatId);
    Property_QString(CompanyId, companyId);
    Property_QString(FileType, fileType);
    Property_QString(FileUploadURL, fileUploadURL);
    Property_QString(AdvanceNotice, advancenotice);

public:
    EUSERTYPE GetUserType() const
    {
        return m_type;
    }
    void SetUserType(EUSERTYPE val)
    {
        m_type = val;
    }

public:
    void Upload();

private:
    void MakeUri();

private:
    EUSERTYPE m_type;
signals:
    void S_SendingStatus(QString code, QString reason, QString rsp); // code 状态码 reason 原因 rsp 空
};

#endif // HTTPUPLOADFILECMD_H
