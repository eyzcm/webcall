#ifndef PLAT_MODEL_GLOBAL_H
#define PLAT_MODEL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PLAT_MODEL_LIBRARY)
#  define PLAT_MODEL_EXPORT Q_DECL_EXPORT
#else
#  define PLAT_MODEL_EXPORT Q_DECL_IMPORT
#endif

#endif // PLAT_MODEL_GLOBAL_H
