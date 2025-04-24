#ifndef WEBCALL_VIEW_H
#define WEBCALL_VIEW_H

#include "webcall_view_global.h"

#include <dllloadinterface.h>
extern "C" WEBCALL_VIEW_EXPORT DllLoadInterface * GetDllInterface();
class ManagerWork;
class WEBCALL_VIEW_EXPORT Webcall_view: public DllLoadInterface
{


public:
    Webcall_view();
    virtual ~Webcall_view();

    virtual QString GetModuleName();
    virtual void Init( int  ,  char * argv[]  );
    virtual bool CheckVersion( QString version );


private:




};


#endif // WEBCALL_VIEW_H
