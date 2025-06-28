QT -= gui

TEMPLATE = lib
DEFINES += PLAT_SERVER_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += EL_PROJECTNAME=\\\"webcall_dbservice\\\"

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

include(serverimpl.pri)


TRANSLATIONS += \
    $$PWD/../../translations/webcall_dbservice_en.ts

INCLUDEPATH += ./ \
    ../plat_utils/common \
    ../plat_utils/js \
    ../plat_view/ui \
    ../plat_utils/ \
    ../platdata/ \
    ../platdata/ui/ \
    ../plat_interface/ \
    ../el_session/ \
    ../../../../utils/src/utils/utils_interface \
    ../../../../utils/src/utils/runinterface \
    ../../../../utils/src/utils/el_common/

msvc{
LIBS += -L$$OUT_PWD/../plat_utils$$DEBUG_RELEASE -lplat_utils  \
-L$$OUT_PWD/../platdata$$DEBUG_RELEASE -lplatdata  \
-L$$OUT_PWD/../utils_interface$$DEBUG_RELEASE -lutils_interface \
-L$$OUT_PWD/../el_session$$DEBUG_RELEASE -lel_session \
-L$$OUT_PWD/../runinterface$$DEBUG_RELEASE -lruninterface \
-L$$OUT_PWD/../plat_interface$$DEBUG_RELEASE -lplat_interface \
-L$$OUT_PWD/../el_common$$DEBUG_RELEASE -lel_common
}
macx {
LIBS += -L$$DESTDIR  -lplat_utils  -lplatdata   -lplat_view -lwebcall_dbmodel  -lutils_interface \
  -lruninterface  -lplat_interface  -lel_common
}

msvc{
    QMAKE_CXXFLAGS += /source-charset:utf-8 /execution-charset:utf-8
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

SOURCES += \
    plat_server.cpp

HEADERS += \
    plat_server_global.h \
    plat_server.h

TRANSLATIONS += \
    plat_server_zh_CN.ts

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
