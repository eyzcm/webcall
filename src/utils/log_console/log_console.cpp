#include "log_console.h"

#include <cstdio>
#include <chrono>

#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"  // support for loading levels from the environment variable
#include "spdlog/fmt/ostr.h" // support for user defined types
#include "agent/gframework.h"

#include "agent/utilshelper.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"

std::shared_ptr<spdlog::logger> g_rotating_logger;
std::shared_ptr<spdlog::logger> g_rotating_errorlogger;
std::shared_ptr<spdlog::logger> g_rotating_protocollogger;
std::shared_ptr<spdlog::logger> g_rotating_testlogger;
std::shared_ptr<spdlog::logger> g_rotating_debuglogger;
std::shared_ptr<spdlog::logger> g_rotating_scriptlogger;
std::shared_ptr<spdlog::logger> g_rotating_webcallinterfacelogger;



log_console * gInterface = new log_console();

DllLoadInterface * GetDllInterface()
{
//    gInterface->connect( gInterface->m_main, SIGNAL(S_Init()),gInterface,SLOT(R_Init()));

    return gInterface;

}

DllLoadInterface * log_console_lib_GetDllInterface()
{
    return GetDllInterface();

}




log_console::log_console()
{

}


QString log_console::GetModuleName()
{
    return "log_console";
};

void log_console::Init( int argc ,  char * argv[]  )
{

   spdlog::cfg::load_env_levels();
    QString logfile = QString("%1/%2").arg(UtilsHelper::GetAppdataLogDir()).arg("im.log");
    QString logprotocolfile = QString("%1/%2").arg(UtilsHelper::GetAppdataLogDir()).arg("protocol.log");
    QString errorfile = QString("%1/%2").arg(UtilsHelper::GetAppdataLogDir()).arg("errorfile.log");
    QString testfile = QString("%1/%2").arg(UtilsHelper::GetAppdataLogDir()).arg("test.log");
    QString debugfile = QString("%1/%2").arg(UtilsHelper::GetAppdataLogDir()).arg("debug.log");
    QString scriptfile = QString("%1/%2").arg(UtilsHelper::GetAppdataLogDir()).arg("script.log");
    QString webcallinterfacefile = QString("%1/%2").arg(UtilsHelper::GetAppdataLogDir()).arg("webcallinterface.log");


     g_rotating_logger = spdlog::rotating_logger_mt("log", logfile.toLocal8Bit().toStdString(), 1048576 * 10, 4);

     g_rotating_logger->flush_on( spdlog::level::trace );

    g_rotating_protocollogger = spdlog::rotating_logger_mt("protocol", logprotocolfile.toLocal8Bit().toStdString(), 1048576 * 10, 4);
    g_rotating_protocollogger->flush_on(spdlog::level::trace);

    g_rotating_webcallinterfacelogger = spdlog::rotating_logger_mt("webcallinterface", webcallinterfacefile.toLocal8Bit().toStdString(), 1048576 * 10, 4);
    g_rotating_webcallinterfacelogger->flush_on(spdlog::level::trace);

    g_rotating_scriptlogger = spdlog::rotating_logger_mt("script", scriptfile.toLocal8Bit().toStdString(), 1048576 * 10, 4);
    g_rotating_scriptlogger->flush_on(spdlog::level::trace);

    g_rotating_errorlogger = spdlog::rotating_logger_mt("error", errorfile.toLocal8Bit().toStdString(), 1048576 * 5, 1);
    g_rotating_errorlogger->flush_on(spdlog::level::trace);

    g_rotating_debuglogger = spdlog::rotating_logger_mt("debug", debugfile.toLocal8Bit().toStdString(), 1048576 * 5, 2);
    g_rotating_debuglogger->flush_on(spdlog::level::trace);

    g_rotating_testlogger = spdlog::rotating_logger_mt("test", testfile.toLocal8Bit().toStdString(), 1048576 * 5, 2);
    g_rotating_testlogger->flush_on(spdlog::level::trace);



    gFramework->Init();


}

bool log_console::CheckVersion( QString version )
{
    return false;
}
