#ifndef UIYL_CHATTOOLBARBUTTONINTERFACE_H
#define UIYL_CHATTOOLBARBUTTONINTERFACE_H

#include <QWidget>
#include "ui/switchbutton.h"
#include "plat_ui_global.h"
class MChatItemData;


class PLAT_UI_EXPORT UIYL_ChatToolbarButtonInterface : public SwitchButton
{
    Q_OBJECT
public:
    explicit UIYL_ChatToolbarButtonInterface(QWidget *parent = nullptr);

    virtual  bool IsNoSendMessageDisable() = 0;  //在发消息状态下可用，在非发消息状态下显示灰色并且不能点击
    virtual  void OnClick( MChatItemData * chatItem ) = 0;
    virtual  QString ErrorMessage( ) { return "";};

    virtual void DisableBtnBySearchType( QString SearchingType ){};



signals:

};

#endif // UIYL_CHATTOOLBARBUTTONINTERFACE_H
