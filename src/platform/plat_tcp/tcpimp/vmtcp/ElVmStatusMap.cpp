#include "ElVmStatusMap.h"

QList<QString> ElVmStatusMap::statusDesc;

ElVmStatusMap::ElVmStatusMap()
{
}

ElVmStatusMap::~ElVmStatusMap()
{
}

QList<QString> ElVmStatusMap::InitData()
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

QString ElVmStatusMap::GetStatusDesc(int status)
{
    if ((status > 0) && (status < statusDesc.size()))
    {
        return statusDesc.at(status);
    }
    return "";
}
