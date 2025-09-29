#ifndef EL_COMMON_GLOBAL_H
#define EL_COMMON_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EL_COMMON_LIBRARY)
#  define EL_COMMON_EXPORT Q_DECL_EXPORT
#else
#  define EL_COMMON_EXPORT Q_DECL_IMPORT
#endif

#endif // EL_COMMON_GLOBAL_H
