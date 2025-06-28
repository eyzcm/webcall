#ifndef PLAT_UI_GLOBAL_H
#define PLAT_UI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PLAT_UI_LIBRARY)
#  define PLAT_UI_EXPORT Q_DECL_EXPORT
#else
#  define PLAT_UI_EXPORT Q_DECL_IMPORT
#endif

#endif // PLAT_UI_GLOBAL_H
