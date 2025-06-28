QT += core gui multimedia network xml webenginewidgets webchannel gui-private svg

TEMPLATE = lib
DEFINES += PLAT_TCP_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DEFINES += EL_PROJECTNAME=\\\"plat_tcp\\\"

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


include(plat_tcp.pri)
include(tcpimp.pri)


INCLUDEPATH += ./ \
    ../plat_utils/ \
    ../plat_interface/ \
    ../platdata/ \
    ../el_session/ \
    ../../../../utils/src/utils/utils_interface \
    ../../../../utils/src/utils/el_common \
    ../../../../utils/src/utils/el_common/include \
    ../../../../utils/src/utils/runInterface

#DllloadService.cpp \
SOURCES += \
    plat_tcp.cpp

HEADERS += \
    plat_tcp_global.h \
    plat_tcp.h


TRANSLATIONS += \
    $$PWD/../../translations/plat_tcp_en.ts

msvc{
LIBS += -L$$OUT_PWD/../plat_utils$$DEBUG_RELEASE -lplat_utils \
-L$$OUT_PWD/../utils_interface$$DEBUG_RELEASE -lutils_interface \
-L$$OUT_PWD/../el_common$$DEBUG_RELEASE -lel_common \
-L$$OUT_PWD/../runInterface$$DEBUG_RELEASE -lrunInterface \
-L$$OUT_PWD/../platdata$$DEBUG_RELEASE -lplatdata \
-L$$OUT_PWD/../plat_interface$$DEBUG_RELEASE -lplat_interface \
-L$$OUT_PWD/../el_session$$DEBUG_RELEASE -lel_session
}
macx {
LIBS += -L$$DESTDIR -lplat_utils -lutils_interface -lel_common -lrunInterface -lplatdata -lplat_interface -lel_session
}

RC_FILE += ../../Resources/EasyLiaoClient.rc

QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
msvc{
QMAKE_LFLAGS_RELEASE += /MAP
QMAKE_LFLAGS_RELEASE += /debug /opt:ref
}
QMAKE_CFLAGS_RELEASE += /Zi

CONFIG += thread exceptions rtti stl

msvc{
    QMAKE_CXXFLAGS += /source-charset:utf-8 /execution-charset:utf-8
}

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
