#include "stdafx.h"
#include "JsWebWidgetHelp.h"
#include "include/json/value.h"
#include "session/el_sessionservice.h"


#include <model/uiplatcommondatamodel.h>
#include "jswebdbcacheServer.h"
//#include "jswebwebcallserver.h"
#include <utils/jsonhelper.h>

#include "data/PlatUserInfo.h"
#include "protocol/PlatProtocolService.h"
#include "utils/jsonhelper.h"
#include "server/platuiservice.h"

JsWebWidgetHelp::JsWebWidgetHelp() : BaseJsCommHandle()
{
    m_tcpEventLoad= 0;

}

JsWebWidgetHelp::~JsWebWidgetHelp()
{
}
void JsWebWidgetHelp::SetInitData()
{
	InjectScript();
}

void JsWebWidgetHelp::ResultData( const QString seqNo,QString body  )
{
	QString jsVal = QString("clientCallback('%1',%2)").arg(seqNo,body);
	CallJs(jsVal);
}

#if 0
QString JsWebWidgetHelp::GetColleageData( QString text)
{
    QList<MContactItemData*> memberList = UICommonDataModel::GetInstance()->GetColleagueMemberList();

    Json::Value val;
    Json::Value dataval(Json::arrayValue) ;

    Q_FOREACH (MContactItemData* valItem, memberList)
    {
        if (valItem)
        {
            if (valItem->Name().contains(text) || valItem->Userid().contains(text)||valItem->NickName().contains(text))
            {
                Json::Value itemVal;
                itemVal["name"] = valItem->Name().toStdString();
                itemVal["nickName"] = valItem->NickName().toStdString();
                itemVal["userid"] = valItem->Userid().toStdString();
                itemVal["departmentId"] = valItem->DepartmentId().toStdString();

                dataval.append(itemVal );

            }
        }
    }
    val["code"] = "200";
    val["data"] = dataval;
    val["reason"] = "";
    return QString::fromStdString(val.toStyledString());

}
#else

QString JsWebWidgetHelp::GetColleageData( QString text)
{
    QList<PlatUserInfo*> memberList = gSessionService->GetColleagueService()->GetUserInfoList();

    Json::Value val;
    Json::Value dataval(Json::arrayValue) ;

    Q_FOREACH (PlatUserInfo* valItem, memberList)
    {
        if (valItem)
        {
            if (valItem->UserName().contains(text) || valItem->UserId().contains(text)||valItem->PubName().contains(text))
            {
                Json::Value itemVal;
                itemVal["name"] = valItem->UserName().toStdString();
                itemVal["nickName"] = valItem->PubName().toStdString();
                itemVal["userid"] = valItem->UserId().toStdString();
                itemVal["uid"] = valItem->Id().toStdString();
                itemVal["departmentId"] = valItem->DepartmentId().toStdString();

                dataval.append(itemVal );

            }
        }
    }
    val["code"] = "200";
    val["data"] = dataval;
    val["reason"] = "";
    return QString::fromStdString(val.toStyledString());

}
#endif

void JsWebWidgetHelp::invoke(int seqNo,QString servername,QString reqMsg)
{
    if( servername == "webcall")
    {
/*
                JsWebWebcallServer::GetInstance()->process( reqMsg)->success([=](const QVariant & var){
                    //成功
                    if( var.type() == QVariant::Type::String )
                    {
                        Q_EMIT s_resultReady( seqNo,   var.toString());
                    }else if( var.type() == QVariant::Type::Map )
                    {
                        Q_EMIT s_resultReady( seqNo,   var.toMap()["data"].toString());
                    }else{
                        Q_EMIT s_resultReady( seqNo,   R"({})");
                    }
                })->error([=](const QVariant & var){
                    //失败
                    if( var.type() == QVariant::Type::String )
                    {
                        Q_EMIT s_resultReady( seqNo,   var.toString());
					}
					else if (var.type() == QVariant::Type::Map)
					{
						Q_EMIT s_resultReady(seqNo, var.toMap()["data"].toString());

                    }

                });

*/

    }

    if( servername == "dbcache")
    {

        QString retdata = JsWebDbCacheServer::GetInstance()->process( reqMsg);
        Q_EMIT s_resultReady( seqNo,   retdata);
    }


}

