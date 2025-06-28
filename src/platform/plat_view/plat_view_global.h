#ifndef PLAT_VIEW_GLOBAL_H
#define PLAT_VIEW_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PLAT_VIEW_LIBRARY)
#  define PLAT_VIEW_EXPORT Q_DECL_EXPORT
#else
#  define PLAT_VIEW_EXPORT Q_DECL_IMPORT
#endif

#endif // PLAT_VIEW_GLOBAL_H
