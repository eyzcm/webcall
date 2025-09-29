#include "dispatcherhelper.h"
#include "tcpagent.h"
#include "gcontrol.h"
#include "jsonhelper.h"
#include "utilshelper.h"
#include "conversationmanager.h"
#include "CommChatMessageCmd.h"
#include "colleaguemanager.h"
#include "contactmanager.h"

DispatcherHelper::DispatcherHelper()
	: QObject()
{

}

DispatcherHelper* DispatcherHelper::m_instance = NULL;

void DispatcherHelper::Init()
{
	gTcpAgent->Clear();
	ConnectSignal();
	gTcpAgent->Init();	
}

void DispatcherHelper::Clear()
{
	
}

void DispatcherHelper::ReTryConnect()
{
	gTcpAgent->Clear();
	ConnectSignal();
	gTcpAgent->TcpReconnect();
}

void DispatcherHelper::SendQuitSameVisitorChatMessageCmd(const QString& chatid, const QString& visitorId)
{
	CommChatMessageCmd req;
	req.ChatAction(E_ACTION_QUITSAMEVISITOR);
	req.ChatId(chatid);
	req.VisitorId(visitorId);
	req.MakeCmd();
	req.SendRequestToServer();
}

void DispatcherHelper::R_Received_Im_Message(QString body)
{
	Received_Im_Message(body);
}

void DispatcherHelper::R_Received_Im_Group_Message(QString body)
{
	Received_Im_Group_Message(body);
}

void DispatcherHelper::R_Received_Im_Group_Event(QString body)
{
	Received_Im_Group_Event(body);
}

void DispatcherHelper::R_Received_Im_Queue(QString body)
{
	Received_Im_Queue(body);
}

void DispatcherHelper::R_Received_Invite_Response(QString body)
{
	Received_Invite_Response(body);
}

void DispatcherHelper::R_ForceLogout()
{
	gControl->ForceLogout();	
}

DispatcherHelper::~DispatcherHelper()
{
	if (m_instance)
	{
		m_instance->deleteLater();
		m_instance = NULL;
	}
}

DispatcherHelper* DispatcherHelper::GetInstance()
{
	if (m_instance== NULL)
	{
		m_instance = new DispatcherHelper;
	}
	return m_instance;
}




void DispatcherHelper::ConnectSignal()
{
	connect(gTcpAgent, SIGNAL(S_connected()), this, SIGNAL(S_connected()));
	connect(gTcpAgent, SIGNAL(S_MessageParsingFailed()), this, SIGNAL(S_MessageParsingFailed()));
	connect(gTcpAgent, SIGNAL(S_NetWorkError(int,QString)), this, SIGNAL(S_NetWorkError(int,QString)));
	connect(gTcpAgent, SIGNAL(S_Received_Im_Message(QString)), this, SLOT(R_Received_Im_Message(QString)));
	connect(gTcpAgent, SIGNAL(S_Received_Im_Group_Message(QString)), this, SLOT(R_Received_Im_Group_Message(QString)));
	connect(gTcpAgent, SIGNAL(S_Received_Im_Group_Event(QString)), this, SLOT(R_Received_Im_Group_Event(QString)));
	connect(gTcpAgent, SIGNAL(S_Received_Im_Queue(QString)), this, SLOT(R_Received_Im_Queue(QString)));
	connect(gTcpAgent, SIGNAL(S_Received_Invite_Response(QString)), this, SLOT(R_Received_Invite_Response(QString)));
	connect(gTcpAgent, SIGNAL(S_ForceLogout()), this, SLOT(R_ForceLogout()));

}

void DispatcherHelper::Received_Im_Message(QString body)
{

	Json::Value val;
	bool isOK = JsonHelper::GetJsonValueFromBody(body, val);
	if (isOK)
	{
		QString strType = JsonHelper::GetValueByKey("type",val);
		EMESSAGETYPE eMsgType = UtilsHelper::GetMessageTypeByString(strType);

		switch(eMsgType)
		{
		case E_MSGTYPE_DEFAULT:
			//Q_ASSERT(false);
			break;
		case E_MSGTYPE_EVENT_CLOSE:
			ParseQuitChatMessage(val);			
			break;
		case E_MSGTYPE_EVENT_END:
			ParseQuitChatMessage(val);
			break;
		case E_MSGTYPE_AUTOACCEPTINVITE:
			//5.4	创建对话
			ParseCreateChatMessage(val);
			break;
		case E_MSGTYPE_COMPOSING:
			ParseCommonMessage(val);
			break;
		case E_MSGTYPE_DENY:
			ParseCommonMessage(val);
			break;
		case E_MSGTYPE_FILE:
			ParseCommonMessage(val);
			break;
		case E_MSGTYPE_MESSAGE:
			ParseCommonMessage(val);
			break;
		case E_MSGTYPE_MONITORSUCCESS:
			ParseCommonMessage(val);
			break;
		case E_MSGTYPE_OPINION:
			ParseCommonMessage(val);
			break;
		case E_MSGTYPE_SCREENSHOTS:
			ParseCommonMessage(val);
			break;
		case E_MSGTYPE_TRANS:
			ParseTransMessage(val);
			break;

		case E_MSGTYPE_LEAVE_MSG:
			ParseLeaveMessage(val);
			break;
		case E_MSGTYPE_FREE_PHONE:
			ParseFreePhoneMessage(val);
			break;
		case E_EVENT_POP_MESSAGE:
			ParsePopMessage(val);
		default:
			Q_ASSERT(false);
			break;
		}				
	}
}

