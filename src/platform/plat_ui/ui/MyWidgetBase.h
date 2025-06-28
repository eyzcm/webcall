#pragma once

#include <QApplication>
#include <QDesktopWidget>
#include <QEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QWidget>
#ifdef _MSC_VER
#include <Windows.h>
#include <windowsx.h>
#endif
#include "plat_ui_global.h"

#define Padding 5
#define Pixmap_Size 30
#define Response_Height 36

enum ShadowSize
{
    NoShadow    = 0,
    SmallShadow = 5,
    BigShadow   = 10
};

class PLAT_UI_EXPORT MyWidgetBase : public QWidget
{
    Q_OBJECT

public:
    MyWidgetBase(QWidget *parent = nullptr);
    ~MyWidgetBase();
    void SetMoveEnable(bool enable);
    void SetDragEnable(bool enable);
    void SetShadowEnable(bool enable);
    void SetShadowSize(ShadowSize shadowSize = BigShadow);
    void SetMoveWidgetHeight(int height = Response_Height);
    void SetLeftPressDown(bool leftPressDown);
    int  GetShadowSize() const
    {
        return m_nShadowSize;
    }
    enum MouseDirection
    {
        Up,
        Down,
        Left,
        Right,
        LeftTop,
        LeftBottom,
        RightBottom,
        RightTop,
        None
    };
    void Region(const QPoint &cursorGlobalPoint);
    void LeftResize(QPoint &tl, QPoint &rb, QPoint &gloPoint, QRect &rMove);
    void RightResize(QPoint &rb, QPoint &gloPoint, QRect &rMove);
    void UpResize(QPoint &tl, QPoint &rb, QPoint &gloPoint, QRect &rMove);
    void DownResize(QPoint &rb, QPoint &gloPoint, QRect &rMove);
    void DrawShadow(QPainter &painter);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent *event);

    void ShowCenter();
    void ShowCenter_Model(); //模态窗口

    int  m_WidgetType;
    bool m_IsMoveEnable; //是否可以移动
    bool m_IsDragEnable; //是否可以拉伸

    void  reductionSize(bool r = true);
    bool  m_reduction; //是否还原窗口大小设置
    QSize m_oldMax;
    QSize m_oldMin;

private:
    int            m_nShadowSize;
    int            m_nCanMoveHeight;
    bool           m_IsLeftPressDown;
    bool           m_IsShadowEnable;
    QPoint         m_DragPosition;
    QPoint         m_LeftMousePressPos;
    QPoint         m_potSizeWithBorder;
    MouseDirection m_MouseDir;
    QRect          m_CurrentScreenRect; //当前屏幕大小,用于多屏幕时,判断是否页面移动到其他屏幕中
};
