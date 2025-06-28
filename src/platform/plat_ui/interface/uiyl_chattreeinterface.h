#ifndef UIYL_CHATTREEINTERFACE_H
#define UIYL_CHATTREEINTERFACE_H

#include <QWidget>
#include "plat_ui_global.h"

class PLAT_UI_EXPORT UIYL_ChatTreeInterface : public QWidget
{
    Q_OBJECT
public:
    explicit UIYL_ChatTreeInterface(QWidget *parent = nullptr);

signals:

};

#endif // UIYL_CHATTREEINTERFACE_H
