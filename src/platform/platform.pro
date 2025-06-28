TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += \
    plat_utils \
    platData \
    el_session \
    plat_interface \
    plat_permission \
    plat_http \
    plat_tcp \
    plat_clientapi

msvc{    
   #SUBDIRS +=  plat_tcp_hv
}
SUBDIRS +=     plat_db \
    plat_model \
    plat_protocol \
    plat_server \
    plat_controll \
    plat_ui \
    plat_view \
    el_login
