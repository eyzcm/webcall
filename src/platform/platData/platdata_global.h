#ifndef PLATDTA_GLOBAL_H
#define PLATDTA_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PLATDTA_LIBRARY)
#  define PLATDTA_EXPORT Q_DECL_EXPORT
#else
#  define PLATDTA_EXPORT Q_DECL_IMPORT
#endif

#endif // PLATDTA_GLOBAL_H
