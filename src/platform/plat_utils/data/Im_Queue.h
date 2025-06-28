#pragma once
#include "data/CommInfo.h"
#include "utils/DefineHelper.h"


class  Im_UserPresence : public CommInfo
{
public:
    Im_UserPresence(void){};
    ~Im_UserPresence(void){};
};

class  Im_VisitorInfo : public CommInfo
{
public:
    Im_VisitorInfo(void){};
    ~Im_VisitorInfo(void){};
    QString GetDisplayName() const;

    QString GetVisitorLocation() const;
};

class Im_Queue
{
public:
    Im_Queue(void);
    Im_Queue(const Im_Queue& other);
    ~Im_Queue(void);

public:
    //客服所在分组ID
    Property_QString(GroupId, groupId);
    //队列标识
    Property_QString(QueueAction, action);
    //访客本次标识
    Property_QString(From, from);
    Property_QString(QueueType, type);

public:
    Im_VisitorInfo* GetVisitorInfo() const
    {
        return m_imVisitorInfo;
    }
    void SetVisitorInfo(const Im_VisitorInfo& val)
    {
        m_imVisitorInfo = new Im_VisitorInfo(val);
    }

private:
    Im_VisitorInfo* m_imVisitorInfo;
};
