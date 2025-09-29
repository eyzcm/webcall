#ifndef LOG_CONSOLE_GLOBAL_H
#define LOG_CONSOLE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LOG_CONSOLE_LIBRARY)
#  define LOG_CONSOLE_EXPORT Q_DECL_EXPORT
#else
#  define LOG_CONSOLE_EXPORT Q_DECL_IMPORT
#endif



#endif // LOG_CONSOLE_GLOBAL_H
