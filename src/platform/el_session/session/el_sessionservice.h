#ifndef EL_SESSIONSERVICE_H
#define EL_SESSIONSERVICE_H

#include <QObject>
#include "utils/UISingleton.h"

#include "el_session_global.h"

class LoginTransitionsPageInterface;
class el_sessionInterface;

class EL_SESSION_EXPORT el_sessionService : public QObject
{
    Q_OBJECT


public:
    ~el_sessionService(){};
    static el_sessionService *GetInstance();
    el_sessionService();
private:

    class Garbo
    {
    public:
        ~Garbo();
    };
    static el_sessionService *m_instance;
public:
    void AddService( QString name, void * service );

    el_sessionInterface * GetSessionService();

    LoginTransitionsPageInterface * GetLoginTransitionsPageService();
signals:

private:
    el_sessionInterface * m_sessionService;
    LoginTransitionsPageInterface * m_LoginPageService;
};
#include "session/logintransitionspageinterface.h"
#include "session/el_sessioninterface.h"

#define gSessionService  el_sessionService::GetInstance()->GetSessionService()
#define gLoginTransitionsPage  el_sessionService::GetInstance()->GetLoginTransitionsPageService()

#endif // EL_SESSIONSERVICE_H
