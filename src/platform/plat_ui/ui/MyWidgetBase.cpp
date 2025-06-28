#include "MyWidgetBase.h"

#include <QGuiApplication>
#include <QScreen>

MyWidgetBase::MyWidgetBase(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setMouseTracking(true);

    m_nShadowSize       = BigShadow;
    m_nCanMoveHeight    = 52;
    m_WidgetType        = 0;
    m_IsMoveEnable      = true;
    m_IsDragEnable      = true;
    m_IsLeftPressDown   = false;
    m_IsShadowEnable    = true;
    m_reduction         = false;
    m_DragPosition      = QPoint();
    m_LeftMousePressPos = QPoint();
    m_potSizeWithBorder = QPoint();
    m_CurrentScreenRect = QRect();
    m_MouseDir          = None;
}

MyWidgetBase::~MyWidgetBase()
{
}

void MyWidgetBase::SetMoveEnable(bool enable)
{
    m_IsMoveEnable = enable;
}

void MyWidgetBase::SetDragEnable(bool enable)
{
    m_IsDragEnable = enable;
}

void MyWidgetBase::SetShadowEnable(bool enable)
{
    m_IsShadowEnable = enable;
}

void MyWidgetBase::SetShadowSize(ShadowSize shadowSize)
{
    m_nShadowSize = shadowSize;
}

void MyWidgetBase::SetMoveWidgetHeight(int height)
{
    m_nCanMoveHeight = height;
}

void MyWidgetBase::SetLeftPressDown(bool leftPressDown)
{
    m_IsLeftPressDown = leftPressDown;
}

void MyWidgetBase::Region(const QPoint &cursorGlobalPoint)
{
    int    x           = cursorGlobalPoint.x();
    int    y           = cursorGlobalPoint.y();
    QRect  rect        = this->rect();
    QPoint topLeft     = mapToGlobal(rect.topLeft());
    QPoint bottomRight = mapToGlobal(rect.bottomRight());

    if ((x <= topLeft.x() + Padding * 2 - 2) && (x >= topLeft.x() + Padding) && (y >= topLeft.y() + Padding) && (y <= topLeft.y() + Padding * 2 - 2))
    {
        // 左上角
        m_MouseDir = LeftTop;
        setCursor(Qt::SizeFDiagCursor); // 设置光标形状
    }
    else if ((x <= bottomRight.x() - Padding) && (x >= bottomRight.x() - Padding * 2 + 2) && (y <= bottomRight.y() - Padding) && (y >= bottomRight.y() - Padding * 2 + 2))
    {
        // 右下角
        m_MouseDir = RightBottom;
        setCursor(Qt::SizeFDiagCursor);
    }
    else if ((x <= topLeft.x() + Padding * 2 - 2) && (x >= topLeft.x() + Padding) && (y <= bottomRight.y() - Padding) && (y >= bottomRight.y() - Padding * 2 + 2))
    {
        //左下角
        m_MouseDir = LeftBottom;
        setCursor(Qt::SizeBDiagCursor);
    }
    else if ((x <= bottomRight.x() - Padding) && (x >= bottomRight.x() - Padding * 2 + 2) && (y >= topLeft.y() + Padding) && (y <= topLeft.y() + Padding * 2 - 2))
    {
        // 右上角
        m_MouseDir = RightTop;
        setCursor(Qt::SizeBDiagCursor);
    }
    else if ((x <= topLeft.x() + Padding * 2 - 2) && (x >= topLeft.x() + Padding))
    {
        // 左边
        m_MouseDir = Left;
        setCursor(Qt::SizeHorCursor);
    }
    else if ((x <= bottomRight.x() - Padding) && (x >= bottomRight.x() - Padding * 2 + 2))
    {
        // 右边
        m_MouseDir = Right;
        setCursor(Qt::SizeHorCursor);
    }
    else if ((y >= topLeft.y() + Padding) && (y <= topLeft.y() + Padding * 2 - 2))
    {
        // 上边
        m_MouseDir = Up;
        setCursor(Qt::SizeVerCursor);
    }
    else if ((y <= bottomRight.y() - Padding) && (y >= bottomRight.y() - Padding * 2 + 2))
    {
        // 下边
        m_MouseDir = Down;
        setCursor(Qt::SizeVerCursor);
    }
    else
    {
        // 默认
        m_MouseDir = None;
        setCursor(Qt::ArrowCursor);
    }
}