void DispatcherHelper::Received_Im_Group_Message(QString body)
{
	Json::Value val;
	bool isOK = JsonHelper::GetJsonValueFromBody(body, val);
	if (isOK)
	{
		QString strType = JsonHelper::GetValueByKey("type",val);
		EMESSAGETYPE eMsgType = UtilsHelper::GetMessageTypeByString(strType);

		switch(eMsgType)
		{
		case E_MSGTYPE_MESSAGE:
			{
				QMap<QString, QString> tempMap;	
				bool isOK = JsonHelper::ParseValueToMap(val,tempMap);	
				if (isOK)
				{
					Im_Message msg;
					msg.AddCommInfo(tempMap);
					gControl->DoReceiveChatGroupMessage(msg);		
				}	
			}
			
			break;
		case E_MSGTYPE_FILE:
			{
				QMap<QString, QString> tempMap;	
				bool isOK = JsonHelper::ParseValueToMap(val,tempMap);	
				if (isOK)
				{
					Im_Message msg;
					msg.AddCommInfo(tempMap);
					gControl->DoReceiveChatGroupMessage(msg);		
				}	
			}
			break;
		case E_MSGTYPE_SCREENSHOTS:
			{
				QMap<QString, QString> tempMap;	
				bool isOK = JsonHelper::ParseValueToMap(val,tempMap);	
				if (isOK)
				{
					Im_Message msg;
					msg.AddCommInfo(tempMap);
					gControl->DoReceiveChatGroupMessage(msg);		
				}	
			}
			break;
		default:
			break;
		}
	}
}

void DispatcherHelper::Received_Im_Group_Event(QString body)
{
	Json::Value val;
	bool isOK = JsonHelper::GetJsonValueFromBody(body, val);
	if (isOK)
	{
		QString strType = JsonHelper::GetValueByKey("event",val);
		EGROUPEVENTTYPE eMsgType = UtilsHelper::GetGroupEventTypeByString(strType);

		//todo shizhenyu .
		QMap<QString, QString> tempMap;	
		bool isOK = JsonHelper::ParseValueToMap(val,tempMap);	
		if (isOK)
		{
			Im_Message msg;
			msg.AddCommInfo(tempMap);
			//gControl->DoReceiveChatGroupMessage(msg);		
		}	
	}
}

void DispatcherHelper::ParseCommonMessage(const Json::Value& val)
{	
	QMap<QString, QString> tempMap;	
	bool isOK = JsonHelper::ParseValueToMap(val,tempMap);	
	if (isOK)
	{
		Im_Message msg;
		msg.AddCommInfo(tempMap);
		gControl->DoReceiveMessage(msg);		
	}	
}

void DispatcherHelper::ParseQuitChatMessage(const Json::Value& val)
{
	QMap<QString, QString> tempMap;	
	bool isOK = JsonHelper::ParseValueToMap(val,tempMap);	
	if (isOK)
	{
		Im_Message msg;
		msg.AddCommInfo(tempMap);
		gControl->DoReceiveMessage(msg);	
		gConversationManager->RemoveChatByChatId(msg.GetValueByKey("chatId"));
	}	
}

void DispatcherHelper::ParseTransMessage(const Json::Value& val)
{
	QMap<QString, QString> tempMap;
	bool isParse = JsonHelper::ParseValueToMap(val, tempMap);
	if (isParse)
	{
		ConversationTransData info;
		CommInfo attrInfo;
		attrInfo.AddCommInfo(tempMap);
		info.SetAttrInfo(attrInfo);

		QMap<QString, QString> exMap;
		bool isEx = JsonHelper::ParseValueToMap(val["extendInfos"], exMap);
		if (isEx)
		{
			CommInfo ex;
			ex.AddCommInfo(exMap);
			info.SetExtendInfo(ex);
		}

		gConversationManager->CreateTransferChat(info);
		gControl->DoCreateTransferChat(info);
		//发送回执。
		SendCommChatMessageCmd(info.GetChatId(), E_Action_TRANS);
	}
}

