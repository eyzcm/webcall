TEMPLATE = subdirs

CONFIG += ordered


SUBDIRS += \
    el_common \
    RunInterface
msvc{    
    SUBDIRS += log_console \
    outdata_console
}
SUBDIRS += utils_interface \
    EL_ScreenShot \
    RouterMonitor
msvc{    
    SUBDIRS += utils_Upgrade
}
    SUBDIRS += utils_log \
    utils_manager
