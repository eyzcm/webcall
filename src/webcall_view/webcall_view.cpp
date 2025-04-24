#include "webcall_view.h"

#include <ui/WebcallUiService.h>

#include <service/viewuiservice.h>



Webcall_view * gInterface = new Webcall_view();
DllLoadInterface * GetDllInterface()
{
//    gInterface->connect( gInterface->m_main, SIGNAL(S_Init()),gInterface,SLOT(R_Init()));

    return gInterface;

}


Webcall_view::Webcall_view()
{

}

Webcall_view::~Webcall_view()
{


}

QString Webcall_view::GetModuleName()
{
    return "Webcall_view";
};

void Webcall_view::Init( int tcount ,  char * argv[]  )
{

    WebcallUiService::GetInstance()->AddService("View_interface", (void *)new ViewUiService() );

}

bool Webcall_view::CheckVersion( QString version )
{
    return false;
}
