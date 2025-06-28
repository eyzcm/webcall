#ifndef EL_SESSION_GLOBAL_H
#define EL_SESSION_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EL_SESSION_LIBRARY)
#  define EL_SESSION_EXPORT Q_DECL_EXPORT
#else
#  define EL_SESSION_EXPORT Q_DECL_IMPORT
#endif

#endif // EL_SESSION_GLOBAL_H
