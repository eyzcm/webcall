#ifndef UIYL_CENTERCONTENTINTERFACE_H
#define UIYL_CENTERCONTENTINTERFACE_H

#include <QWidget>

#include "plat_ui_global.h"

class PLAT_UI_EXPORT UIYL_CenterContentInterface : public QWidget
{
    Q_OBJECT
public:
    explicit UIYL_CenterContentInterface(QWidget *parent = nullptr);

signals:

};

#endif // UIYL_CENTERCONTENTINTERFACE_H
