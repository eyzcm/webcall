#ifndef EL_MENUNAVINTERFACE_H
#define EL_MENUNAVINTERFACE_H
#include <QWidget>
#include "el_session_global.h"
class UIMenuDataInfo;


class EL_SESSION_EXPORT El_MenuNavInterface: public QWidget
{
	Q_OBJECT
public:
    El_MenuNavInterface();

    virtual void SetMenuData(QString key , UIMenuDataInfo * val ) = 0;
    virtual QMap<QString, UIMenuDataInfo *> GetMenuData( ) = 0;


signals:
    void s_navClick(QString key );


};

#endif // EL_MENUNAVINTERFACE_H
