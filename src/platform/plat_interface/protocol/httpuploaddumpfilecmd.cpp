#include "httpuploaddumpfilecmd.h"
#include "./utils/DefineHelper.h"
#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include "service/util_service.h"

HttpUploadDumpFileCmd::HttpUploadDumpFileCmd() : HttpCmd()
{
}

HttpUploadDumpFileCmd::~HttpUploadDumpFileCmd()
{
}

void HttpUploadDumpFileCmd::Upload()
{
    MakeUri();
    QFileInfo info;
    info             = QFileInfo(FileName());
    QString tempName = info.fileName();
    SetHeader("fileName", tempName);
    SetHeader("Content-Type", "application/octet-stream");

    QFile f(FileName());

    if (!f.open(QIODevice::ReadOnly))
    {
        Q_ASSERT(false);
        GERRORLOG("打开文件出错！！！");
        Q_EMIT S_Finished("400", "file can't open!!!", "");
        return;
    }
    QByteArray data = f.readAll();
    f.close();
    Post(data);
}

void HttpUploadDumpFileCmd::MakeUri()
{
    QString format = "%1?cId=%2";
    QString strUri = "";
    strUri         = format.arg(DumpUploadURL()).arg(CompanyId());
    Uri(strUri);
}