void MyWidgetBase::LeftResize(QPoint &tl, QPoint &rb, QPoint &gloPoint, QRect &rMove)
{
    int nDiff = m_DragPosition.x() - tl.x();
    if (rb.x() - (gloPoint.x() - nDiff) <= this->minimumWidth())
    {
        rMove.setX(tl.x());
    }
    else
    {
        if (gloPoint.x() > m_DragPosition.x())
        {
            rMove.setX(gloPoint.x() - nDiff);
        }
        else
        {
            if (gloPoint.x() - m_potSizeWithBorder.x() < tl.x())
            {
                rMove.setX(gloPoint.x() - nDiff);
            }
        }
    }
}

void MyWidgetBase::RightResize(QPoint &rb, QPoint &gloPoint, QRect &rMove)
{
    int nDiff = gloPoint.x() - m_DragPosition.x();
    if (gloPoint.x() < m_DragPosition.x())
    {
        rMove.setWidth(rMove.width() + nDiff);
    }
    else
    {
        if (gloPoint.x() + m_potSizeWithBorder.x() > rb.x())
        {
            rMove.setWidth(rMove.width() + nDiff);
        }
    }
}

void MyWidgetBase::UpResize(QPoint &tl, QPoint &rb, QPoint &gloPoint, QRect &rMove)
{
    int nDiff = m_DragPosition.y() - tl.y();

    if (rb.y() - (gloPoint.y() - nDiff) <= this->minimumHeight())
    {
        rMove.setY(tl.y());
    }
    else
    {
        if (gloPoint.y() > m_DragPosition.y())
        {
            rMove.setY(gloPoint.y() - nDiff);
        }
        else
        {
            if (gloPoint.y() - m_potSizeWithBorder.y() < tl.y())
            {
                rMove.setY(gloPoint.y() - nDiff);
            }
        }
    }
}

void MyWidgetBase::DownResize(QPoint &rb, QPoint &gloPoint, QRect &rMove)
{
    int nDiff = gloPoint.y() - m_DragPosition.y();
    if (gloPoint.y() < m_DragPosition.y())
    {
        rMove.setHeight(rMove.height() + nDiff);
    }
    else
    {
        if (gloPoint.y() + m_potSizeWithBorder.y() > rb.y())
        {
            rMove.setHeight(rMove.height() + nDiff);
        }
    }
}

void MyWidgetBase::DrawShadow(QPainter &painter)
{
    //绘制左上角、左下角、右上角、右下角、上、下、左、右边框
    QList<QPixmap> pixmaps;
    if (m_nShadowSize == BigShadow)
    {
        pixmaps.append(QPixmap(":/shadow_main/Resources/Image/Login/MainShadow/left.png"));
        pixmaps.append(QPixmap(":/shadow_main/Resources/Image/Login/MainShadow/right.png"));
        pixmaps.append(QPixmap(":/shadow_main/Resources/Image/Login/MainShadow/up.png"));
        pixmaps.append(QPixmap(":/shadow_main/Resources/Image/Login/MainShadow/down.png"));
        pixmaps.append(QPixmap(":/shadow_main/Resources/Image/Login/MainShadow/left_top.png"));
        pixmaps.append(QPixmap(":/shadow_main/Resources/Image/Login/MainShadow/right_top.png"));
        pixmaps.append(QPixmap(":/shadow_main/Resources/Image/Login/MainShadow/left_bottom.png"));
        pixmaps.append(QPixmap(":/shadow_main/Resources/Image/Login/MainShadow/right_bottom.png"));
    }
    else if (m_nShadowSize == SmallShadow)
    {
        pixmaps.append(QPixmap(":/shadow_child/Resources/Image/Login/ChildShadow/left.png"));
        pixmaps.append(QPixmap(":/shadow_child/Resources/Image/Login/ChildShadow/right.png"));
        pixmaps.append(QPixmap(":/shadow_child/Resources/Image/Login/ChildShadow/up.png"));
        pixmaps.append(QPixmap(":/shadow_child/Resources/Image/Login/ChildShadow/down.png"));
        pixmaps.append(QPixmap(":/shadow_child/Resources/Image/Login/ChildShadow/left_top.png"));
        pixmaps.append(QPixmap(":/shadow_child/Resources/Image/Login/ChildShadow/right_top.png"));
        pixmaps.append(QPixmap(":/shadow_child/Resources/Image/Login/ChildShadow/left_bottom.png"));
        pixmaps.append(QPixmap(":/shadow_child/Resources/Image/Login/ChildShadow/right_bottom.png"));
    }
    else
    {
        return; //无边框的时候直接返回
    }

    painter.drawPixmap(0, 0, Pixmap_Size, Pixmap_Size, pixmaps[4]);
    painter.drawPixmap(this->width() - Pixmap_Size, 0, Pixmap_Size, Pixmap_Size, pixmaps[5]);
    painter.drawPixmap(0, this->height() - Pixmap_Size, Pixmap_Size, Pixmap_Size, pixmaps[6]);
    painter.drawPixmap(this->width() - Pixmap_Size, this->height() - Pixmap_Size, Pixmap_Size, Pixmap_Size, pixmaps[7]);
    painter.drawPixmap(0, Pixmap_Size, Pixmap_Size, this->height() - 2 * Pixmap_Size, pixmaps[0].scaled(Pixmap_Size, this->height() - 2 * Pixmap_Size));
    painter.drawPixmap(this->width() - Pixmap_Size, Pixmap_Size, Pixmap_Size, this->height() - 2 * Pixmap_Size, pixmaps[1].scaled(Pixmap_Size, this->height() - 2 * Pixmap_Size));
    painter.drawPixmap(Pixmap_Size, 0, this->width() - 2 * Pixmap_Size, Pixmap_Size, pixmaps[2].scaled(this->width() - 2 * Pixmap_Size, Pixmap_Size));
    painter.drawPixmap(Pixmap_Size, this->height() - Pixmap_Size, this->width() - 2 * Pixmap_Size, Pixmap_Size, pixmaps[3].scaled(this->width() - 2 * Pixmap_Size, Pixmap_Size));
}

