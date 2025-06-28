#include "customtoolbtn.h"

CustomToolBtn::CustomToolBtn(QWidget *parent) : QToolButton(parent)
{
}

CustomToolBtn::~CustomToolBtn()
{
}

void CustomToolBtn::enterEvent(QEvent *)
{
    Q_EMIT S_MoveOnButton(true);
}

void CustomToolBtn::leaveEvent(QEvent *)
{
    Q_EMIT S_MoveOnButton(false);
}
