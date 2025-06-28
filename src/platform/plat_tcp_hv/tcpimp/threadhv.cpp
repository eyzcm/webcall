#include "threadhv.h"


ThreadHv::ThreadHv( ClientTcpImp * imp)
{
    m_imp = nullptr;

}

void ThreadHv::run()
{

    if( m_imp )
    {
        m_imp->tcp_client_run();
        m_imp->tcp_client_free();
    }

}
