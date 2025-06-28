#ifndef PLAT_DB_GLOBAL_H
#define PLAT_DB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PLAT_DB_LIBRARY)
#  define PLAT_DB_EXPORT Q_DECL_EXPORT
#else
#  define PLAT_DB_EXPORT Q_DECL_IMPORT
#endif

#define  MEMDB  "memory"

#endif // PLAT_DB_GLOBAL_H
