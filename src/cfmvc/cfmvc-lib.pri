# ANDROID_BUNDLED_JAR_DEPENDENCIES = jar/CFAndroidMvc-bundled.jar
# ANDROID_JAR_DEPENDENCIES = jar/CFAndroidMvc.jar

INCLUDEPATH += $$PWD

PUBLIC_HEADERS += \
    $$PWD/cfmvc_global.h \
    $$PWD/cfmvc.h \
    $$PWD/cfmodel.h \
    $$PWD/cfapimodel.h \
    $$PWD/cfscreenmodel.h \
    $$PWD/cfassetmodel.h

PRIVATE_HEADERS += \
    $$PWD/cfapimodel_p.h

SOURCES += \
    $$PWD/cfmvc.cpp \
    $$PWD/cfmodel.cpp \
    $$PWD/cfapimodel.cpp \
    $$PWD/cfapimodel_p.cpp \
    $$PWD/cfscreenmodel.cpp \
    $$PWD/cfassetmodel.cpp

android {
    QT += androidextras

    HEADERS += \
        $$PWD/cfandroidutil.h

} else:ios {

    HEADERS += \
        $$PWD/cfiosutil.h

} else {

}

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS

