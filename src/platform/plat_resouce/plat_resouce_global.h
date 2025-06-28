#ifndef EL_RESOUCE_GLOBAL_H
#define EL_RESOUCE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PLAT_RESOUCE_LIBRARY)
#  define PLAT_RESOUCE_EXPORT Q_DECL_EXPORT
#else
#  define PLAT_RESOUCE_EXPORT Q_DECL_IMPORT
#endif

#endif // EL_RESOUCE_GLOBAL_H
