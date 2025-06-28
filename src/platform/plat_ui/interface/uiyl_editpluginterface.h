#ifndef UIYL_EDITPLUGINTERFACE_H
#define UIYL_EDITPLUGINTERFACE_H

#include <QObject>

#include "plat_ui_global.h"

class PLAT_UI_EXPORT UIYL_EditPlugInterface : public QObject
{
    Q_OBJECT
public:
    explicit UIYL_EditPlugInterface(QObject *parent = nullptr);

signals:

};

#endif // UIYL_EDITPLUGINTERFACE_H
