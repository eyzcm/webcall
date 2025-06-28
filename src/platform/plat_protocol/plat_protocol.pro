QT -= gui

TEMPLATE = lib
DEFINES += PLAT_PROTOCOL_LIBRARY
QT += core gui multimedia network xml webenginewidgets webchannel gui-private svg

TEMPLATE = lib
DEFINES += PLAT_PROTOCOL_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += EL_PROJECTNAME=\\\"plat_protocol\\\"

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

macx {
include (../../Resources/macversion.h)
DEFINES += APP_VERSION_MAJOR=$$VERSION_MAJOR \
           APP_VERSION_MINOR=$$VERSION_MINOR \
           APP_VERSION_PATCH=$$VERSION_PATCH
}

win32:CONFIG(release, debug|release): DEBUG_RELEASE=/release
else:win32:CONFIG(debug, debug|release): DEBUG_RELEASE=/debug

msvc{
    DLLDESTDIR += $$OUT_PWD/../bin$$DEBUG_RELEASE
}
macx {
    DESTDIR += $$OUT_PWD/../bin$$DEBUG_RELEASE
}

include(protocol_biz/protocol_biz.pri)

SOURCES += \
    plat_protocol.cpp

HEADERS += \
    plat_protocol_global.h \
    plat_protocol.h


TRANSLATIONS += \
    $$PWD/../../translations/plat_protocol_en.ts

RC_FILE += ../../Resources/EasyLiaoClient.rc

INCLUDEPATH += ./ \
    ../plat_utils/ \
    ../plat_tcp/ \
    ../plat_http/ \
    ../platdata/ \
    ../plat_interface/ \
    ../el_session/ \
    ../plat_clientapi/ \
    ../../../../utils/src/utils/utils_interface \
    ../../../../utils/src/utils/el_common \
    ../../../../utils/src/utils/runInterface

win32 {
LIBS += \
-L$$OUT_PWD/../plat_utils$$DEBUG_RELEASE -lplat_utils  \
-L$$OUT_PWD/../platdata$$DEBUG_RELEASE -lplatdata \
-L$$OUT_PWD/../plat_interface$$DEBUG_RELEASE -lplat_interface \
-L$$OUT_PWD/../plat_clientapi$$DEBUG_RELEASE -lplat_clientapi \
-L$$OUT_PWD/../el_session$$DEBUG_RELEASE -lel_session \
-L$$OUT_PWD/../utils_interface$$DEBUG_RELEASE -lutils_interface \
-L$$OUT_PWD/../runInterface$$DEBUG_RELEASE -lrunInterface \
-L$$OUT_PWD/../el_common$$DEBUG_RELEASE -lel_common
}
macx {
LIBS += -L$$DESTDIR -lplat_utils -lplatdata  -lplat_interface  -lel_session  -lutils_interface  -lrunInterface  -lel_common
}

win32 {
    LIBS += -lkernel32
    LIBS += -luser32
    LIBS += -lgdi32
    LIBS += -lwinspool
    LIBS += -lcomdlg32
    LIBS += -ladvapi32
    LIBS += -lshell32
    LIBS += -lole32
    LIBS += -loleaut32
    LIBS += -luuid
    LIBS += -lodbc32
    LIBS += -lodbccp32
    }
msvc{
QMAKE_CXXFLAGS += /MP
}

CONFIG += thread exceptions rtti stl


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

