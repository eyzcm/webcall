#ifndef DISPATCHERHELPER_H
#define DISPATCHERHELPER_H

#include <QObject>
#include "include/json/value.h"
#include "EnumDefine.h"

class DispatcherHelper : public QObject
{
	Q_OBJECT

public:
	~DispatcherHelper();

	static DispatcherHelper* GetInstance();
private:
	DispatcherHelper();

private:
	static DispatcherHelper* m_instance;


public:
	void Init();
	void Clear();
	void ReTryConnect();
	void ConnectSignal();
	
signals:
	void S_connected();
	void S_NetWorkError(int errorCode,QString msg);
	void S_MessageParsingFailed();

public:
	void SendQuitSameVisitorChatMessageCmd(const QString& chatid, const QString& visitorId);

	void SendCommChatMessageCmd(const QString& chatid, ECHATACTION action);

	
private slots:
	void R_Received_Im_Message(QString body);
	void R_Received_Im_Group_Message(QString body);
	void R_Received_Im_Group_Event(QString body);
	void R_Received_Im_Queue(QString body);
	void R_Received_Invite_Response(QString body);
	void R_ForceLogout();
private:
	void Received_Im_Message(QString body);
	void Received_Im_Group_Message(QString body);
	void Received_Im_Group_Event(QString body);
	void ParseCommonMessage(const Json::Value& val);
	void ParseQuitChatMessage(const Json::Value& val);
	void ParseTransMessage(const Json::Value& val);
	void ParseLeaveMessage(const Json::Value& val);
	void ParseFreePhoneMessage(const Json::Value& val);	
	void ParseCreateChatMessage(const Json::Value& val);
	void ParsePopMessage(const Json::Value& val);
	void Received_Im_Queue(QString body);
	void Received_Invite_Response(const QString& body);
	
};

#define gDispatcherHelper DispatcherHelper::GetInstance()
#endif // DISPATCHERHELPER_H
