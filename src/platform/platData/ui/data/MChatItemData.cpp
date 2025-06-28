#include "MChatItemData.h"
 #include "utils/DefineHelper.h" 

MChatItemData::MChatItemData(void)
{
    m_isTakeOver         = false;
    m_chatState          = E_CHATITEM_SETUP;
    m_isNewChat          = false;
    m_inviteMode         = 2;
    m_aiCreate           = 0;
    m_updateCardType     = 1;
    m_removeReserveRobot = 0;

    m_strSearchingType = "";
    m_strAwake         = "";
    m_strAwakeInfo     = "";
    m_strNickName      = "";
    m_chatCloseTime    = QDateTime();
    m_msgtype          = E_MSGTYPE_DEFAULT;
}

MChatItemData::MChatItemData(const MChatItemData& o)
{
    this->ExtendInfo(o.ExtendInfo());
    this->Name(o.Name());
    this->ChatState(o.ChatState());
    this->UserType(o.UserType());
    this->Userid(o.Userid());
    this->Vid(o.Vid());
    this->Chatid(o.Chatid());
    this->TakeOver(o.TakeOver());
    this->NewChat(o.NewChat());
    this->InviteMode(o.InviteMode());
    this->AiCreate(o.AiCreate());
    this->UpdateCardType(o.UpdateCardType());
    this->SearchingType(o.SearchingType());
    this->Awake(o.Awake());
    this->AwakeInfo(o.AwakeInfo());
    this->NickName(o.NickName());
    this->ChatCloseTime(o.ChatCloseTime());
    this->EffectiveAllocate(o.EffectiveAllocate());
    this->RemoveReserveRobot(o.RemoveReserveRobot());
    this->MsgType(o.MsgType());
}

void MChatItemData::UpdataData(const MChatItemData& o)
{
    this->ExtendInfo(o.ExtendInfo());
    this->Name(o.Name());
    this->ChatState(o.ChatState());
    this->UserType(o.UserType());
    this->Userid(o.Userid());
    this->Vid(o.Vid());
    this->Chatid(o.Chatid());
    this->TakeOver(o.TakeOver());
    this->NewChat(o.NewChat());
    this->InviteMode(o.InviteMode());
    this->AiCreate(o.AiCreate());
    this->UpdateCardType(o.UpdateCardType());
    this->SearchingType(o.SearchingType());
    this->Awake(o.Awake());
    this->AwakeInfo(o.AwakeInfo());
    this->NickName(o.NickName());
    this->EffectiveAllocate(o.EffectiveAllocate());
    this->RemoveReserveRobot(o.RemoveReserveRobot());
    this->MsgType(o.MsgType());
}

MChatItemData::~MChatItemData(void)
{
}

void MChatItemData::Chatid(QString val)
{
    m_chatid = val;
}

QString MChatItemData::Chatid() const
{
    return m_chatid;
}

void MChatItemData::Vid(QString val)
{
    m_vid = val;
}

QString MChatItemData::Vid() const
{
    return m_vid;
}

void MChatItemData::Userid(QString val)
{
    m_userid = val;
}

QString MChatItemData::Userid() const
{
    return m_userid;
}

void MChatItemData::UserType(EUSERTYPE val)
{
    m_userType = val;
}

EUSERTYPE MChatItemData::UserType() const
{
    return m_userType;
}

void MChatItemData::ChatState(ECHATITEMSTATE val)
{
    // zcmadd 是否有问题
    if (m_chatState == val)
    {

        Q_EMIT S_OnChatStateChanged(m_chatState, val);
    }
    m_chatState = val;

    if (val == E_CHATITEM_LEAVE)
    {
        m_chatCloseTime = QDateTime::currentDateTime();
    }
}

ECHATITEMSTATE MChatItemData::ChatState() const
{
    return m_chatState;
}

void MChatItemData::Name(QString val)
{
    if (StringCompare(m_name, val))
    {
        Q_EMIT S_OnNameChanged(m_name, val);
    }
    m_name = val;
}

QString MChatItemData::Name() const
{
    if(!GetExtendData("cardName").isEmpty())
    {
        return GetExtendData("cardName");
    }
    if(!NickName().isEmpty())
    {
        return NickName();
    }
    return m_name;
}

void MChatItemData::InitData(QString vid, QString userid, QString chatid, QString name, EUSERTYPE eUserType, ECHATITEMSTATE eState)
{
    m_vid       = vid;
    m_userType  = eUserType;
    m_userid    = userid;
    m_chatid    = chatid;
    m_chatState = eState;
    m_name      = name;
}

