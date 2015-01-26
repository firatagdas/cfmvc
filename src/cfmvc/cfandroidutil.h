#ifndef CFANDROIDUTIL_H
#define CFANDROIDUTIL_H

#include <QtAndroidExtras>
#include <QMap>

#define Q2J_STRING(S) ((S).isEmpty() ? NULL : QAndroidJniObject::fromString((S)).object())
#define J2Q_STRING(ENV, V) QString::fromUtf8((ENV)->GetStringUTFChars(V, 0))


#endif // CFANDROIDUTIL_H

