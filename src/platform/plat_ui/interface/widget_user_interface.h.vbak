#ifndef WIDGET_USER_INTERFACE_H
#define WIDGET_USER_INTERFACE_H

#include <QWidget>
#include "plat_ui_global.h"

class PLAT_UI_EXPORT widget_user_interface : public QWidget
{
    Q_OBJECT
public:
    explicit widget_user_interface(QWidget *parent = nullptr);

	virtual void SetUserName(QString name) {};
	virtual void    SetAccountNumber(QString strNum) {};
	virtual QString GetAccountNUmber() { return ""; };
    //设置用户头像
	virtual void SetHeadPortrait(QString strImagePath) {};

signals:

};

#endif // WIDGET_USER_INTERFACE_H
