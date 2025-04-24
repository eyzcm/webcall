#include "UI_Widget_ChatTransfer.h"
#include "control/CommonSignalService.h"
#include "service/DbUiWebcallService.h"
#include "model/UICommonDataModel.h"
#include "session/el_sessionservice.h"
#include "data/UISummarizeInfo.h"
#include "data/UIVisitorInfo.h"
#include "service/DbUiWebcallService.h"
#include "utils/utilshelper.h"

UI_Widget_ChatTransfer::UI_Widget_ChatTransfer(QWidget* parent) : QWidget(parent), m_pChat(nullptr), m_strPreCustomerID("-")
{
    ui.setupUi(this);

    m_nTiming = 10;
    m_pTimer  = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(R_TimeOut()));

    m_strSourceLeafNodeName = "";
    m_extIp                 = "";
    m_staticID              = "";
    m_visitorID             = "";
    m_chatID                = "";
    m_searchEngineId        = "";
    m_nodeType              = E_YLUI_NODETYPE_NONE;
    m_chatState             = E_CHATITEM_DEFAULT;

    connect(ui.m_btn_refuse, SIGNAL(clicked()), this, SLOT(R_Btn_Refuse_Clicked()));
    connect(ui.m_btn_accept, SIGNAL(clicked()), this, SLOT(R_Btn_Accept_Clicked()));

    connect(this, SIGNAL(S_Btn_Refuse_Clicked(YL_MainTreeLeafNodeItemData*)), CommonSignalService::GetInstance(), SIGNAL(S_Btn_Refuse_Clicked(YL_MainTreeLeafNodeItemData*)));

    connect(this, SIGNAL(S_Btn_Accept_Clicked(YL_MainTreeLeafNodeItemData*)), CommonSignalService::GetInstance(), SIGNAL(S_Btn_Accept_Clicked(YL_MainTreeLeafNodeItemData*)));

    //代码设置label控件宽度（初始化数据时，宽度为默认值100，不准确）
    ui.m_label_visitorName->setFixedWidth(257);
    ui.m_label_visitorID->setFixedWidth(233);
    ui.m_label_visitTime->setFixedWidth(175);
    ui.m_label_cardTag->setFixedWidth(163);
    ui.m_label_preCustomer->setFixedWidth(175);
    ui.m_label_preCustomerID->setFixedWidth(175);
}

UI_Widget_ChatTransfer::~UI_Widget_ChatTransfer()
{
}

void UI_Widget_ChatTransfer::InitData(YL_MainTreeLeafNodeItemData* nodeLeaf)
{
    if (nodeLeaf)
    {
        m_pChat = nodeLeaf;

        m_strSourceLeafNodeName = nodeLeaf->Name();
        m_staticID              = nodeLeaf->Userid();
        m_chatID                = nodeLeaf->Chatid();
        m_visitorID             = nodeLeaf->Vid();
        m_searchEngineId        = nodeLeaf->GetExtendData("SearchEngineId");
        m_extIp                 = nodeLeaf->GetExtendData("ip");
        m_nodeType              = nodeLeaf->NodeType();
        m_chatState             = nodeLeaf->ChatState();
    }
    else
    {
        m_pChat = nullptr;

        m_staticID       = "";
        m_chatID         = "";
        m_visitorID      = "";
        m_searchEngineId = "";
        m_nodeType       = E_YLUI_NODETYPE_NONE;
        m_chatState      = E_CHATITEM_DEFAULT;
    }

    SetIcon_EngineType();
    SetVNameAndVID(m_visitorID);
    SetIpInfo(m_chatID, m_visitorID);
    SetVisitorTime(m_visitorID);
    SetCardTagInfo(m_staticID);
    SetPreCustomerText(m_chatID, m_nodeType);

    m_pTimer->start(1000);
}

void UI_Widget_ChatTransfer::SetIcon_EngineType()
{
    QString path = GetSearchEngineIcon_Original();
    QString icon = QString("border-image: url(%1);").arg(path);
    ui.label_engineIcon->setStyleSheet(icon);
}

void UI_Widget_ChatTransfer::SetVNameAndVID(QString vid)
{
    if (vid.isEmpty())
    {
        ui.m_label_visitorID->setText("-");
        ui.m_label_visitorID->setToolTip("");
    }
    else
    {
        QString formatStr = FormatString(ui.m_label_visitorID, vid);
        ui.m_label_visitorID->setText(formatStr);
        ui.m_label_visitorID->setToolTip(vid);

        if (m_strSourceLeafNodeName.isEmpty())
        {
            QString strVisitorName = gUICacheService->GetDisplayName(vid);
            if (strVisitorName.isEmpty())
            {
                ui.m_label_visitorName->setText("-");
                ui.m_label_visitorName->setToolTip("");
            }
            else
            {
                QString formatStr = FormatString(ui.m_label_visitorName, strVisitorName);
                ui.m_label_visitorName->setText(formatStr);
                ui.m_label_visitorName->setToolTip(strVisitorName);
            }
        }
        else
        {
            QString formatStr = FormatString(ui.m_label_visitorName, m_strSourceLeafNodeName);
            ui.m_label_visitorName->setText(formatStr);
            ui.m_label_visitorName->setToolTip(m_strSourceLeafNodeName);
        }
    }
}

