#include "datafactory/ConstructionData.h"
#include <QMap>
#include "data/UIVisitorInfo.h"
#include "data/UIContactInfo.h"
#include "data/UIChatInfo.h"
#include "data/UIWaitVisitorInfo.h"
#include "data/UIContactGroupInfo.h"
#include "data/UIVisitorCol.h"

#include "data/UIHistoryInfo.h"

#include "data/UITrackInfo.h"
#include "data/UICardInfo.h"
#include "data/UIGroupInfo.h"

#include "data/UIMenuDataInfo.h"
#include <utils/ConstStringHelper.h>

QList<ConstructionDataInterface *> ConstructionData::m_otherConstructionList;

ConstructionData::ConstructionData(void)
{
}


ConstructionData::~ConstructionData(void)
{
}

int ConstructionData::FreeObj( const QString & datatype,void * itemobj )
{
    int ret = -1;
    if(IsIndexOfString(datatype, "LIST"))
    {
        QList<void *> * list = (QList<void *> *)itemobj;
        qDeleteAll(*list);
        list->clear();
        ret = 200;
    }
    else if(IsIndexOfString(datatype, "MAP"))
    {
        QMap<QString,void *> * map = (QMap<QString,void *> *)itemobj;
        for (auto itor = map->begin(); itor != map->end(); itor++)
        {
            void * obj = itor.value();
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
        if( itemobj )
        {
            delete itemobj;
            ret = 200;
        }
    }
    return ret;
}

void * ConstructionData::CreateObj(const QString & datatype,void * itemobj)
{
    void* ret = nullptr;
    if (IsIndexOfString(datatype, ConstStringHelper::m_cUIVISITORINFO))
    {
        ret = new UIVisitorInfo((UIVisitorInfo *)itemobj);
    }
    else if (IsIndexOfString(datatype, ConstStringHelper::m_cUICONTACTINFO))
    {
        ret = new UIContactInfo((UIContactInfo*)itemobj);
    }
    else if (IsIndexOfString(datatype, ConstStringHelper::m_cUICHATINFO))
    {
        ret = new UIChatInfo((UIChatInfo*)itemobj);
    }
    else if (IsIndexOfString(datatype, ConstStringHelper::m_cUIWAITVISITORINFO))
    {
        ret = new UIWaitVisitorInfo((UIWaitVisitorInfo*)itemobj);


    }
    else if (IsIndexOfString(datatype, ConstStringHelper::m_cUICONTACTGROUPINFO))
    {
        ret = new UIContactGroupInfo((UIContactGroupInfo*)itemobj);
    }
    else if (IsIndexOfString(datatype, ConstStringHelper::m_cUIVISITORCOL))
    {
        ret = new UIVisitorCol((UIVisitorCol*)itemobj);
    }
    else if (IsIndexOfString(datatype, ConstStringHelper::m_cCOMMINFO))
    {
        ret = new CommInfo((CommInfo *)itemobj);
    }
    else if (IsIndexOfString(datatype, ConstStringHelper::m_cUIHISTORYINFO))
    {
        ret = new UIHistoryInfo((UIHistoryInfo *)itemobj);

    }
    else if (IsIndexOfString(datatype, ConstStringHelper::m_cUITRACKINFO))
    {
        ret = new UITrackInfo(*(UITrackInfo *)itemobj);
    }
    else if (IsIndexOfString(datatype, ConstStringHelper::m_cUICARDINFO))
    {
        ret = new UICardInfo(*(UICardInfo *)itemobj);
    }
    else if (IsIndexOfString(datatype, ConstStringHelper::m_cUIGROUPINFO))
    {
        ret = new UIGroupInfo((UIGroupInfo *)itemobj);
    }
    else if (IsIndexOfString(datatype, ConstStringHelper::m_cUIMenuDataInfo))
    {
        ret = new UIMenuDataInfo((UIMenuDataInfo*)itemobj);
    }
    else
    {
        foreach ( ConstructionDataInterface * obj ,m_otherConstructionList)
        {
            ret =  obj->CreateObj(datatype,itemobj);
            if( ret ) break;
        }
        //Q_ASSERT(false);
    }
    return ret;
}

bool ConstructionData::IsIndexOfString(QString sourceStr, QString indexStr)
{
    bool isRet = false;
    if (sourceStr.indexOf(indexStr, 0, Qt::CaseInsensitive)!= -1)
    {
        isRet = true;
    }
    return isRet;
}

int ConstructionData::AddObjToList(const QString & datatype,void *itemobj,void * outlist)
{
    int ret = -1;
    if(IsIndexOfString(datatype, ConstStringHelper::m_cLIST))
    {
        if (itemobj)
        {
            QList<void *> * plist = (QList<void *> *)outlist;
            plist->append(itemobj);
        }
        ret = 200;
    }
    else if(IsIndexOfString(datatype, ConstStringHelper::m_cUICONTACTINFO_MEMBERMAP))
    {

        UIContactInfo * info =(UIContactInfo * ) itemobj;
        if (info)
        {
            QMap<QString, UIContactInfo *> * pmap = (QMap<QString, UIContactInfo*> *)outlist;
            pmap->insert( info->UserId(), info);
        }

        ret = 200;
    }
    else if(IsIndexOfString(datatype, ConstStringHelper::m_cUIGROUPINFO_MAP))
    {

        UIGroupInfo * info =(UIGroupInfo * ) itemobj;
        if (info)
        {
            QMap<QString, UIGroupInfo *> * pmap = (QMap<QString, UIGroupInfo*> *)outlist;
            pmap->insert( info->GroupId(), info);
        }

        ret = 200;
    }
    else if(IsIndexOfString(datatype, ConstStringHelper::m_cUICONTACTGROUPINFO_GROUPMAP))
    {
        UIContactGroupInfo * info =(UIContactGroupInfo * ) itemobj;
        if (info)
        {
            QMap<QString, UIContactGroupInfo *> * pmap = (QMap<QString, UIContactGroupInfo*> *)outlist;
            pmap->insert( info->DepartmentId(), info);
        }

        ret = 200;
    }
    else if(IsIndexOfString(datatype, ConstStringHelper::m_cUICHATINFO_CHATMAP))
    {
        UIChatInfo * info =(UIChatInfo * ) itemobj;
        if (info)
        {
            QMap<QString, UIChatInfo *> * pmap = (QMap<QString, UIChatInfo*> *)outlist;
            pmap->insert( info->ChatId(), info);
        }

        ret = 200;
    }
    else if(IsIndexOfString(datatype, ConstStringHelper::m_cUIVISITORINFO_VISITORMAP))
    {
        UIVisitorInfo * info =(UIVisitorInfo * ) itemobj;
        if (info)
        {
            QMap<QString, UIVisitorInfo *> * pmap = (QMap<QString, UIVisitorInfo*> *)outlist;
            pmap->insert( info->Vid(), info);
        }

        ret = 200;
    }
    else if(IsIndexOfString(datatype, ConstStringHelper::m_cUIWAITVISITORINFO_VISITORMAP))
    {
        UIWaitVisitorInfo * info =(UIWaitVisitorInfo * ) itemobj;
        if (info)
        {
            QMap<QString, UIWaitVisitorInfo *> * pmap = (QMap<QString, UIWaitVisitorInfo*> *)outlist;
            pmap->insert( info->UserId(), info);
        }
        ret = 200;

    }
    else
    {
        foreach ( ConstructionDataInterface * obj ,m_otherConstructionList)
        {
            ret =  obj->AddObjToList(datatype,itemobj,outlist);
            if( ret == 200  ) break;
        }
        //Q_ASSERT(false);
        //do nothing。
    }
    return ret;
}

void ConstructionData::AddConstructionExt(ConstructionDataInterface * obj )
{
    if( m_otherConstructionList.indexOf( obj ) < 0 )
        m_otherConstructionList.append(obj);
}
