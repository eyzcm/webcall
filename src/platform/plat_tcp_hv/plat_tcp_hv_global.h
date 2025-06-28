#ifndef PLAT_TCP_HV_GLOBAL_H
#define PLAT_TCP_HV_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PLAT_TCP_HV_LIBRARY)
#  define PLAT_TCP_HV_EXPORT Q_DECL_EXPORT
#else
#  define PLAT_TCP_HV_EXPORT Q_DECL_IMPORT
#endif

#endif // PLAT_TCP_HV_GLOBAL_H
