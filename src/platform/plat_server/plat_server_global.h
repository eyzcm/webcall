#ifndef PLAT_SERVER_GLOBAL_H
#define PLAT_SERVER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PLAT_SERVER_LIBRARY)
#  define PLAT_SERVER_EXPORT Q_DECL_EXPORT
#else
#  define PLAT_SERVER_EXPORT Q_DECL_IMPORT
#endif

#endif // PLAT_SERVER_GLOBAL_H
