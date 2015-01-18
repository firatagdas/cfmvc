CXX_MODULE = cfmvc
TARGET  = declarative_cfmvc
TARGETPATH = CFMvc
IMPORT_VERSION = 1.0

QT += qml quick cfmvc cfmvc-private
SOURCES += \
    $$PWD/cfmvc.cpp

load(qml_plugin)

OTHER_FILES += qmldir
