#include "plat_view.h"
#include "ui/PlatMenuNav.h"
#include <QTimer>
#include "server/platuiservice.h"
#include "server/platviewuiservice.h"
#include <tree/UIYL_TreeWidget.h>


Plat_view * gInterface = new Plat_view();


DllLoadInterface * GetDllInterface()
{
    return gInterface;

}
DllLoadInterface * Plat_ui_lib_GetDllInterface()
{
    return gInterface;

}

Plat_view::Plat_view()
{
    m_menuNav = nullptr;
}


El_MenuNavInterface * Plat_view::MenuNav()
{
    if( m_menuNav == nullptr )
    {
        m_menuNav = new PlatMenuNav();

    }
    return m_menuNav;
}


QString Plat_view::GetModuleName()
{
    return "Plat_view";
};

void Plat_view::Init( int tcount ,  char * argv[]  )
{

    UIYL_TreeWidget* widget_dialogue_list_panel = new UIYL_TreeWidget(nullptr);
    widget_dialogue_list_panel->setObjectName(QString::fromUtf8("widget_dialogue_list_panel"));
    PlatUiService::GetInstance()->AddService("UIYL_ChatTreeInterface", (void*)widget_dialogue_list_panel);
    PlatUiService::GetInstance()->AddService("View_interface", (void *)new PlatViewUiService() );

        QTimer::singleShot(1 * 1000, this, [=]() {
            R_LoadUi();
        });

}

bool Plat_view::CheckVersion( QString version )
{
    return false;
}

void Plat_view::R_LoadUi()
{



}
