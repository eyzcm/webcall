#ifndef CUSTOMTOOLBTN_H
#define CUSTOMTOOLBTN_H

#include <QToolButton>
#include "plat_ui_global.h"

class PLAT_UI_EXPORT CustomToolBtn : public QToolButton
{
    Q_OBJECT

public:
    CustomToolBtn(QWidget *parent);
    ~CustomToolBtn();
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);
signals:
    void S_MoveOnButton(bool isOn);
};

#endif // CUSTOMTOOLBTN_H
