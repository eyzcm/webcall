#ifndef DLLLOADINTERFACE_H
#define DLLLOADINTERFACE_H

#include <QObject>
#include <QString>

class DllLoadInterface :public QObject
{
    Q_OBJECT

public:
    DllLoadInterface();

    virtual QString GetModuleName(){return "";};
    virtual void Init( int  ,  char * argv[]  ){ Q_EMIT S_Init();};
    virtual bool CheckVersion( QString version ){return false;};

 signals:
     void S_Init();



};


typedef DllLoadInterface * (*fGetDllInterface)();

#endif // DLLLOADINTERFACE_H
