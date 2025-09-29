#include "BaseConstructionData.h"
#include "ConstStringHelper.h"


#include <QMap>

BaseConstructionData::BaseConstructionData(void)
{
}

BaseConstructionData::~BaseConstructionData(void)
{
}

int BaseConstructionData::FreeObj(const QString &datatype, void *itemobj)
{
    int ret = -1;
    if (IsIndexOfString(datatype, "LIST"))
    {
        QList<void *> *list = (QList<void *> *)itemobj;
        qDeleteAll(*list);
        list->clear();
        ret = 200;
    }
    else if (IsIndexOfString(datatype, "MAP"))
    {
        QMap<QString, void *> *map = (QMap<QString, void *> *)itemobj;
        for (auto itor = map->begin(); itor != map->end(); itor++)
        {
            void *obj = itor.value();
            if (obj)
            {
                delete obj;
            }
        }
        map->clear();
        ret = 200;
    }
    else
    {
        if (itemobj)
        {
            delete itemobj;
            ret = 200;
        }
    }
    return ret;
}

void *BaseConstructionData::CreateObj(const QString &datatype, void *itemobj)
{
    void *ret = nullptr;

    return ret;
}

bool BaseConstructionData::IsIndexOfString(QString sourceStr, QString indexStr)
{
    bool isRet = false;
    if (sourceStr.indexOf(indexStr, 0, Qt::CaseInsensitive) != -1)
    {
        isRet = true;
    }
    return isRet;
}

int BaseConstructionData::AddObjToList(const QString &datatype, void *itemobj, void *outlist)
{
    int ret = -1;
    if (IsIndexOfString(datatype, ConstStringHelper::m_cLIST))
    {
        if (itemobj)
        {
            QList<void *> *plist = (QList<void *> *)outlist;
            plist->append(itemobj);
        }
        ret = 200;
    }
    else
    {
        // Q_ASSERT(false);
        // do nothing。
    }
    return ret;
}