void MyWidgetBase::mousePressEvent(QMouseEvent *event)
{
    switch (event->button())
    {
        case Qt::LeftButton:
        {
            m_IsLeftPressDown = true;
            if (m_IsDragEnable)
            {
                if (m_MouseDir != None)
                {
                    QRect  rect = this->rect();
                    QPoint tl   = mapToGlobal(rect.topLeft());
                    QPoint rb   = mapToGlobal(rect.bottomRight());

                    if (m_MouseDir == Left)
                    {
                        m_potSizeWithBorder.setX(event->globalX() - tl.x());
                    }
                    else if (m_MouseDir == Right)
                    {
                        m_potSizeWithBorder.setX(rb.x() - event->globalX());
                    }
                    else if (m_MouseDir == Up)
                    {
                        m_potSizeWithBorder.setY(event->globalY() - tl.y());
                    }
                    else if (m_MouseDir == Down)
                    {
                        m_potSizeWithBorder.setY(rb.y() - event->globalY());
                    }
                    else if (m_MouseDir == LeftTop)
                    {
                        m_potSizeWithBorder.setX(event->globalX() - tl.x());
                        m_potSizeWithBorder.setY(event->globalY() - tl.y());
                    }
                    else if (m_MouseDir == RightTop)
                    {
                        m_potSizeWithBorder.setX(rb.x() - event->globalX());
                        m_potSizeWithBorder.setY(event->globalY() - tl.y());
                    }
                    else if (m_MouseDir == LeftBottom)
                    {
                        m_potSizeWithBorder.setX(event->globalX() - tl.x());
                        m_potSizeWithBorder.setY(rb.y() - event->globalY());
                    }
                    else if (m_MouseDir == RightBottom)
                    {
                        m_potSizeWithBorder.setX(rb.x() - event->globalX());
                        m_potSizeWithBorder.setY(rb.y() - event->globalY());
                    }
                }
            }
            if (!m_IsDragEnable || m_MouseDir == None) //移动前锁定窗口大小,防止多屏幕切换导致窗口大小发生变化
            {
                reductionSize();
            }

            m_DragPosition      = event->globalPos();
            m_LeftMousePressPos = event->pos();
            m_CurrentScreenRect = QGuiApplication::screenAt(m_DragPosition)->geometry();
            break;
        }
        case Qt::RightButton:
        {
            break;
        }
        default:
            QWidget::mousePressEvent(event);
            break;
    }
}

