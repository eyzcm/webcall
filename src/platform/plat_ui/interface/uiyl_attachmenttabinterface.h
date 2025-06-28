#ifndef UIYL_ATTACHMENTTABINTERFACE_H
#define UIYL_ATTACHMENTTABINTERFACE_H

#include <QWidget>
#include "plat_ui_global.h"

class PLAT_UI_EXPORT UIYL_AttachmentTabInterface : public QWidget
{
    Q_OBJECT
public:
    explicit UIYL_AttachmentTabInterface(QWidget *parent = nullptr);

    virtual QString GetId() = 0;
    virtual QString GetName() = 0;


signals:

};

#endif // UIYL_ATTACHMENTTABINTERFACE_H
