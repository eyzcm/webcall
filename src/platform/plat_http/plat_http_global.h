#ifndef PLAT_HTTP_GLOBAL_H
#define PLAT_HTTP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PLAT_HTTP_LIBRARY)
#  define PLAT_HTTP_EXPORT Q_DECL_EXPORT
#else
#  define PLAT_HTTP_EXPORT Q_DECL_IMPORT
#endif

#endif // PLAT_HTTP_GLOBAL_H
