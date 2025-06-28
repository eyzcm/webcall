#include "utils/Plat_EnumDefine.h"
#include "CommChatMessageCmd.h"
#include "service/util_service.h"



CommChatMessageCmd::CommChatMessageCmd(void) : TcpCmd()
{
}

CommChatMessageCmd::~CommChatMessageCmd(void)
{
}

void CommChatMessageCmd::MakeCmd()
{
    QString strUriFromat = "/Message?type=%1&chatId=%2";
    QString strUri       = "";
    switch (ChatAction())
    {
        case E_Action_QUIT:
            // 5.9	客服关闭对话
            strUri = strUriFromat.arg("quit").arg(ChatId());
            Uri(strUri);
            Body("OK");

            // gUIConversationManager->RemoveChatInfoByChatId(ChatId());
            break;
        case E_Action_SYS_QUIT:
            strUri = strUriFromat.arg("quit").arg(ChatId());
            strUri.append("&autoClose=1");
            Uri(strUri);
            Body("OK");
            break;
        case E_Action_TRANS:
            // 5.10.2	转移对话成功回执
            strUri = strUriFromat.arg("eventMessage").arg(ChatId());
            Uri(strUri);
            Body("<TRANS>");
            break;
        case E_Action_OPINION:
            // 5.7.1	邀请访客评价
            strUri = strUriFromat.arg("eventMessage").arg(ChatId());
            Uri(strUri);
            Body("<OPINION>");
            break;

        case E_ACTION_GETFOCUS:
            // 5.7.1	客服正在输入
            strUri = strUriFromat.arg("getfocus").arg(ChatId());
            Uri(strUri);
            Body("OK");
            break;
        case E_ACTION_QUITSAMEVISITOR:
            strUri = strUriFromat.arg("quit").arg(ChatId());
            strUri.append("&sameVisitor=true&v=");
            strUri.append(VisitorId());
            Uri(strUri);
            Body("OK");
            break;
        default:
            Q_ASSERT(false);
            GERRORLOG("未知的类型！");
            break;
    }
}
