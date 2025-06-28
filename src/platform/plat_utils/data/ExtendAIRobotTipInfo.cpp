#include "ExtendAIRobotTipInfo.h"


CExtendAIRobotTipInfo::CExtendAIRobotTipInfo(void)
{
	
}


CExtendAIRobotTipInfo::CExtendAIRobotTipInfo(const CExtendAIRobotTipInfo& other)
{
	for(int i = 0; i< other.Items().size(); i++)
	{
		CExtendAIRobotTipInfoItem* obj = new CExtendAIRobotTipInfoItem(*other.Items()[i]);
		m_actionItems.push_back(obj);
	}
}

CExtendAIRobotTipInfo::CExtendAIRobotTipInfo(CExtendAIRobotTipInfo* pother)
{
	for(int i = 0; i< pother->Items().size(); i++)
	{
		CExtendAIRobotTipInfoItem* obj = new CExtendAIRobotTipInfoItem(*pother->Items()[i]);
		m_actionItems.push_back(obj);
	}

}

CExtendAIRobotTipInfo::~CExtendAIRobotTipInfo(void)
{
	qDeleteAll(m_actionItems);	
	m_actionItems.clear();
}

void CExtendAIRobotTipInfo::Items(QList<CExtendAIRobotTipInfoItem*> val)
{
	m_actionItems = val;
}

QList<CExtendAIRobotTipInfoItem*> CExtendAIRobotTipInfo::Items() const
{
	return m_actionItems;
}

void CExtendAIRobotTipInfo::AddItem(const CExtendAIRobotTipInfoItem& item)
{
	CExtendAIRobotTipInfoItem* itemObj = new CExtendAIRobotTipInfoItem(item);
	m_actionItems.push_back(itemObj);
}

CExtendAIRobotTipInfoItem* CExtendAIRobotTipInfo::FindItemByAction(const QString& actionKey)
{
	CExtendAIRobotTipInfoItem* ret = nullptr;
	if (!actionKey.isEmpty())
	{
		Q_FOREACH(CExtendAIRobotTipInfoItem* item, m_actionItems){
			if (item)
			{
				if (item->AIKey() == actionKey)
				{
					ret = item;
					break;
				}
			}
		}
	}
	return ret;
}


CExtendAIRobotTipInfoItem::CExtendAIRobotTipInfoItem()
{
	m_clientKey = "";
	m_aiKey = "";
	m_aiValueType = "1";
	m_aiContent = "";
	m_note = "";
}
CExtendAIRobotTipInfoItem::CExtendAIRobotTipInfoItem(const CExtendAIRobotTipInfoItem& other)
{
	this->ClientKey( other.ClientKey());
	this->AIKey( other.AIKey() );
	this->ValueType( other.ValueType() );
	this->AIContent( other.AIContent() );
	this->Note( other.Note() );
	
}

CExtendAIRobotTipInfoItem::~CExtendAIRobotTipInfoItem()
{

}

void CExtendAIRobotTipInfoItem::Note(QString val)
{
	m_note = val;
}

QString CExtendAIRobotTipInfoItem::Note() const
{
	return m_note;
}

void CExtendAIRobotTipInfoItem::AIContent(QString val)
{
	m_aiContent = val;
}

QString CExtendAIRobotTipInfoItem::AIContent() const
{
	return m_aiContent;
}

void CExtendAIRobotTipInfoItem::ValueType(QString val)
{
	m_aiValueType = val;
}

QString CExtendAIRobotTipInfoItem::ValueType() const
{
	return m_aiValueType;
}

void CExtendAIRobotTipInfoItem::AIKey(QString val)
{
	m_aiKey = val;
}

QString CExtendAIRobotTipInfoItem::AIKey() const
{
	return m_aiKey;
}

void CExtendAIRobotTipInfoItem::ClientKey(QString val)
{
	m_clientKey = val;
}

QString CExtendAIRobotTipInfoItem::ClientKey() const
{
	return m_clientKey;
}
