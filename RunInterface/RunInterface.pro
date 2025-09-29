QT   += core gui

TEMPLATE = lib
DEFINES += RUNINTERFACE_LIBRARY

CONFIG += c++11
CONFIG += staticlib

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += EL_PROJECTNAME=\\\"runinterface\\\"

macx {
include (../../Resources/macversion.h)
DEFINES += APP_VERSION_MAJOR=$$VERSION_MAJOR \
           APP_VERSION_MINOR=$$VERSION_MINOR \
           APP_VERSION_PATCH=$$VERSION_PATCH
}

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32:CONFIG(release, debug|release): DEBUG_RELEASE=/release
else:win32:CONFIG(debug, debug|release): DEBUG_RELEASE=/debug
msvc{
    DLLDESTDIR += $$OUT_PWD/../bin$$DEBUG_RELEASE
}
macx {
    DESTDIR += $$OUT_PWD/../bin$$DEBUG_RELEASE
}

SOURCES += \
    dllloadinterface.cpp \
    dllloadservice.cpp \
    runinterface.cpp

HEADERS += \
    dllloadinterface.h \
    dllloadservice.h \
    runinterface.h



TRANSLATIONS += \
    $$PWD/../../translations/runinterface_en.ts

RC_FILE += ../../Resources/EasyLiaoClient.rc

QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
msvc{
QMAKE_LFLAGS_RELEASE += /MAP
QMAKE_LFLAGS_RELEASE += /debug /opt:ref
}
QMAKE_CFLAGS_RELEASE += /Zi
# 启用多线程、异常、RTTI、STL支持
CONFIG += thread exceptions rtti stl

msvc{
    QMAKE_CXXFLAGS += /source-charset:utf-8 /execution-charset:utf-8
}

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
