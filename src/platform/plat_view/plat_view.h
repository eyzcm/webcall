#ifndef PLAT_VIEW_H
#define PLAT_VIEW_H

#include "plat_view_global.h"
#include "session/el_menunavinterface.h"
#include <QWidget>

#include <dllloadinterface.h>

extern "C" PLAT_VIEW_EXPORT DllLoadInterface * GetDllInterface();
extern "C" PLAT_VIEW_EXPORT DllLoadInterface * Plat_ui_lib_GetDllInterface();


class PLAT_VIEW_EXPORT Plat_view: public DllLoadInterface
{
public:
    Plat_view();

    El_MenuNavInterface * MenuNav();

    virtual QString GetModuleName();
    virtual void Init( int  ,  char * argv[]  );
    virtual bool CheckVersion( QString version );

public slots:
    void R_LoadUi();
private:



private:
    El_MenuNavInterface * m_menuNav;
};


#endif // PLAT_VIEW_H
