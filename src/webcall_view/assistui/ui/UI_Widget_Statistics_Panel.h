#pragma once

#include "UIObserverImp.h"
#include "UI_Widget_Statistics_BrokenNet_Panel.h"
#include "ui_UI_Widget_Statistics_Panel.h"
#include <QMouseEvent>
#include <QWidget>

class UI_Widget_Statistics_Panel : public QWidget, public IUIObserverImp
{
    Q_OBJECT

public:
    UI_Widget_Statistics_Panel(QWidget *parent = 0);
    ~UI_Widget_Statistics_Panel();

public:
    void         Init();
    void         SetBrokenNet(int isBroken);
    virtual void Update(QString type, QObject *obj);
    virtual void RemoveUpdate(QString type, QString body);

private:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    Ui::UI_Widget_Statistics_PanelClass   ui;
    UI_Widget_Statistics_BrokenNet_Panel *m_pBrokenNet;
};