void DispatcherHelper::SendCommChatMessageCmd(const QString& chatid, ECHATACTION action)
{
	CommChatMessageCmd req;
	req.ChatAction(action);
	req.ChatId(chatid);
	req.MakeCmd();
	req.SendRequestToServer();
}
void DispatcherHelper::ParseLeaveMessage(const Json::Value& val)
{
	//{
	//	"exts": {
	//		"message": "留言内容",		//留言内容
	//			"phone": "18511989099",	//客户电话
	//			"email": null,				//客户邮箱 电话和邮箱二选一
	//			"subject": "留言主题",		//留言主题
	//			"messageId": 74821		//数据库主键
	//	},
	//	"colleagueMessage": false,
	//	"type": "EVENT_LEAVE_MSG",	//消息类型
	//	"fromUserId": "01000000000100244827921216831194"	//访客ID
	//}


	QMap<QString, QString> tempMap;
	bool isParse = JsonHelper::ParseValueToMap(val, tempMap);
	if (isParse)
	{	
		QMap<QString, QString> exMap;
		bool isEx = JsonHelper::ParseValueToMap(val["exts"], exMap);
		if (isEx)
		{
			CommInfo ex;
			ex.AddCommInfo(exMap);

			CommInfo attr;
			attr.AddCommInfo(tempMap);
			gControl->NotifyLeaveMessage(attr,ex);	
		}

	}
}

void DispatcherHelper::ParseFreePhoneMessage(const Json::Value& val)
{
	//{
	//	"exts": {
	//		"phone": "18511989099",		//电话
	//			"freePhoneId": 1042			//数据库主键
	//	},
	//	"colleagueMessage": false,
	//	"type": "EVENT_FREE_PHONE",		//消息类型
	//	"fromUserId": "01000000000100000355708505481155"  //访客ID
	//}
	QMap<QString, QString> tempMap;
	bool isParse = JsonHelper::ParseValueToMap(val, tempMap);
	if (isParse)
	{	
		QMap<QString, QString> exMap;
		bool isEx = JsonHelper::ParseValueToMap(val["exts"], exMap);
		if (isEx)
		{
			CommInfo ex;
			ex.AddCommInfo(exMap);

			CommInfo attr;
			attr.AddCommInfo(tempMap);

			gControl->NotifyFreePhoneMessage(attr,ex);
		}

	}
}

void DispatcherHelper::ParseCreateChatMessage(const Json::Value& val)
{
	//产生正式对话
	QMap<QString, QString> tempMap;
	bool isParse = JsonHelper::ParseValueToMap(val, tempMap);
	if (isParse)
	{

		ConversationInfo info;
		CommInfo attrInfo;
		attrInfo.AddCommInfo(tempMap);
		info.SetAttrConversationInfo(attrInfo);

		ConversationExtendInfos extendInfo;
		QMap<QString, QString> attrExtendInfoMap;
		bool isExtendInfo = JsonHelper::ParseValueToMap(val["extendInfos"],attrExtendInfoMap);
		if (isExtendInfo)
		{
			CommInfo attrExtendInfo;
			attrExtendInfo.AddCommInfo(attrExtendInfoMap);
			extendInfo.SetAttrExtendInfos(attrExtendInfo);

			QMap<QString, QString> attrViewInfoMap;
			bool isViewInfo = JsonHelper::ParseValueToMap(val["extendInfos"]["viewInfo"],attrViewInfoMap);
			if (isViewInfo)
			{
				CommInfo attrViewInfo;
				attrViewInfo.AddCommInfo(attrViewInfoMap);
				extendInfo.SetConversationViewInfo(attrViewInfo);
			}
		}
		info.SetConversationExtendInfos(extendInfo);
		info.IsTempConversation(false);
		//正式会话，
		gConversationManager->CreateTempChat(info);

		gControl->DoCreateTempChat(info);
	}
}

void DispatcherHelper::ParsePopMessage(const Json::Value& val)
{
	//{
	//	"message":"您有一个名片等待处理",   //弹出消息内容
	//		"exts":{
	//			"url":http://",            //点击弹出内容
	//			"title":"名片分配"    //标题
	//	},"colleagueMessage":false,
	//	"type":"EVENT_POP_MESSAGE" //类型
	//} 
	QMap<QString, QString> tempMap;
	bool isParse = JsonHelper::ParseValueToMap(val, tempMap);
	if (isParse)
	{	
		QMap<QString, QString> exMap;
		bool isEx = JsonHelper::ParseValueToMap(val["exts"], exMap);
		if (isEx)
		{
			CommInfo ex;
			ex.AddCommInfo(exMap);

			CommInfo attr;
			attr.AddCommInfo(tempMap);
			gControl->NotifyPopMessage(attr,ex);	
		}

	}

}

