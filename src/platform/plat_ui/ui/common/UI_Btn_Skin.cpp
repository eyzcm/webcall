#include "UI_Btn_Skin.h"
#include "utils/utilshelper.h"

UI_Btn_Skin::UI_Btn_Skin(UISkinInfo* info, QWidget* parent) : QPushButton(parent), m_info(info), m_isCheck(false)
{
    m_strFullPath = UtilsHelper::GetFullPathByRunDir(info->FileName());
    m_pixmapBgImg.load(GetThumbnailFullFileNameByKey(info->Key()));
    m_pixmapCheck.load(":/YL/Resources/YL/TopRight/Icon_ChangeSkin_Check.png");
}

UI_Btn_Skin::~UI_Btn_Skin()
{
}

void UI_Btn_Skin::SetCheck(bool check)
{
    if (check)
    {
        m_isCheck = true;
    }
    else
    {
        m_isCheck = false;
    }
}

UISkinInfo* UI_Btn_Skin::GetSkinInfo()
{
    return m_info;
}

QString UI_Btn_Skin::GetFullPath()
{
    return m_strFullPath;
}

void UI_Btn_Skin::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), m_pixmapBgImg);

    if (m_isCheck)
    {
        QRect rect = QRect(60, 25, 15, 15);
        painter.drawPixmap(rect, m_pixmapCheck);
    }
}

QString UI_Btn_Skin::GetThumbnailFullFileNameByKey(QString key)
{
    static QString strThumbnailFormat = "%1/Res/Thumbnail/%2.png";

    QString ret = strThumbnailFormat.arg(UtilsHelper::GetRunAppDir()).arg(key);
    return ret;
}
