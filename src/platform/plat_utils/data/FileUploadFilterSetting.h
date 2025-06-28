#pragma once
#include "utils/DefineHelper.h"
#include "data/CommInfo.h"


class  FileUploadFilterSetting
{
public:
    FileUploadFilterSetting(void);
    ~FileUploadFilterSetting(void);

    //    "fileUploadMax": "20480",
    //    "douYinFileUploadFormart": "bmp,gif,png,jpeg,jpg",
    //    "uploadFileFormart": "txt,jpg,jpeg,png,gif,bmp,pdf,rm,doc,docx,ppt,pptx,xls,xlsx,rar,zip,pps,MP4,mp4,WebM,Ogg",
    //    "playFileFormart": "mp4,flv,f4v,webm,m4v,mov,3gp,3g2,rm,rmvb,wmv,avi,asf,mpg,mpeg,mpe,ts,div,dv,divx,vob,dat,mkv,lavf,cpk,dirac,ram,qt,fli,flc,mod"

public:
    Property_Int(FileUploadMax, fileUploadMax);
    Property_QString(UploadFileFormart, uploadFileFormart);
    Property_QString(PicFormart, picFormart);
    Property_QString(PlayFileFormart, playFileFormart);

public:
    QString                                        GetFileFilterByEngineId(QString engineId);
    QString                                        GetPicFilterByEngineId(QString engineId);
    QMap<QString, QList<FileUploadFilterSetting*>> GetEnginerFiter() const
    {
        return m_enginerFiter;
    }
    void SetEnginerFiter(const QMap<QString, QList<FileUploadFilterSetting*>>& val)
    {
        m_enginerFiter = val;
    }

    int CheckFileSize(QString filename, QString engineId, int filetype);

private:
    QMap<QString, QList<FileUploadFilterSetting*>> m_enginerFiter;
};
