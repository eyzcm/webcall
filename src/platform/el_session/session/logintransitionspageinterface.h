#ifndef LOGINTRANSITIONSPAGEINTERFACE_H
#define LOGINTRANSITIONSPAGEINTERFACE_H
#include <qobject.h>
#include "el_session_global.h"
class EL_SESSION_EXPORT LoginTransitionsPageInterface
{
public:
    LoginTransitionsPageInterface();
    ~LoginTransitionsPageInterface(){};

    virtual void     SendSchedule(QString percentage, QString msg)=0;
};

#endif // LOGINTRANSITIONSPAGEINTERFACE_H