void UI_Widget_ChatTransfer::SetIpInfo(QString chatID, QString vid)
{
    QString strIP = "-";

    //先从对话中获取访客ID
    UIChatInfo* chatInfo = UICommonDataModel::GetInstance()->GetChatInfoByChatId(chatID);
    if (chatInfo)
    {
        QString visitorIP = chatInfo->Visitor_ip();
        if (!visitorIP.isEmpty())
        {
            strIP = visitorIP;
        }
    }

    //如果未从对话中获取到访客IP，则从访客信息中获取访客IP
    if (strIP == "-")
    {
        UIVisitorInfo* visitorInfo = UICommonDataModel::GetInstance()->GetVisitorInfoById(vid);
        if (visitorInfo)
        {
            QString ip = visitorInfo->Ip();
            if (!ip.isEmpty())
            {
                strIP = ip;
            }
        }
    }

    if (strIP == "-" && !m_extIp.isEmpty())
    {
        strIP = m_extIp;
    }

    if (strIP == "-")
    {
        ui.m_label_sourceIP->setText(strIP);
        ui.m_label_sourceIP->setToolTip("");
    }
    else
    {
        QString formatStr = FormatString(ui.m_label_sourceIP, strIP);
        ui.m_label_sourceIP->setText(formatStr);
        ui.m_label_sourceIP->setToolTip(strIP);
    }
}

void UI_Widget_ChatTransfer::SetVisitorTime(QString vid)
{
    QString strVisitTime = "-";

    //从访客信息中获取来访时间
    UIVisitorInfo* visitorInfo = UICommonDataModel::GetInstance()->GetVisitorInfoById(vid);
    if (visitorInfo)
    {
        QString visitTime = visitorInfo->CreateTime();
        if (!visitTime.isEmpty())
        {
            strVisitTime = UtilsHelper::GetDateTimeStringByMSecs(visitTime, "yyyy-MM-dd hh:mm:ss");
        }
    }

    if (strVisitTime == "-")
    {
        ui.m_label_visitTime->setText(strVisitTime);
        ui.m_label_visitTime->setToolTip("");
    }
    else
    {
        QString formatStr = FormatString(ui.m_label_visitTime, strVisitTime);
        ui.m_label_visitTime->setText(formatStr);
        ui.m_label_visitTime->setToolTip(strVisitTime);
    }
}

void UI_Widget_ChatTransfer::SetCardTagInfo(QString userid)
{
    QString strCardColor = "-";
    QString strCardTag   = "-";

    //设置标签名称
    UICardInfo* cardinfo = UICommonDataModel::GetInstance()->GetVisitorCardByUserId(userid);
    if (cardinfo)
    {
        QString keyTag = cardinfo->Tag();
        if (!keyTag.isEmpty())
        {
            UISummarizeInfo* tagInfo = UICommonDataModel::GetInstance()->GetSummaryTagById(keyTag);
            if (tagInfo)
            {
                strCardColor = tagInfo->Color();
                strCardTag   = tagInfo->Title();
            }
        }
        cardinfo->deleteLater();
    }

    if (strCardTag == "-")
    {
        ui.m_label_cardTag->setText(strCardTag);
        ui.m_label_cardTag->setToolTip("");
    }
    else
    {
        QString formatStr = FormatString(ui.m_label_cardTag, strCardTag);
        ui.m_label_cardTag->setText(formatStr);
        ui.m_label_cardTag->setToolTip(strCardTag);
    }

    //设置色块样式
    if (strCardColor == "-")
    {
        QString style = "background-color: #ffffff;";
        ui.m_label_summaryColor->setStyleSheet(style);
        ui.m_label_summaryColor->setText("-");
    }
    else
    {
        QString style = QString("background-color: %1;").arg(strCardColor);
        ui.m_label_summaryColor->setStyleSheet(style);
        ui.m_label_summaryColor->setText("");
    }
}

