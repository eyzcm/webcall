#pragma once
#include <QString>
#include <QList>


class  CExtendAIRobotTipInfoItem{
public:
	CExtendAIRobotTipInfoItem();
	CExtendAIRobotTipInfoItem(const CExtendAIRobotTipInfoItem& other);
	~CExtendAIRobotTipInfoItem();

public:

	QString ClientKey() const;
	void ClientKey(QString val);

	QString AIKey() const;
	void AIKey(QString val);
	
	QString ValueType() const;
	void ValueType(QString val);

	QString AIContent() const;
	void AIContent(QString val);
	
	QString Note() const;
	void Note(QString val);
private:
	QString m_clientKey;	//客户端key
	QString m_aiKey;		//ai后台的key
	QString m_aiValueType;	//string 为 1, int 为 2, sum求和累计为3, timeSpec 时间段类型为4.
	QString m_aiContent;	//ai后台传输的内容
	QString m_note;			//备注
};

class  CExtendAIRobotTipInfo
{
public:
	CExtendAIRobotTipInfo(void);

	CExtendAIRobotTipInfo(const CExtendAIRobotTipInfo& other);
	CExtendAIRobotTipInfo(CExtendAIRobotTipInfo* other);
	~CExtendAIRobotTipInfo(void);

private:	
	QList<CExtendAIRobotTipInfoItem*> m_actionItems;

public:
	QList<CExtendAIRobotTipInfoItem*> Items() const;
	void Items(QList<CExtendAIRobotTipInfoItem*> val);
	void AddItem(const CExtendAIRobotTipInfoItem& item);
	CExtendAIRobotTipInfoItem* FindItemByAction(const QString& actionKey);

};

