#ifndef UIYL_PLUGINCONSTRUCTIONINTERFACE_H
#define UIYL_PLUGINCONSTRUCTIONINTERFACE_H

#include <QObject>
#include "interface/uiyl_editpluginterface.h"

#include "plat_ui_global.h"

class PLAT_UI_EXPORT UIYL_PluginConstructionInterface : public QObject
{
    Q_OBJECT
public:
    explicit UIYL_PluginConstructionInterface(QObject *parent = nullptr);

    virtual QList<UIYL_EditPlugInterface *>  ConstructionEditPlugin(QObject * obj ) = 0;
    virtual void DeleteEditPlugin() = 0;
	
	

signals:

};

#endif // UIYL_EDITPLUGINCONSTRUCTIONINTERFACE_H
