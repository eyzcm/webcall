#include "sendmsgcmd.h"
#include "utils/jsonhelper.h"

SendMsgCmd::SendMsgCmd() : TcpCmd()
{
    MsgType(E_MSGTYPE_DEFAULT);
    RichText(false);
}

SendMsgCmd::~SendMsgCmd()
{
}

void SendMsgCmd::MakeCmd()
{
    QString TempUri;
    switch (MsgType())
    {
        case E_MSGTYPE_DEFAULT:
            Q_ASSERT(false);
            break;
        case E_MSGTYPE_EVENT_CLOSE:
            break;
        case E_MSGTYPE_EVENT_END:
            break;
        case E_MSGTYPE_AUTOACCEPTINVITE:
            break;
        case E_MSGTYPE_AUTOMONTOR:
            break;
        case E_MSGTYPE_COMPOSING:
            break;
        case E_MSGTYPE_DENY:
            break;
        case E_MSGTYPE_FILE:
            if (m_eUserType == E_CUSTOMER)
            {
                TempUri = QString("/Message?to=%1&type=%2&callbackId=%3&robot=%4&questioId=%5&fileName=").arg(MsgTo()).arg("file").arg(CallbackId()).arg(Robot()).arg(RecorderId());
                TempUri.append(FileName());
            }
            else if (m_eUserType == E_CHATGROUP)
            {
                TempUri = QString("/im/group/message?groupId=%1&type=%2&callbackId=%3&robot=%3&questioId=%4").arg(MsgTo()).arg("2").arg(CallbackId()).arg(Robot()).arg(RecorderId());
                Body(FileName());
            }
            else if (m_eUserType == E_VISITOR || m_eUserType == E_MONITOR)
            {
                TempUri = QString("/Message?chatId=%1&type=%2&callbackId=%3&robot=%4&questioId=%5&fileName=").arg(MsgTo()).arg("file").arg(CallbackId()).arg(Robot()).arg(RecorderId());
                TempUri.append(FileName());
                this->ChatId(MsgTo());
            }
            else
            {
                Q_ASSERT(false);
            }

            break;
        case E_MSGTYPE_EVENTCUSTOM:
                if (m_eUserType == E_CUSTOMER)
                {
                    TempUri = QString("/Message?to=%1&type=%2&callbackId=%3&robot=%4&questioId=%5").arg(MsgTo()).arg("eventCustom").arg(CallbackId()).arg(Robot()).arg(RecorderId());
                }
                else if (m_eUserType == E_VISITOR || m_eUserType == E_MONITOR)
                {
                    QString robotLogIdparam;
                    if (!RobotLogId().isEmpty())
                    {
                        robotLogIdparam = QString("&robotLogId=%1").arg(RobotLogId());
                    }
                    if (m_isRichText)
                    {

                        TempUri = QString("/Message?chatId=%1&type=%2&callbackId=%3&robot=%4%5&questioId=%6&richText=%7").arg(MsgTo()).arg("eventCustom").arg(CallbackId()).arg(Robot()).arg(robotLogIdparam).arg(RecorderId()).arg("1");
                    }
                    else
                    {
                        TempUri = QString("/Message?chatId=%1&type=%2&callbackId=%3&robot=%4%5&questioId=%6&richText=%7").arg(MsgTo()).arg("eventCustom").arg(CallbackId()).arg(Robot()).arg(robotLogIdparam).arg(RecorderId()).arg("0");
                    }
                }
                else
                {
                    Q_ASSERT(false);
                }
                break;
        case E_MSGTYPE_MESSAGE:
            if (m_eUserType == E_CUSTOMER)
            {
                TempUri = QString("/Message?to=%1&type=%2&callbackId=%3&robot=%4&questioId=%5").arg(MsgTo()).arg("message").arg(CallbackId()).arg(Robot()).arg(RecorderId());
            }
            else if (m_eUserType == E_CHATGROUP)
            {
                /// im/group/message?groupId=101&type=1
                TempUri = QString("/im/group/message?groupId=%1&type=%2&callbackId=%3&robot=%4&questioId=%5").arg(MsgTo()).arg("1").arg(CallbackId()).arg(Robot()).arg(RecorderId());
            }
            else if (m_eUserType == E_VISITOR || m_eUserType == E_MONITOR)
            {
                QString robotLogIdparam;
                if (!RobotLogId().isEmpty())
                {
                    robotLogIdparam = QString("&robotLogId=%1").arg(RobotLogId());
                }
                if (m_isRichText)
                {

                    TempUri = QString("/Message?chatId=%1&type=%2&callbackId=%3&robot=%4%5&questioId=%6&richText=%7").arg(MsgTo()).arg("message").arg(CallbackId()).arg(Robot()).arg(robotLogIdparam).arg(RecorderId()).arg("1");
                }
                else
                {
                    TempUri = QString("/Message?chatId=%1&type=%2&callbackId=%3&robot=%4%5&questioId=%6&richText=%7").arg(MsgTo()).arg("message").arg(CallbackId()).arg(Robot()).arg(robotLogIdparam).arg(RecorderId()).arg("0");
                }
            }
            else
            {
                Q_ASSERT(false);
            }
            break;
        case E_MSGTYPE_REVOKE:
            if (m_eUserType == E_VISITOR || m_eUserType == E_MONITOR)
            {
                TempUri = QString("/Message?chatId=%1&type=%2&recorderId=%3").arg(MsgTo()).arg("revoke").arg(RecorderId());
            }
            break;
        case E_MSGTYPE_MONITORSUCCESS:
            break;
        case E_MSGTYPE_OPINION:
            break;
        case E_MSGTYPE_SCREENSHOTS:
            if (m_eUserType == E_CUSTOMER)
            {
                TempUri = QString("/Message?to=%1&type=%2&callbackId=%3&robot=%4&questioId=%5&fileName=").arg(MsgTo()).arg("screenShots").arg(CallbackId()).arg(Robot()).arg(RecorderId());
                TempUri.append(FileName());
            }
            else if (m_eUserType == E_CHATGROUP)
            {
                TempUri = QString("/im/group/message?groupId=%1&type=%2&callbackId=%3&robot=%4&questioId=%5").arg(MsgTo()).arg("3").arg(CallbackId()).arg(Robot()).arg(RecorderId());
                Body(FileName());
            }
            else if (m_eUserType == E_VISITOR || m_eUserType == E_MONITOR)
            {
                TempUri = QString("/Message?chatId=%1&type=%2&callbackId=%3&robot=%4&questioId=%5&fileName=").arg(MsgTo()).arg("screenShots").arg(CallbackId()).arg(Robot()).arg(RecorderId());
                TempUri.append(FileName());
            }
            else
            {
                Q_ASSERT(false);
            }
            break;
        case E_MSGTYPE_VIDEO:
            if (m_eUserType == E_CUSTOMER)
            {
                TempUri = QString("/Message?to=%1&type=%2&callbackId=%3&robot=%4&questioId=%5&fileName=").arg(MsgTo()).arg("video").arg(CallbackId()).arg(Robot()).arg(RecorderId());
                TempUri.append(FileName());
            }
            else if (m_eUserType == E_VISITOR || m_eUserType == E_MONITOR)
            {
                TempUri = QString("/Message?chatId=%1&type=%2&callbackId=%3&robot=%4&questioId=%5&fileName=").arg(MsgTo()).arg("video").arg(CallbackId()).arg(Robot()).arg(RecorderId());
                TempUri.append(FileName());
            }
            else
            {
                Q_ASSERT(false);
            }
            break;
        case E_MSGTYPE_VOICE:
            if (m_eUserType == E_CUSTOMER)
            {
                TempUri = QString("/Message?to=%1&type=%2&callbackId=%3&robot=%4&questioId=%5&fileName=").arg(MsgTo()).arg("voice").arg(CallbackId()).arg(Robot()).arg(RecorderId());
                TempUri.append(FileName());
            }
            else if (m_eUserType == E_VISITOR || m_eUserType == E_MONITOR)
            {
                TempUri = QString("/Message?chatId=%1&type=%2&callbackId=%3&robot=%4&questioId=%5&fileName=").arg(MsgTo()).arg("voice").arg(CallbackId()).arg(Robot()).arg(RecorderId());
                TempUri.append(FileName());
            }
            else
            {
                Q_ASSERT(false);
            }
            break;
        case E_MSGTYPE_IMAGE:
            if (m_eUserType == E_CUSTOMER)
            {
                TempUri = QString("/Message?to=%1&type=%2&callbackId=%3&robot=%4&questioId=%5&fileName=").arg(MsgTo()).arg("image").arg(CallbackId()).arg(Robot()).arg(RecorderId());
                TempUri.append(FileName());
            }
            else if (m_eUserType == E_VISITOR || m_eUserType == E_MONITOR)
            {
                TempUri = QString("/Message?chatId=%1&type=%2&callbackId=%3&robot=%4&questioId=%5&fileName=").arg(MsgTo()).arg("image").arg(CallbackId()).arg(Robot()).arg(RecorderId());
                TempUri.append(FileName());
            }
            else
            {
                Q_ASSERT(false);
            }
            break;
        case E_MSGTYPE_TRANS:
            break;
        default:
            Q_ASSERT(false);
            break;
    }

    Uri(TempUri);
}
void SendMsgCmd::ExtMap(QMap<QString, QString> map)
{
    if (map.size() > 0)
    {
        this->IsJsontype(true);
        Json::Value val;
        Json::Value extjonsbody;

        for (auto iter = map.begin(); iter != map.end(); iter++)
        {
            if (iter.key() == "subType")
            {
                val[iter.key().toStdString()] = iter.value().toStdString();
            }
            extjonsbody[(iter.key()).toStdString()] = iter.value().toStdString();
            if ((iter.key() == "autojoinchat") || (iter.key() == "delayTime"))
            {
                val[("ext." + iter.key()).toStdString()] = iter.value().toStdString();
            }
        }

        QString json = QString::fromStdString(extjonsbody.toStyledString());

        val["message"]     = this->Body().toStdString();
        val["extBodyJson"] = json.toStdString();

        this->Body(QString::fromStdString(val.toStyledString()));
    }
}
