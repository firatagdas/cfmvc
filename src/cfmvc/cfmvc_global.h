#ifndef CFMVCGLOBAL_H
#define CFMVCGLOBAL_H

#include <QtCore/qglobal.h>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#  if defined(QT_BUILD_CFMVC_LIB)
#    define Q_CFMVC_EXPORT Q_DECL_EXPORT
#  else
#    define Q_CFMVC_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define Q_CFMVC_EXPORT
#endif

QT_END_NAMESPACE
#endif // CFMVCGLOBAL_H