void MChatItemData::InitData(MMonitorChatData data)
{
    this->ExtendInfo(data.VisitorInfoMap());
    this->Name(data.Name());
    this->ChatState(E_CHATITEM_MONITOR_CHAT);
    this->UserType(E_MONITOR);
    this->Userid(data.Userid());
    this->Vid(data.Vid());
    this->Chatid(data.ChatId());
    this->SearchingType(data.SearchingType());
    this->Awake(data.Awake());
    this->AwakeInfo(data.AwakeInfo());
    this->NickName(data.NickName());
    this->ChatCloseTime(data.ChatCloseTime());
    this->MsgType(E_MSGTYPE_DEFAULT);

    this->AddExtendData("ip", data.GetUIChatInfoValueByKey("ip"));
    this->AddExtendData("CustomerID", data.CustomerId());
    this->AddExtendData("TokeOverID", "");
    this->AddExtendData("CustomerName", data.CustomerName());
    this->AddExtendData("VisitorMsgCount", data.GetUIChatInfoValueByKey("visitorMsgCount"));
    this->AddExtendData("UserMsgCount", data.GetUIChatInfoValueByKey("userMsgCount"));
    this->AddExtendData("Device", data.GetUIChatInfoValueByKey("device"));
    this->AddExtendData("xst", data.GetUIChatInfoValueByKey("xst"));
    this->AddExtendData("SearchEngineId", data.GetUIChatInfoValueByKey("SearchEngineId"));
    this->AddExtendData("AiChatFlag", data.GetUIChatInfoValueByKey("AiChatFlag"));
}

bool MChatItemData::TakeOver() const
{
    return m_isTakeOver;
}
void MChatItemData::TakeOver(bool val)
{
    m_isTakeOver = val;
}

bool MChatItemData::NewChat() const
{

    return m_isNewChat;
}

void MChatItemData::NewChat(bool val)
{
    m_isNewChat = val;
}

int MChatItemData::InviteMode() const
{

    return m_inviteMode;
}

void MChatItemData::InviteMode(int val)
{
    m_inviteMode = val;
}

bool MChatItemData::IsFindVisitorByKey(QString key, QString type)
{
    bool isFind = false;
    do
    {
        if (Name().contains(key, Qt::CaseInsensitive))
        {
            isFind = true;
            break;
        }
        if (NickName().contains(key, Qt::CaseInsensitive))
        {
            isFind = true;
            break;
        }
        if (Chatid().contains(key, Qt::CaseInsensitive))
        {
            isFind = true;
            break;
        }
        if (Vid().contains(key, Qt::CaseInsensitive))
        {
            isFind = true;
            break;
        }
        if (GetExtendData("keyword").contains(key, Qt::CaseInsensitive))
        {
            isFind = true;
            break;
        }
        if (type == "card")
        {
            if (GetExtendData("hasCard") == "1")
            {
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
        if (GetExtendData("cardName").contains(key, Qt::CaseInsensitive))
        {
            isFind = true;
            break;
        }
        if (GetExtendData("cardTel").contains(key, Qt::CaseInsensitive))
        {
            isFind = true;
            break;
        }
        if (GetExtendData("cardMobile").contains(key, Qt::CaseInsensitive))
        {
            isFind = true;
            break;
        }
        if (GetExtendData("cardNote").contains(key, Qt::CaseInsensitive))
        {
            isFind = true;
            break;
        }
        if (GetExtendData("cardWX").contains(key, Qt::CaseInsensitive))
        {
            isFind = true;
            break;
        }
        if (GetExtendData("cardQQ").contains(key, Qt::CaseInsensitive))
        {
            isFind = true;
            break;
        }

    } while (false);

    return isFind;
}

bool MChatItemData::IsFindCustomerByKey(QString key)
{
    bool isFind = false;
    do
    {
        if (Name().contains(key, Qt::CaseInsensitive))
        {
            isFind = true;
            break;
        }
        if (Userid().contains(key, Qt::CaseInsensitive))
        {
            isFind = true;
            break;
        }
        if (GetExtendData("outName").contains(key, Qt::CaseInsensitive))
        {
            isFind = true;
            break;
        }

    } while (false);

    return isFind;
}

int MChatItemData::AiCreate() const
{
    return m_aiCreate;
}
void MChatItemData::AiCreate(int val)
{
    m_aiCreate = val;
}

int MChatItemData::UpdateCardType() const
{
    return m_updateCardType;
}

void MChatItemData::UpdateCardType(int val)
{
    m_updateCardType = val;
}

QString MChatItemData::SearchingType() const
{
    return m_strSearchingType;
}
void MChatItemData::SearchingType(QString val)
{
    m_strSearchingType = val;
}

QString MChatItemData::Awake() const
{
    return m_strAwake;
}
void MChatItemData::Awake(QString val)
{
    m_strAwake = val;
}

QString MChatItemData::AwakeInfo() const
{
    return m_strAwakeInfo;
}
void MChatItemData::AwakeInfo(QString val)
{
    m_strAwakeInfo = val;
}

QString MChatItemData::NickName() const
{
    return m_strNickName;
}
void MChatItemData::NickName(QString val)
{
    m_strNickName = val;
}

QDateTime MChatItemData::ChatCloseTime() const
{
    return m_chatCloseTime;
}

void MChatItemData::ChatCloseTime(QDateTime val)
{
    m_chatCloseTime = val;
}

QString MChatItemData::EffectiveAllocate() const
{
    return m_effectiveAllocate;
}
void MChatItemData::EffectiveAllocate(QString val)
{
    m_effectiveAllocate = val;
}

void MChatItemData::RemoveReserveRobot(int val)
{
    m_removeReserveRobot = val;
}

int MChatItemData::RemoveReserveRobot() const
{
    return m_removeReserveRobot;
}
