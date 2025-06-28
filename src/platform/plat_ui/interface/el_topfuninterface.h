#ifndef EL_TOPFUNINTERFACE_H
#define EL_TOPFUNINTERFACE_H

#include <QWidget>
#include "plat_ui_global.h"

class PLAT_UI_EXPORT EL_TopFunInterface : public QWidget
{
    Q_OBJECT
public:
    explicit EL_TopFunInterface(QWidget *parent = nullptr);

	virtual void setTopFun() {};

signals:

};

#endif // EL_TOPFUNINTERFACE_H
