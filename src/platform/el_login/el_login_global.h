#ifndef EL_LOGIN_GLOBAL_H
#define EL_LOGIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EL_LOGIN_LIBRARY)
#  define EL_LOGIN_EXPORT Q_DECL_EXPORT
#else
#  define EL_LOGIN_EXPORT Q_DECL_IMPORT
#endif

#endif // EL_LOGIN_GLOBAL_H
