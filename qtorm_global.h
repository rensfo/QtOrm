#ifndef QTORM_GLOBAL_H
#define QTORM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTORM_LIBRARY)
#  define QTORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTORMSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QTORM_GLOBAL_H
