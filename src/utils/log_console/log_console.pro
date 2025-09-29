QT -= gui
QT += xml

TEMPLATE = lib
DEFINES += LOG_CONSOLE_LIBRARY

CONFIG += c++11


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += EL_PROJECTNAME=\\\"log_console\\\"

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

INCLUDEPATH += ./ \
    ../RunInterface \
    ../../../third/spdlog/include

include(agent.pri)


SOURCES += \
        log_console.cpp

TRANSLATIONS += \
    $$PWD/../../translations/log_console_en.ts

RC_FILE += ../../Resources/EasyLiaoClient.rc

msvc{
LIBS += -L$$OUT_PWD/../runinterface$$DEBUG_RELEASE -lruninterface
}
macx {
LIBS += -L$$DESTDIR -lruninterface
}



QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
msvc{
QMAKE_LFLAGS_RELEASE += /MAP
QMAKE_LFLAGS_RELEASE += /debug /opt:ref
}
QMAKE_CFLAGS_RELEASE += /Zi

msvc{
    QMAKE_CXXFLAGS += /source-charset:utf-8 /execution-charset:utf-8
}


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    log_console.h \
    log_console_global.h
