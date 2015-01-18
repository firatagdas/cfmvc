CONFIG += testcase parallel_test
TARGET = tst_cfmvc
osx:CONFIG -= app_bundle


QT += cfmvc cfmvc-private testlib
SOURCES += \
    tst_cfmvc.cpp
