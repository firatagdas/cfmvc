load(qt_build_paths)
CONFIG += java
DESTDIR = $$MODULE_BASE_OUTDIR/jar

JAVACLASSPATH += $$PWD/src

JAVASOURCES += $$PWD/com/cf/mvc/CFAndroidMvcConroller.java

# install
target.path = $$[QT_INSTALL_PREFIX]/jar
INSTALLS += target
