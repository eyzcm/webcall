QT += core gui multimedia network xml webenginewidgets webchannel gui-private svg

TEMPLATE = lib
DEFINES += WEBCALL_VIEW_LIBRARY

CONFIG += c++11


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += EL_PROJECTNAME=\\\"webcall_view\\\"

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32:CONFIG(release, debug|release): DLLDESTDIR += $$OUT_PWD/../bin/release
win32:CONFIG(debug, debug|release): DLLDESTDIR += $$OUT_PWD/../bin/debug

#include("./ui/yl_tree/yl_tree.pri")

include(./service/uiservice.pri)
include("ui_common.pri")
include("./ui/ai/aiwnd.pri")
include(./popui/popui.pri)
include(./popui/EL_DouYin_Business/EL_DouYin_Business.pri)
include(./assistui/assistui.pri)
include(./assistui/EL_CommonWord/EL_CommonWord.pri)
include(./chatui/chatui.pri)
include(./settingui/settingui.pri)
include(./lefttree/lefttree.pri)
include(./EL_BuiltInBrowser/EL_BuiltInBrowser.pri)
include(./topmenu.pri)





INCLUDEPATH += ./ \
    ../webcall_utils/ui/common/ \
    ../webcall_serviceModel/EL_BasicModule/ \
    ./ui/ \
    ./ui/common/SummaryTag/ \
    ./ui/ai/ \
    ./ui/common/ \
    ./ui/topmenu/ \
    ./popui/ \
    ./popui/ui/ \
    ./popui/ui/tableview/ \
    ./popui/ui/announcement/ \
    ./settingui/ \
    ./settingui/ui/ \
    ./assistui/ui/ \
    ./assistui/ui/ListView/ \
    ./assistui/ui/TreeView/ \
    ./assistui/ui/robot/ \
    ./lefttree/ \
    ./lefttree/ui/JY_UI/ \
    ./lefttree/ui/ \
    ./lefttree/ui/JY_UI/ListView/ \
    ./lefttree/ui/JY_UI/TreeView/ \
    ./lefttree/ui/YL_UI/ \
    ./lefttree/ui/YL_UI/ListView/ \
    ./lefttree/ui/YL_UI/TreeView/ \
    ./chatui/ui/ \
    ./settingui/ \
    ./assistui/ \
    ./chatui/ \
    ./popui/ \
    ../../../../platform/src/platform/plat_utils/common/ \
    ../../../../platform/src/platform/plat_utils/ \
    ../../../../platform/src/platform/plat_view \
    ../../../../platform/src/platform/plat_view/ui \
    ../../../../platform/src/platform/platdata/ \
    ../../../../platform/src/platform/plat_interface/ \
    ../../../../platform/src/platform/el_session/ \
    ../webcall_data/ \
    ../webcall_dbModel/ \
    ../webcall_Model/ \
    ../webcall_interface/ \
    ../webcall_dbservice/ \
    ../webcall_utils/ \
    ../webcall_servicemodel/ \
    ../webcall_controll/ \
    ../webcall_servicemodel/ui/YL_UI/topmenu \
    ../webcall_servicemodel/EL_BuiltInBrowser/ \
    ../../../../utils/src/utils/utils_interface \
    ../../../../utils/src/utils/el_common \
    ../../../../utils/src/utils/runinterface \
    $$OUT_PWD

SOURCES += \
    webcall_view.cpp

HEADERS += \
    UIObserverImp.h \
    webcall_view_global.h \
    webcall_view.h


TRANSLATIONS += \
    $$PWD/../../translations/webcall_view_en.ts

RC_FILE += ../../Resources/EasyLiaoClient.rc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../plat_utils/release -lplat_utils \
-L$$OUT_PWD/../runinterface/release -lruninterface \
-L$$OUT_PWD/../utils_interface/release -lutils_interface \
-L$$OUT_PWD/../el_common/release -lel_common \
-L$$OUT_PWD/../platdata/release -lplatdata \
-L$$OUT_PWD/../plat_utils/release -lplat_utils \
-L$$OUT_PWD/../plat_view/release -lplat_view \
-L$$OUT_PWD/../plat_interface/release -lplat_interface \
-L$$OUT_PWD/../el_session/release -lel_session \
-L$$OUT_PWD/../webcall_data/release -lwebcall_data \
-L$$OUT_PWD/../webcall_dbModel/release -lwebcall_dbModel \
-L$$OUT_PWD/../webcall_Model/release -lwebcall_Model \
-L$$OUT_PWD/../webcall_interface/release -lwebcall_interface \
-L$$OUT_PWD/../webcall_utils/release -lwebcall_utils \
-L$$OUT_PWD/../webcall_serviceModel/release -lwebcall_serviceModel \
-L$$OUT_PWD/../webcall_dbservice/release -lwebcall_dbservice \
-L$$OUT_PWD/../webcall_controll/release -lwebcall_controll



else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../plat_utils/debug -lplat_utils \
-L$$OUT_PWD/../runinterface/debug -lruninterface \
-L$$OUT_PWD/../utils_interface/debug -lutils_interface \
-L$$OUT_PWD/../el_common/debug -lel_common \
-L$$OUT_PWD/../plat_utils/debug -lplat_utils \
-L$$OUT_PWD/../plat_interface/debug -lplat_interface \
-L$$OUT_PWD/../platdata/debug -lplatdata \
-L$$OUT_PWD/../plat_view/debug -lplat_view \
-L$$OUT_PWD/../el_session/debug -lel_session \
-L$$OUT_PWD/../webcall_data/debug -lwebcall_data \
-L$$OUT_PWD/../webcall_dbModel/debug -lwebcall_dbModel \
-L$$OUT_PWD/../webcall_Model/debug -lwebcall_Model \
-L$$OUT_PWD/../webcall_interface/debug -lwebcall_interface \
-L$$OUT_PWD/../webcall_utils/debug -lwebcall_utils \
-L$$OUT_PWD/../webcall_serviceModel/debug -lwebcall_serviceModel \
-L$$OUT_PWD/../webcall_dbservice/debug -lwebcall_dbservice \
-L$$OUT_PWD/../webcall_controll/debug -lwebcall_controll



#启动多核编译 MSVC编译器
QMAKE_CXXFLAGS += /MP
#让项目使用预编译头文件的支持
CONFIG         += recompile_header
PRECOMPILED_HEADER = ./stdafx.h

CONFIG += thread exceptions rtti stl


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
QMAKE_LFLAGS_RELEASE += /MAP
QMAKE_CFLAGS_RELEASE += /Zi
QMAKE_LFLAGS_RELEASE += /debug /opt:ref
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

RESOURCES += \
    ../../../../client-webcall-main/src/main/client_resouce/easyliaoclient.qrc
