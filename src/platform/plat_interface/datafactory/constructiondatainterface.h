#ifndef CONSTRUCTIONDATAINTERFACE_H
#define CONSTRUCTIONDATAINTERFACE_H
#include <QObject>
#include "plat_interface_global.h"

class PLAT_INTERFACE_EXPORT ConstructionDataInterface : public QObject
{
    Q_OBJECT
public:
    ConstructionDataInterface();

    virtual void * CreateObj(const QString & datatype,void * itemobj) = 0;
    virtual int  AddObjToList(const QString & datatype,void *itemobj,void * outlist) = 0;

signals:

};

#endif // CONSTRUCTIONDATAINTERFACE_H