void MyWidgetBase::mouseMoveEvent(QMouseEvent *event)
{
    QPoint gloPoint = event->globalPos();
    if (!m_IsLeftPressDown)
    {
        if (m_IsDragEnable)
        {
            Region(gloPoint);
        }
    }
    else
    {
        QScreen *sc = QGuiApplication::screenAt(gloPoint);
        if (sc)
        {
            if (m_CurrentScreenRect == sc->geometry()) //没有移动到其他屏幕中时
            {
                QRect  rect = geometry();
                QPoint tl   = rect.topLeft();
                QPoint rb   = rect.bottomRight();
                if (m_MouseDir != None)
                {
                    if (m_IsDragEnable)
                    {
                        QRect rMove(tl, rb);
                        switch (m_MouseDir)
                        {
                            case Left:
                            {
                                LeftResize(tl, rb, gloPoint, rMove);
                                break;
                            }
                            case Right:
                            {
                                RightResize(rb, gloPoint, rMove);
                                break;
                            }
                            case Up:
                            {
                                UpResize(tl, rb, gloPoint, rMove);
                                break;
                            }
                            case Down:
                            {
                                DownResize(rb, gloPoint, rMove);
                                break;
                            }
                            case LeftTop:
                            {
                                LeftResize(tl, rb, gloPoint, rMove);
                                UpResize(tl, rb, gloPoint, rMove);
                                break;
                            }
                            case RightTop:
                                RightResize(rb, gloPoint, rMove);
                                UpResize(tl, rb, gloPoint, rMove);
                                break;
                            case LeftBottom:
                                LeftResize(tl, rb, gloPoint, rMove);
                                DownResize(rb, gloPoint, rMove);
                                break;
                            case RightBottom:
                                RightResize(rb, gloPoint, rMove);
                                DownResize(rb, gloPoint, rMove);
                                break;
                            default:
                                break;
                        }
                        if (rect.size() != rMove.size()) //优化拉伸时卡顿
                        {
                            setGeometry(rMove);
                        }
                    }
                }
                else
                {
                    if (m_IsMoveEnable && m_LeftMousePressPos.y() < Response_Height) //最大化时不可移动
                    {
                        QPoint diffValue   = gloPoint - m_DragPosition;
                        QRect  rc          = sc->availableGeometry();
                        int    nDeskHeight = rc.height();
                        int    nY          = nDeskHeight - (Response_Height + BigShadow);
                        if (this->y() > nY)
                        {
                            move(this->x() + diffValue.x(), nY);
                        }
                        else
                        {
                            move(this->x() + diffValue.x(), this->y() + diffValue.y());
                        }
                    }
                }
            }
            else
            {
                //将窗口定位在鼠标位置
                QPoint point  = m_LeftMousePressPos;
                QPoint gpoint = gloPoint;
                gpoint -= point;
                this->move(gpoint);
                m_CurrentScreenRect = sc->geometry();
            }
            m_DragPosition = gloPoint;
        }
    }
    QWidget::mouseMoveEvent(event);
}

void MyWidgetBase::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        reductionSize(false);
        m_IsLeftPressDown = false;
        //        if (m_MouseDir != None)
        //        {
        releaseMouse();
        setCursor(Qt::ArrowCursor);
        m_MouseDir = None;
        //        }
    }
    QWidget::mouseReleaseEvent(event);
}

void MyWidgetBase::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    if (m_IsShadowEnable)
    {
        QPainter painter(this);
        this->DrawShadow(painter);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(255, 255, 255, 0));
        painter.drawRect(QRect(m_nShadowSize, m_nShadowSize, this->width() - 2 * m_nShadowSize, this->height() - 2 * m_nShadowSize));
    }
}

void MyWidgetBase::ShowCenter()
{
    this->show();
}

void MyWidgetBase::ShowCenter_Model()
{
    this->setAttribute(Qt::WA_ShowModal, true);
    this->setWindowModality(Qt::ApplicationModal);
    ShowCenter();
}

void MyWidgetBase::reductionSize(bool r)
{
    if (r)
    {
        if (!m_reduction)
        {
            m_reduction = true;
            m_oldMin    = minimumSize();
            m_oldMax    = maximumSize();
            setMinimumSize(size());
            setMaximumSize(size());
        }
    }
    else
    {
        if (m_reduction)
        {
            m_reduction = false;
            setMinimumSize(m_oldMin);
            setMaximumSize(m_oldMax);
        }
    }
}