void DispatcherHelper::Received_Im_Queue(QString body)
{

	Json::Value val;
	bool isOk = JsonHelper::GetJsonValueFromBody(body, val);
	if (isOk)
	{
		QString strType = JsonHelper::GetValueByKey("type",val);
		if (strType == "presence")
		{
			/*
			{
			"client": "WEB",
			"status": 1,
			"runningStatus": "3",
			"from": "zhuqingxiang",
			"type": "presence",
			"userType": "customer"
			}
			*/
			Im_UserPresence msg;
			QMap<QString, QString> tempMap;
			bool isParse = JsonHelper::ParseValueToMap(val,tempMap);
			msg.AddCommInfo(tempMap);
			QString status = msg.GetValueByKey("status");

			QString runningStatus = msg.GetValueByKey("runningStatus");
			//客服状态变化
			EPRESENCESTATUS eStatus = UtilsHelper::GetPressenceStatus(status, runningStatus);					
			EPRESENCESTATUS eOldStatus = gColleagueManager->GetPresenceStatusByID(msg.GetValueByKey("from"));
			if (eStatus != eOldStatus)
			{
				gColleagueManager->CustomerPresenceChanged(msg);
				gControl->CustomerPresenceChanged(msg);	
			}
		}
		else if (strType == "queue")
		{
			QString action = JsonHelper::GetValueByKey("action",val);
			if (action == "new")
			{
				//5.1	访客进入排队消息
				////todo shizhenyu 
				Im_Queue msg;
				msg.From(JsonHelper::GetValueByKey("from", val));
				msg.GroupId(JsonHelper::GetValueByKey("groupId", val));
				msg.QueueAction(action);
				msg.QueueType(strType);
				Json::Value visitorVal = val["visitorInfo"];
				QMap<QString, QString> tempMap;
				bool isParse = JsonHelper::ParseValueToMap(visitorVal, tempMap);
				if (isParse)
				{
					Im_VisitorInfo visitorInfo;
					visitorInfo.AddCommInfo(tempMap);
					msg.SetVisitorInfo(visitorInfo);
				}
				gControl->WaitQueueVisitorChanged(msg);
				gContactManager->WaitQueueVisitorChanged(msg);				
			}
			else if (action == "remove")
			{
				//todo shizhenyu 
				//5.2	退出队列
				Im_Queue msg;
				msg.From(JsonHelper::GetValueByKey("from", val));
				msg.GroupId(JsonHelper::GetValueByKey("groupId", val));
				msg.QueueAction(action);
				msg.QueueType(strType);
				gControl->WaitQueueVisitorChanged(msg);
				gContactManager->WaitQueueVisitorChanged(msg);
			}
		}
		else if (strType == "customerInfoChanged")
		{
			//do nothing.
		}
		else
		{
			Q_ASSERT(false);
		}
	}
}

void DispatcherHelper::Received_Invite_Response(const QString& body)
{
	/*{
		"extendInfos": {
			"chat_url": "http:\/\/www.easyliao.cn\/",
				"refer_page_url": null,
				"searching": null,
				"visitor_ip": "124.65.159.146",
				"visitor_name": null,
				"visitor_location_country": "中国北京市联通",
				"keyWord": null
		},
		"viewUserId": "01000000000100068883400929378384",
		"type": "invite",
		"chatId": -6353,
		"viewId": "01000000000100808941750879699959"
	}*/

	Json::Value val;
	bool isOK = JsonHelper::GetJsonValueFromBody(body, val);
	if (isOK)
	{
		QString strType = JsonHelper::GetValueByKey("type",val);
		if (strType == "invite")
		{
			//主动邀请会话
			//产生临时会话
			ConversationInfo info;

			QMap<QString, QString> tempMap;
			bool isParse = JsonHelper::ParseValueToMap(val, tempMap);
			if (isParse)
			{
				CommInfo attrInfo;
				attrInfo.AddCommInfo(tempMap);
				info.SetAttrConversationInfo(attrInfo);
			}
			//临时会话，不解析其他节点。
			//todo shizhenyu
			info.IsTempConversation(true);
			gConversationManager->CreateTempChat(info);
			gControl->DoCreateTempChat(info);
		}
		else if (strType == "forceInvite")
		{
			//直接对话响应
			ConversationInfo info;

			QMap<QString, QString> tempMap;
			bool isParse = JsonHelper::ParseValueToMap(val, tempMap);
			if (isParse)
			{
				CommInfo attrInfo;
				attrInfo.AddCommInfo(tempMap);
				info.SetAttrConversationInfo(attrInfo);
			}
			//临时会话，不解析其他节点。
			//todo shizhenyu
			info.IsTempConversation(true);
			gConversationManager->CreateTempChat(info);
			gControl->DoCreateTempChat(info);
		}
		else
		{
			Q_ASSERT(false);
		}
	} 
}
