#ifndef PLAT_UTILS_GLOBAL_H
#define PLAT_UTILS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PLAT_UTILS_LIBRARY)
#  define PLAT_UTILS_EXPORT Q_DECL_EXPORT
#else
#  define PLAT_UTILS_EXPORT Q_DECL_IMPORT
#endif

#endif // PLAT_UTILS_GLOBAL_H
