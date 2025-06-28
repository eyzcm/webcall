QT += core gui multimedia network xml webenginewidgets webchannel gui-private svg

TEMPLATE = lib
DEFINES += PLAT_CONTROLL_LIBRARY

CONFIG += c++11



DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += EL_PROJECTNAME=\\\"plat_controll\\\"
DEFINES += gIProtocolAgent=gPlatProtocolService UICommonDataModel=UIPlatCommonDataModel CommonModelService=PlatCommonModelService

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

include(plat_controll.pri)


INCLUDEPATH += ./ \
    ../plat_utils/common \
    ../plat_utils/js \
    ../plat_view \
    ../plat_view/ui \
    ../plat_utils/ \
    ../el_session/ \
    ../plat_view/ \
    ../platdata/ \
    ../plat_model/ \
    ../platdata/ui/ \
    ../plat_interface/ \
    ../../../../utils/src/utils/el_common/ \
    ../../../../utils/src/utils/utils_interface


SOURCES +=

HEADERS += \
    plat_controll_global.h \
    stdafx.h \
    stdafx.h

TRANSLATIONS += \
    plat_controll_zh_CN.ts



msvc{
LIBS += -L$$OUT_PWD/../el_session$$DEBUG_RELEASE -lel_session \
-L$$OUT_PWD/../utils_interface$$DEBUG_RELEASE -lutils_interface \
-L$$OUT_PWD/../plat_utils$$DEBUG_RELEASE -lplat_utils \
-L$$OUT_PWD/../platdata$$DEBUG_RELEASE -lplatdata \
-L$$OUT_PWD/../plat_interface$$DEBUG_RELEASE -lplat_interface \
-L$$OUT_PWD/../plat_model$$DEBUG_RELEASE -lplat_model \
-L$$OUT_PWD/../el_common$$DEBUG_RELEASE -lel_common
}
macx {
LIBS += -L$$DESTDIR -lel_session  -lplat_utils  -lplatdata  -lel_common
}

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

