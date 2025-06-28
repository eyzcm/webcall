#include "httpuploadfilecmd.h"
#include "./utils/DefineHelper.h"
#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include "service/util_service.h"

HttpUploadFileCmd::HttpUploadFileCmd() : HttpCmd()
{
}

HttpUploadFileCmd::~HttpUploadFileCmd()
{
}

void HttpUploadFileCmd::Upload()
{
    MakeUri();
    QFileInfo info;
    info             = QFileInfo(FileName());
    QString tempName = info.fileName();
    SetHeader("fileName", tempName);
    SetHeader("Content-Type", "application/octet-stream");

    QFile f(FileName());

    if (f.size() > 1024 * 1024 * 20) //上传文件限制在20M以内
    {
        Q_EMIT S_SendingStatus("400", tr("文件过大,上传文件限制在20M以内"), AdvanceNotice());
        return;
    }
    if (!f.open(QIODevice::ReadOnly))
    {
        Q_ASSERT(false);
        GERRORLOG("打开文件出错");
        Q_EMIT S_SendingStatus("400", tr("打开文件错误,原因:") + f.errorString(), AdvanceNotice());
        return;
    }
    Q_EMIT S_SendingStatus("200", FileName(), AdvanceNotice());
    QByteArray data = f.readAll();
    f.close();
    Post(data);
}

void HttpUploadFileCmd::MakeUri()
{
    if (GetUserType() == E_VISITOR)
    {
        QString format = "%1?type=%2&chatId=%3&cId=%4";
        QString strUri = "";
        strUri         = format.arg(FileUploadURL()).arg(FileType()).arg(ChatId()).arg(CompanyId());
        Uri(strUri);
    }
    else if (GetUserType() == E_CUSTOMER)
    {
        QString format = "%1?type=%2&to=%3&cId=%4";
        QString strUri = "";
        strUri         = format.arg(FileUploadURL()).arg(FileType()).arg(ChatId()).arg(CompanyId());
        Uri(strUri);
    }
    else if (GetUserType() == E_MONITOR)
    {
        QString format = "%1?type=%2&chatId=%3&cId=%4";
        QString strUri = "";
        strUri         = format.arg(FileUploadURL()).arg(FileType()).arg(ChatId()).arg(CompanyId());
        Uri(strUri);
    }
    else if (GetUserType() == E_CHATGROUP)
    {
        QString format = "%1?type=%2&groupId=%3&cId=%4";
        QString strUri = "";
        strUri         = format.arg(FileUploadURL()).arg(FileType()).arg(ChatId()).arg(CompanyId());
        Uri(strUri);
        Uri(strUri);
    }
    else if (GetUserType() == E_UEDITOR)
    {
        QString format = "%1?type=%2&groupId=%3&cId=%4";
        QString strUri = "";
        strUri         = format.arg(FileUploadURL()).arg(FileType()).arg(ChatId()).arg(CompanyId());
        Uri(strUri);
    }
    else if (GetUserType() == E_USERTYPE_DEFAULT)
    {
        QString format = "%1?type=%2&cId=%4";
        QString strUri = "";
        strUri         = format.arg(FileUploadURL()).arg(FileType()).arg(CompanyId());
        Uri(strUri);
    }
    else
    {
        Q_ASSERT(false);
    }
}
