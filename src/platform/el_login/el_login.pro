QT += core gui multimedia network xml webenginewidgets webchannel gui-private svg

TEMPLATE = lib
DEFINES += EL_LOGIN_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DEFINES += EL_PROJECTNAME=\\\"el_login\\\"

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

include(el_login.pri)


INCLUDEPATH += ./ \
    ui/Login_UI \
    ../plat_utils/common \
    ../plat_utils/js \
    ../plat_utils/ui \
    ../plat_utils/ \
    ../platdata/ \
    ../plat_interface/ \
    ../el_session/ \
    ../plat_permission/ \
    ../plat_view/ \
    ../plat_ui/ \
    ../../../../utils/src/utils/utils_interface \
    ../../../../utils/src/utils/el_common/utils \
    ../../../../utils/src/utils/el_common/data \
    ../../../../utils/src/utils/el_common/common \
    ../../../../utils/src/utils/el_common/include \
    ../../../../utils/src/utils/el_common \
    ../../../../utils/src/utils/runInterface \
    $$OUT_PWD


TRANSLATIONS += \
    $$PWD/../../translations/el_login_en.ts

RC_FILE += ../../Resources/EasyLiaoClient.rc
msvc{
LIBS += -L$$OUT_PWD/../plat_utils$$DEBUG_RELEASE -lplat_utils \
-L$$OUT_PWD/../el_common$$DEBUG_RELEASE -lel_common \
-L$$OUT_PWD/../runInterface$$DEBUG_RELEASE -lrunInterface \
-L$$OUT_PWD/../utils_interface$$DEBUG_RELEASE -lutils_interface \
-L$$OUT_PWD/../platdata$$DEBUG_RELEASE -lplatdata \
-L$$OUT_PWD/../plat_interface$$DEBUG_RELEASE -lplat_interface \
-L$$OUT_PWD/../plat_permission$$DEBUG_RELEASE -lplat_permission \
-L$$OUT_PWD/../plat_view$$DEBUG_RELEASE -lplat_view \
-L$$OUT_PWD/../plat_ui$$DEBUG_RELEASE -lplat_ui \
-L$$OUT_PWD/../el_session$$DEBUG_RELEASE -lel_session
}
macx {
LIBS += -L$$DESTDIR -lplat_utils -lel_common -lrunInterface -lutils_interface -lplatdata -lplat_interface -lplat_permission \
-lplat_view-lplat_ui  -lel_session
}


msvc{
QMAKE_CXXFLAGS += /MP
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
