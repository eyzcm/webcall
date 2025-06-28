#ifndef COLLEAGUECMD_H
#define COLLEAGUECMD_H


#include "protocol/httpcmd.h"
#include "plat_interface_global.h"

class PLAT_INTERFACE_EXPORT ColleagueCmd : public HttpCmd
{
    Q_OBJECT

public:
    ColleagueCmd();
    ~ColleagueCmd();

private:
};

#endif // COLLEAGUECMD_H