void UI_Widget_ChatTransfer::SetPreCustomerText(QString& chatID, EYLUI_NODETYPE noteType)
{
    m_strPreCustomerID         = "-";
    QString strPreCustomerName = "-";

    if (!chatID.isEmpty())
    {
        UIChatInfo* chatInfo = UICommonDataModel::GetInstance()->GetChatInfoByChatId(chatID);
        if (chatInfo)
        {
            if ((noteType == E_YLUI_NODETYPE_MONITOR_VISITOR_OFFLINE_VISITOR) || (noteType == E_YLUI_NODETYPE_MONITOR_VISITOR_ONLINE_VISITOR))
            {
                m_strPreCustomerID = chatInfo->CustomerID();
                strPreCustomerName = chatInfo->CustomerName();
            }
            else
            {
                m_strPreCustomerID = chatInfo->PreCustomerId();
            }

            if (m_strPreCustomerID != "-")
            {
                UIUserInfo* currentUser = gSessionService->UserInfo();
                if (currentUser)
                {
                    if (currentUser->UserId() == m_strPreCustomerID)
                    {
                        strPreCustomerName = currentUser->RealName();
                    }
                    else
                    {
                        UIContactInfo* contactInfo = UICommonDataModel::GetInstance()->GetColleagueMemberByID(m_strPreCustomerID);
                        if (contactInfo)
                        {
                            //显示对外名称
                            strPreCustomerName = contactInfo->DisplayInnerName();
                        }
                    }
                }
            }
        }
    }

    //设置上次访问客服
    if (strPreCustomerName == "-")
    {
        ui.m_label_preCustomer->setText(strPreCustomerName);
        ui.m_label_preCustomer->setToolTip("");
    }
    else
    {
        QString formatStr = FormatString(ui.m_label_preCustomer, strPreCustomerName);
        ui.m_label_preCustomer->setText(formatStr);
        ui.m_label_preCustomer->setToolTip(strPreCustomerName);
    }

    //设置上次访客客服ID
    if (m_strPreCustomerID == "-")
    {
        ui.m_label_preCustomerID->setText(m_strPreCustomerID);
        ui.m_label_preCustomerID->setToolTip("");
    }
    else
    {
        QString formatStr = FormatString(ui.m_label_preCustomerID, m_strPreCustomerID);
        ui.m_label_preCustomerID->setText(formatStr);
        ui.m_label_preCustomerID->setToolTip(m_strPreCustomerID);
    }
}

QString UI_Widget_ChatTransfer::FormatString(QLabel* label, QString& str)
{
    QFontMetrics fontWidth(label->font());
    QString      formatStr = fontWidth.elidedText(str, Qt::ElideRight, label->width());
    return formatStr;
}

QString UI_Widget_ChatTransfer::GetSearchEngineIcon_Original()
{
    QString iconUri = "";
    do
    {
        QString name = gUISystemConfigManager->GetEngineNameById(m_searchEngineId);
        if (!name.isEmpty())
        {
            if (name == "google")
            {
                iconUri = "a_source_google";
            }
            else if (name == "baidu")
            {
                iconUri = "a_source_baidu";
            }
            else if (name == "sogou")
            {
                iconUri = "a_source_sogou";
            }
            else if (name == "360")
            {
                iconUri = "a_source_360";
            }
            else
            {
                iconUri = "a_source_unknow";
            }
        }
        else
        {
            // do noting
            iconUri = "a_source_unknow";
        }
    } while (false);

    QString style     = "";
    QString strFromat = ":/YL/Resources/YL/MainTree/OriginalColor_small/%1_%2.png";
    if (!iconUri.isEmpty())
    {
        if (m_nodeType == E_YLUI_NODETYPE_MONITOR_VISITOR_OFFLINE_VISITOR)
        {
            style = strFromat.arg(iconUri).arg("offline");
        }
        else
        {
            if (m_chatState == E_CHATITEM_LEAVE)
            {
                style = strFromat.arg(iconUri).arg("offline");
            }
            else
            {
                style = strFromat.arg(iconUri).arg("online");
            }
        }
    }
    return style;
}

void UI_Widget_ChatTransfer::R_TimeOut()
{
    m_nTiming--;
    if (m_nTiming == -1)
    {
        R_Btn_Refuse_Clicked();
    }
    else
    {
        QString str = QString(tr("接受（%1）")).arg(m_nTiming);
        ui.m_btn_accept->setText(str);
    }
}

void UI_Widget_ChatTransfer::R_Btn_Refuse_Clicked()
{
    m_pTimer->stop();
    if (m_pChat)
    {
        Q_EMIT S_Btn_Refuse_Clicked(m_pChat);
        Q_EMIT S_Closed();
    }
}

void UI_Widget_ChatTransfer::R_Btn_Accept_Clicked()
{
    m_pTimer->stop();
    if (m_pChat)
    {
        Q_EMIT S_Btn_Accept_Clicked(m_pChat);
        Q_EMIT S_Closed();
    }
}
