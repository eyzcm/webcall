#pragma once

#include <QWidget>
#include "plat_ui_global.h"

class PLAT_UI_EXPORT MyWidget : public QWidget
{
    Q_OBJECT

public:
    MyWidget(QWidget *parent = nullptr);
    ~MyWidget();
    QPoint m_point;

    void enterEvent(QEvent *event);
    //    void leaveEvent(QEvent *event);
    virtual void paintEvent(QPaintEvent *event);

signals:
    void S_OnMouseDoubleClicked(QMouseEvent *event);
    void S_OnMouseMoveEvent(QMouseEvent *event);
    void S_OnMouseReleaseEvent(QMouseEvent *event);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

private:
    bool m_isLeftMousePressed;
    bool m_isMouseMove;
};
