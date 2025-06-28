#include "MyWidget.h"
#include <QApplication>
#include <QStyleOption>
#include <qpainter.h>


MyWidget::MyWidget(QWidget *parent) : QWidget(parent)
{
    m_isLeftMousePressed = false;
    m_isMouseMove        = false;
}

MyWidget::~MyWidget()
{
}

void MyWidget::paintEvent(QPaintEvent *event)
{
    //使得widget本身对setStyleSheet();起作用
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    QWidget::paintEvent(event);
}

void MyWidget::enterEvent(QEvent *event)
{
    unsetCursor();
}

void MyWidget::mousePressEvent(QMouseEvent *event)
{
    m_isLeftMousePressed = true;
    m_isMouseMove        = false;
    m_point              = QCursor::pos();
    QWidget::mousePressEvent(event);
}

void MyWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isLeftMousePressed && m_point != QCursor::pos())
    {
        m_isMouseMove = true;
        Q_EMIT S_OnMouseMoveEvent(event);
    }
    else
    {
        m_isMouseMove = false;
    }

    QWidget::mouseMoveEvent(event);
}

void MyWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isMouseMove && m_isLeftMousePressed)
    {
        Q_EMIT S_OnMouseReleaseEvent(event);
    }

    m_isLeftMousePressed = false;
    m_isMouseMove        = false;
    QWidget::mouseReleaseEvent(event);
}

void MyWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    m_isLeftMousePressed = false;
    m_isMouseMove        = false;
    Q_EMIT S_OnMouseDoubleClicked(event);
    QWidget::mouseDoubleClickEvent(event);
}
