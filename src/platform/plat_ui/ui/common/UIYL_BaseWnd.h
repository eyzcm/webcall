#ifndef UIYL_BaseWnd_H
#define UIYL_BaseWnd_H

#include <QEvent>
#include <QWidget>

#include "plat_ui_global.h"
class BaseWnd;


class PLAT_UI_EXPORT UIYL_BaseWnd : public QWidget
{
    Q_OBJECT

public:
    UIYL_BaseWnd(QWidget *parent = 0);
    ~UIYL_BaseWnd();
    virtual void CustomInitWnd(BaseWnd *baseWnd) = 0;
    void         LoadCustom();
    void         Init();
    void         show();
    void         showNormal();
    void         close();
    void         resize(int w, int h);

    BaseWnd *m_baseWnd;
    int          m_customInit;
signals:
    void S_HideWndAndDelCenterWidget();
};

#endif // UIYL_BaseWnd_H
