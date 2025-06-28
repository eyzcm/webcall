#pragma once
#include "datafactory/ConstructionData.h"
#include "data/MChatItemData.h"
#include "data/UIChatInfo.h"
#include "data/UIContactGroupInfo.h"
#include "data/UIContactInfo.h"
#include "data/UIVisitorInfo.h"
#include "data/UIWaitVisitorInfo.h"
#include "data/MContactItemData.h"


//////////////////////////////////////////////////////////////////////////
// author:familyszy
// time: 2019-4-11
// datatype命名规则，
// 1.不能与已有的命名重复。
// 2.返回List类型时，datatype为单个类型后面携带"-LIST"
// 3.返回是Map类型时，datatype如果为默认类型，这用默认常量，如果不是默认类型，这使用单个类型后面携带"-MAP".
//
//////////////////////////////////////////////////////////////////////////

#include "plat_interface_global.h"
class PLAT_INTERFACE_EXPORT UIBaseFactory : public ConstructionData
{
public:
    UIBaseFactory(void);
    ~UIBaseFactory(void);

    virtual void* CreateObj(const QString& datatype, void* itemobj);
    virtual int   AddObjToList(const QString& datatype, void* itemobj, void* outlist);
    static void AddConstructionExt(ConstructionDataInterface * obj );

public:
    MChatItemData* BuilderChatItemData(UIWaitVisitorInfo* info);
    MChatItemData* BuilderChatItemData(UIContactInfo* info);
    MChatItemData* BuilderChatItemData(UIVisitorInfo* info);
    MChatItemData* BuilderChatItemData(UIChatInfo* info, ECHATITEMSTATE state);
    MChatItemData* BuilderChatItemData(UIChatInfo* info);
    MChatItemData* CreateMChatItemData(const UIChatInfo& info);
    MChatItemData* CreateMChatItemData(const UIChatInfo& info, ECHATITEMSTATE state);
    MChatItemData* CreateMChatItemData(const UIVisitorInfo& info);
    MChatItemData* CreateMChatItemData(const UIContactInfo& info);
    MChatItemData* CreateMChatItemData(const UIWaitVisitorInfo& info);

    MContactItemData* BuilderContactItemData(const UIVisitorInfo& info, EUINODEMODELTYPE type);
    MContactItemData* BuilderContactItemData(const UIContactGroupInfo& info, EUINODEMODELTYPE type);
    MContactItemData* BuilderContactItemData(const UIChatInfo& info, EUINODEMODELTYPE type);
    MContactItemData* BuilderContactItemData(const UIWaitVisitorInfo& info, EUINODEMODELTYPE type);
    MContactItemData* BuilderContactItemData(const UIContactInfo& info, EUINODEMODELTYPE type);


};
