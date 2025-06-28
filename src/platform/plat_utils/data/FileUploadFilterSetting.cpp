#include "FileUploadFilterSetting.h"
#include "QFileInfo"
#include <QStringList>

FileUploadFilterSetting::FileUploadFilterSetting(void)
{
}

FileUploadFilterSetting::~FileUploadFilterSetting(void)
{
}

QString FileUploadFilterSetting::GetFileFilterByEngineId(QString engineId)
{
    QString retstr = this->m_uploadFileFormart;
    if (engineId.isEmpty() == false)
    {
        if (m_enginerFiter.find(engineId) != m_enginerFiter.end())
        {
            retstr                               = "";
            QList<FileUploadFilterSetting*> list = m_enginerFiter[engineId];
            foreach (FileUploadFilterSetting* setting, list)
            {
                retstr += setting->UploadFileFormart();
                retstr += " ";
            }
        }
    }
    return retstr;
}

QString FileUploadFilterSetting::GetPicFilterByEngineId(QString engineId)
{
    QString retstr = this->PicFormart();

    if (engineId.isEmpty() == false)
    {
        if (m_enginerFiter.find(engineId) != m_enginerFiter.end())
        {
            retstr                               = "";
            QList<FileUploadFilterSetting*> list = m_enginerFiter[engineId];
            foreach (FileUploadFilterSetting* setting, list)
            {
                retstr += setting->PicFormart();
                retstr += " ";
            }
        }
    }
    return retstr;
}

/************************************************************************/
/*    检查文件限制
   filename : 文件名
   engineId   搜索id
   filetype   类型    0 为文件， 1为图片
   返回值 ：
     成功返回为0 ，超过大小，返回限制大小
*/
/************************************************************************/
int FileUploadFilterSetting::CheckFileSize(QString filename, QString engineId, int filetype)
{
    int       limitfilesize = m_fileUploadMax * 1024;
    QFileInfo fileInfo(filename);

    if (engineId.isEmpty() == false)
    {
        if (m_enginerFiter.find(engineId) != m_enginerFiter.end())
        {
            QList<FileUploadFilterSetting*> list = m_enginerFiter[engineId];
            if (list.size() > 0)
            {
                limitfilesize = list.at(0)->FileUploadMax();
                if (list.size() > 1)
                {
                    QString ext;
                    int     lastchar = filename.lastIndexOf(".");
                    if (lastchar >= 0)
                    {
                        ext = filename.right(filename.length() - lastchar - 1);
                    }
                    foreach (FileUploadFilterSetting* setting, list)
                    {
                        QString fileFilter;
                        if (filetype == 0)
                            fileFilter = setting->UploadFileFormart();
                        else if (filetype == 1)
                            fileFilter = setting->PicFormart();

                        fileFilter        = fileFilter.replace(" *.", ",");
                        fileFilter        = fileFilter.replace("*.", "");
                        fileFilter        = fileFilter.replace(" ", "");
                        QStringList tlist = fileFilter.split(",");

                        if (tlist.contains(ext, Qt::CaseInsensitive))
                        {
                            limitfilesize = setting->FileUploadMax() * 1024;
                            break;
                        }
                    }
                }
            }
        }
    }

    if (fileInfo.size() < limitfilesize)
        return 0;

    return limitfilesize / 1024 / 1024;
}
