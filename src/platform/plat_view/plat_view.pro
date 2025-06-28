QT += core gui multimedia network xml webenginewidgets webchannel gui-private svg

TEMPLATE = lib
DEFINES += PLAT_VIEW_LIBRARY

CONFIG += c++11


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += EL_PROJECTNAME=\\\"plat_view\\\"
DEFINES += gIProtocolAgent=gPlatProtocolService UICommonDataModel=UIPlatCommonDataModel CommonModelService=PlatCommonModelService CommonSignalService=PlatCommonSignalService


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

include(plat_view.pri)
include(server/server.pri)
include(ui.pri)
include(uiyl/uiyl.pri)
#include(interface/interface.pri)



INCLUDEPATH += ./ \
    ./ui \
    ./ui/other \
    ./uiyl \
    ./uiyl/common \
    ./uiyl/utils \
    ../el_session/ \
    ../plat_utils/ \
    ../plat_ui/ \
    ../plat_ui/ui/common/ \
    ../plat_interface/ \
    ../plat_ui/ui/ \
    ../platdata/ \
    ../plat_model/ \
    ../platdata/ui/ \
    ../../../../utils/src/utils/utils_interface \
    ../../../../utils/src/utils/el_common/ \
    ../../../../utils/src/utils/runinterface



SOURCES +=

HEADERS += \
    plat_view_global.h \
    stdafx.h

TRANSLATIONS += \
    plat_view_zh_CN.ts



msvc{
LIBS += -L$$OUT_PWD/../el_session$$DEBUG_RELEASE -lel_session \
-L$$OUT_PWD/../plat_utils$$DEBUG_RELEASE -lplat_utils \
-L$$OUT_PWD/../plat_ui$$DEBUG_RELEASE -lplat_ui \
-L$$OUT_PWD/../platdata$$DEBUG_RELEASE -lplatdata \
-L$$OUT_PWD/../plat_interface$$DEBUG_RELEASE -lplat_interface \
-L$$OUT_PWD/../utils_interface$$DEBUG_RELEASE -lutils_interface \
-L$$OUT_PWD/../runinterface$$DEBUG_RELEASE -lruninterface \
-L$$OUT_PWD/../plat_model$$DEBUG_RELEASE -lplat_model \
-L$$OUT_PWD/../el_common$$DEBUG_RELEASE -lel_common

}
macx {
LIBS += -L$$DESTDIR -lel_session  -lplat_utils  -lplatdata  -lel_common -lplat_interface
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

