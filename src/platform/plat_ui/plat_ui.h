#ifndef PLAT_UI_H
#define PLAT_UI_H

#include "plat_ui_global.h"

#include <dllloadinterface.h>

extern "C" PLAT_UI_EXPORT DllLoadInterface * GetDllInterface();
extern "C" PLAT_UI_EXPORT DllLoadInterface * Plat_ui_lib_GetDllInterface();

class ManagerWork;
class PLAT_UI_EXPORT Plat_ui: public DllLoadInterface
{


public:
    Plat_ui();
    virtual ~Plat_ui();

    virtual QString GetModuleName();
    virtual void Init( int  ,  char * argv[]  );
    virtual bool CheckVersion( QString version );

public slots:
    void R_LoadUi();
private:




};


#endif // PLAT_UI_H