QString JsWebWidgetHelp::SendMsg(QString msg)
{
    Json::Value val;
    Json::Value dataval;
    dataval["msg"] = msg.toStdString();
    val["code"] = "200";
    val["data"] = dataval;
    val["reason"] = "";

    return QString::fromStdString(val.toStyledString());

}

void    JsWebWidgetHelp::LoadFinished()
{
	m_isLoadOk = true;
    SetInitData();
}

QString JsWebWidgetHelp::GetColleageList(QString filterText)
{
    return GetColleageData(filterText );
}

QString JsWebWidgetHelp::GetAccountInfo()
{
    Json::Value val;
    val["token"]     = gSessionService->UserInfo()->Token().toStdString();
    val["companyid"] = gSessionService->UserInfo()->CompanyId().toStdString();
    val["userid"]    = gSessionService->UserInfo()->UserId().toStdString();
    val["realName"]    = gSessionService->UserInfo()->RealName().toStdString();
    val["nickName"]    = gSessionService->UserInfo()->NickName().toStdString();
    //todo 待处理
    //val["version"]   = gUISystemConfigManager->GetCurrentVersion().toStdString();

    QString jsonStr = QString::fromStdString(val.toStyledString());
    return jsonStr;

}

void JsWebWidgetHelp::R_SysOpenWeb( )
{
	QDesktopServices::openUrl(m_url );
}
void JsWebWidgetHelp::R_WebRefresh()
{
	Q_EMIT S_RefreshWeb();
}
void JsWebWidgetHelp::R_OpenDevTool()
{
	gViewService->DoGotoUrl("http://localhost:11111");
}

void JsWebWidgetHelp::SetFirstUrl(QString url)
{
	m_url = url;
}

void  JsWebWidgetHelp::InjectScript()
{
	
    QString tt = R"({function clientWebClass()
    {
        this._eventFun = null;
        this._callNo = 0;
        this._funMap = new Map();
        this._name = "clientWebClass";
        this.jsObj = null;
        this._retFun = function( callNo, retobj  )
        {
            var fun = this._funMap.get(callNo);
            if( fun != null ){
                fun(JSON.parse(retobj));
                this._funMap.delete(callNo);
            }
        }
        this.invoke = function( servername, reqMsg,callback)
        {
            this._callNo++;
            this._funMap.set(this._callNo , callback);
            if( this.jsObj != null)
                this.jsObj.invoke(this._callNo, servername,JSON.stringify(reqMsg) );
        }
        this.SetEventFun= function( fun )
        {
           this._eventFun = fun;
            this.jsObj.R_StartTcpEvent( );
        }

        this.eventFun= function(body)
        {
          if( this._eventFun != null) this._eventFun(body);
        }
    }
    var clientHandler = new clientWebClass();
    new QWebChannel(qt.webChannelTransport, function(channel) {
            clientHandler.jsObj  = channel.objects.jsObj;
            clientHandler.jsObj.IsLoadFinished();
            if (typeof window.setInit === 'function') {
             window.setInit(true)  }else{
            window.qtFlag=true;}
            clientHandler.jsObj.s_resultReady.connect(function ( callNo,  param1 , param2,param3) {
                    clientHandler._retFun( callNo,  param1 , param2,param3 );
                });
          });})";
	
	CallJs(tt);

}
/*
void JsWebWidgetHelp::callfun( int callNo, QString serverType,QString funname ,QString  param1,QString param2,QString param3 )
{
    Q_EMIT s_resultReady( callNo,   param1, param2, param3);
}
*/


void JsWebWidgetHelp::R_StartTcpEvent()
{
    if( m_tcpEventLoad == 0)
    {
        m_tcpEventLoad= 1;
        connect( gTcpManager, &Plat_TcpManagerInterface::S_ReceivedMessage, [=](TcpMessagePtr message){
            Json::Value val;
            Json::Value valobj;

            val["packetType"] = message->Uri().toStdString();
            if( JsonHelper::GetJsonValueFromBody(message->Body(),valobj) )
            {
                val["payload"] = valobj;
            }
            QString jsonStr = QString::fromStdString(val.toStyledString());

            QString jsVal   = QString("clientHandler.eventFun(%1)").arg(jsonStr);
            CallJs(jsVal);


        });
    }

}
