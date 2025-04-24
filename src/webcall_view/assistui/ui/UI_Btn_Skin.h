#pragma once

#include "data/uiskininfo.h"
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>

class UI_Btn_Skin : public QPushButton
{
    Q_OBJECT

public:
    UI_Btn_Skin(UISkinInfo* info, QWidget* parent = nullptr);
    ~UI_Btn_Skin();

public:
    void        SetCheck(bool check);
    UISkinInfo* GetSkinInfo();
    QString     GetFullPath();

private:
    QString GetThumbnailFullFileNameByKey(QString key);

private:
    void paintEvent(QPaintEvent* event);

private:
    UISkinInfo* m_info;
    QPixmap     m_pixmapBgImg;
    QPixmap     m_pixmapCheck;
    bool        m_isCheck;
    QString     m_strFullPath;
};
