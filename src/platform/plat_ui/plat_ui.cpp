#include "plat_ui.h"

#include <server/platUiService.h>
//#include "server/platviewuiservice.h"




 Plat_ui * gInterface = new Plat_ui();


DllLoadInterface * GetDllInterface()
{
    return gInterface;

}
DllLoadInterface * Plat_ui_lib_GetDllInterface()
{
    return gInterface;

}

Plat_ui::Plat_ui()
{

}

Plat_ui::~Plat_ui()
{


}

QString Plat_ui::GetModuleName()
{
    return "Plat_ui";
};

void Plat_ui::Init( int tcount ,  char * argv[]  )
{
/*
    PlatUiService::GetInstance()->AddService("View_interface", (void *)new PlatViewUiService() );
        QTimer::singleShot(1 * 1000, this, [=]() {
            R_LoadUi();
        });
        */
}

bool Plat_ui::CheckVersion( QString version )
{
    return false;
}

void Plat_ui::R_LoadUi()
{

}
