#include "ElConnectStatusMap.h"

QList<QString> ElConnectStatusMap::statusDesc;

ElConnectStatusMap::ElConnectStatusMap()
{
}

ElConnectStatusMap::~ElConnectStatusMap()
{
}

QList<QString> ElConnectStatusMap::InitData()
{
    QList<QString> list;
    /*
    list.append();
    list.append();
    list.append();
    list.append();
    list.append();
    list.append();
    */

    return list;
}

QString ElConnectStatusMap::GetStatusDesc(int status)
{
    if ((status > 0) && (status < statusDesc.size()))
    {
        return statusDesc.at(status);
    }
    return "";
}
