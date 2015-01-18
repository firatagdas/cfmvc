ANDROID_BUNDLED_JAR_DEPENDENCIES = \
    jar/CFAndroidMvc-bundled.jar
ANDROID_JAR_DEPENDENCIES = \
    jar/CFAndroidMvc.jar

INCLUDEPATH += $$PWD

PUBLIC_HEADERS += \
    cfmvc_global.h

android {
    QT += androidextras


} else:ios {


} else {

}

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS \
    $$PWD/cfmvc.h \
    $$PWD/cfmodel.h \
    $$PWD/cfapimodel.h

SOURCES += \
    $$PWD/cfmvc.cpp \
    $$PWD/cfmodel.cpp \
    $$PWD/cfapimodel.cpp