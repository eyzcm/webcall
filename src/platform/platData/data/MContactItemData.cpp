#include "MContactItemData.h"
 #include "utils/DefineHelper.h"

MContactItemData::MContactItemData(void)
{
    m_noteType         = E_UI_MODEL_NONE;
    m_treeuuid         = "";
    m_inviteMode       = 0;
    m_strSearchingType = "";
    m_strAwake         = "";
    m_strAwakeInfo     = "";
    m_strNickName      = "";
}

MContactItemData::MContactItemData(const MContactItemData& o)
{
    this->ExtendInfo(o.ExtendInfo());
    this->Name(o.Name());
    this->NoteType(o.NoteType());
    this->Groupid(o.Groupid());
    this->DepartmentId(o.DepartmentId());
    this->Userid(o.Userid());
    this->Vid(o.Vid());
    this->SetUUid(o.GetUUid());
    this->Chatid(o.Chatid());
    this->InviteMode(o.InviteMode());
    this->SearchingType(o.SearchingType());
    this->Awake(o.Awake());
    this->AwakeInfo(o.AwakeInfo());
    this->NickName(o.NickName());
}

MContactItemData::~MContactItemData(void)
{
}

void MContactItemData::Chatid(QString val)
{
    m_chatid = val;
}

QString MContactItemData::Chatid() const
{
    return m_chatid;
}

void MContactItemData::Vid(QString val)
{
    m_vid = val;
}

QString MContactItemData::Vid() const
{
    return m_vid;
}

void MContactItemData::Userid(QString val)
{
    m_userid = val;
}

QString MContactItemData::Userid() const
{
    return m_userid;
}

void MContactItemData::Name(QString val)
{
    if (StringCompare(m_name, val))
    {
        Q_EMIT S_OnNameChanged(m_name, val);
    }
    m_name = val;
}

QString MContactItemData::Name() const
{
    return m_name;
}

void MContactItemData::InitData(QString strUUId, QString vid, QString userid, QString chatid, QString name, QString groupid, QString departmentId, EUINODEMODELTYPE etype)
{
    m_vid          = vid;
    m_noteType     = etype;
    m_userid       = userid;
    m_chatid       = chatid;
    m_groupid      = groupid;
    m_name         = name;
    m_departmentId = departmentId;

    m_treeuuid     = strUUId;
}

void MContactItemData::Groupid(QString val)
{
    m_groupid = val;
}

QString MContactItemData::Groupid() const
{
    return m_groupid;
}

void MContactItemData::NoteType(EUINODEMODELTYPE val)
{
    m_noteType = val;
}

EUINODEMODELTYPE MContactItemData::NoteType() const
{
    return m_noteType;
}

void MContactItemData::SetUUid(QString val)
{

    m_treeuuid = val;
}

QString MContactItemData::GetUUid() const
{
    return m_treeuuid;
}

void MContactItemData::DepartmentId(QString val)
{
    m_departmentId = val;
}

QString MContactItemData::DepartmentId() const
{
    return m_departmentId;
}

int MContactItemData::InviteMode() const
{

    return m_inviteMode;
}

void MContactItemData::InviteMode(int val)
{
    m_inviteMode = val;
}

QString MContactItemData::SearchingType() const
{
    return m_strSearchingType;
}
void MContactItemData::SearchingType(QString val)
{
    m_strSearchingType = val;
}

QString MContactItemData::Awake() const
{
    return m_strAwake;
}
void MContactItemData::Awake(QString val)
{
    m_strAwake = val;
}

QString MContactItemData::AwakeInfo() const
{
    return m_strAwakeInfo;
}
void MContactItemData::AwakeInfo(QString val)
{
    m_strAwakeInfo = val;
}

QString MContactItemData::NickName() const
{
    return m_strNickName;
}
void MContactItemData::NickName(QString val)
{
    m_strNickName = val;
}
