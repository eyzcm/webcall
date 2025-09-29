#ifndef DLLLOADSERVICE_H
#define DLLLOADSERVICE_H

#include <QObject>

class DllLoadService : public QObject
{
    Q_OBJECT
public:
    explicit DllLoadService(QObject *parent = nullptr);

    QString GetModuleName(){return "";};
    void Init( int /*argc*/ ,  char * /*argv[]*/  ){};
    bool CheckVersion( QString version ){return false;};



signals:
    QString S_GetModuleName();
    void S_Init( int /*argc*/ ,  char * /*argv[]*/  );
    bool S_CheckVersion( QString version );

};

#endif // DLLLOADSERVICE_H
