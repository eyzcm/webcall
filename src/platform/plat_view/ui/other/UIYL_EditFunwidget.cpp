#include "stdafx.h"
#include "uiyl_editfunwidget.h"
#include "MyIconStyle.h"
#include "data/CommInfo.h"
#include "session/el_sessionservice.h"
#include "ui/Common/BaseMainWnd.h"
#include "uimessagebox.h"
#include "utils/ConstStringHelper.h"

#include "utils/jsonhelper.h"

#include "utils/UICommUtilsHelper.h"
#include "utils/utilshelper.h"
#include <QClipboard>
#include <QAction>
#include <QLabel>
#include <QMenu>
#include "server/platuiservice.h"
//#include "uiyl_editextwidget.h"

#include "interface/uiyl_pluginconstructioninterface.h"

UIYL_EditFunwidget::UIYL_EditFunwidget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    m_inputType = 0;
    m_webView_edit = nullptr;
    m_my_chat_toolBarPanel = nullptr;
    InitExWidget();
    Init();

    InitUnfoldOrCollapse();
    InitShowToolbarPanel();
    InitManualTakeOver();
    InitAlternateRobot();
    connect(PlatUiService::GetInstance(), SIGNAL(S_LoadUi(int)), this, SLOT(R_LoadUi(int)));
}

UIYL_EditFunwidget::~UIYL_EditFunwidget()
{
}

void UIYL_EditFunwidget::Init()
{
    InitModel();
    InitSendButton();
}

void UIYL_EditFunwidget::LoadOtherUi()
{
    if( m_webView_edit ) m_webView_edit->deleteLater();
    m_webView_edit = PlatUiService::GetInstance()->GetInputUi();
}

void UIYL_EditFunwidget::InitModel()
{
	/*
    UIToolBarModel::GetInstance()->RegisterObserver(ui.m_my_chat_toolBarPanel);
    ui.m_my_chat_toolBarPanel->InitTabPage(E_TAB_MY_CHAT_PAGE);
	*/
}


void UIYL_EditFunwidget::InitSendButton()
{
    InitSendButton(ui.m_my_chat_sendBtn);
}

void UIYL_EditFunwidget::InitSendButton(UI_Btn_Enter *objData)
{
    if (objData)
    {
        QString sendBtnStyle = "QToolButton{border:0px;color:#ffffff;background-color:rgba(255, 255, 255, 0)}";
        objData->SetMainBtnStyle(sendBtnStyle);
        QString sendBtnContentStyle = "border-image: url(:/YL/Resources/YL/InputFun/btn_enterNormal.png);";
        objData->SetContentStyle(sendBtnContentStyle);
        objData->SetMainBtn("", tr("发送"));
        objData->SetArrawBtnIcon(":/YL/Resources/YL/InputFun/btn_downArrow.png");
        connect(objData, SIGNAL(S_MoveOnButton(bool)), this, SLOT(R_MoveOnSendMenuButton(bool)));
        connect(objData, SIGNAL(S_MainBtnClicked()), this, SLOT(R_OnSendBtnClick()));
        connect(objData, SIGNAL(S_ArrawBtnClicked()), this, SLOT(R_OnSendBtnMenuClick()));
    }
}

void UIYL_EditFunwidget::CreateEnterMenu(QString data, UI_Btn_Enter *objData)
{
    QMenu   *sendmenu    = new QMenu();
    QAction *enterAction = new QAction(QIcon(""), tr("按Enter键发送消息"), this);
    connect(enterAction, SIGNAL(triggered()), this, SLOT(R_OnSendBtnMenuClicked()));
    enterAction->setData(QVariant("enter"));
    sendmenu->addAction(enterAction);
    QAction *ctrlenterAction = new QAction(QIcon(""), tr("按Ctrl+Enter键发送消息"), this);
    connect(ctrlenterAction, SIGNAL(triggered()), this, SLOT(R_OnSendBtnMenuClicked()));
    ctrlenterAction->setData(QVariant("ctrl_enter"));
    sendmenu->addAction(ctrlenterAction);

    if (data == "enter")
    {
        enterAction->setIcon(QIcon(":/Resources/Image/menu_select.png"));
    }
    else if (data == "ctrl_enter")
    {
        ctrlenterAction->setIcon(QIcon(":/Resources/Image/menu_select.png"));
    }
    else
    {
        Q_ASSERT(false);
    }
    if (objData)
    {
        objData->SetMenu(sendmenu);
    }
}

void UIYL_EditFunwidget::SetEnterMenuIcon(QString data)
{
    CreateEnterMenu(data, ui.m_my_chat_sendBtn);
}


void UIYL_EditFunwidget::R_MoveOnSendMenuButton(bool isOn)
{
    if (!isOn)
    {
        QString sendBtnContentStyle = "border-image: url(:/YL/Resources/YL/InputFun/btn_enterNormal.png);";
        ui.m_my_chat_sendBtn->SetContentStyle(sendBtnContentStyle);
    }
    else
    {
        QString sendBtnContentStyle = "border-image: url(:/YL/Resources/YL/InputFun/btn_enterHover.png);";
        ui.m_my_chat_sendBtn->SetContentStyle(sendBtnContentStyle);
    }
}

void UIYL_EditFunwidget::R_OnSendBtnMenuClicked()
{
    QAction *action = (QAction *) this->sender();
    auto     data   = action->data().toString();

    //todo 9999
    /*
    CommInfo otherInfo(*gUIPersonalConfigManager->GetUserOtherSetting());
    otherInfo.SetValueByKey("sendtype", data);
    gUIPersonalConfigManager->SetUserOtherSetting(otherInfo);
*/
    SetEnterMenuIcon(data);
}

void UIYL_EditFunwidget::R_OnSendBtnClick() //发送文字消息 1
{
    m_webView_edit->GetInputValue(1);
}

void UIYL_EditFunwidget::R_GetInputValue(QString str, int i)
{
    if (i == 0)
    {
        str = UtilsHelper::ReplaceHtmlTag(str);
        str = str.replace("&nbsp;", " ");
        str = str.trimmed();
        m_webView_edit->SetInputValue(str);
    }
    else if (i == 1)
    {
        SendMessageByBody(str);
    }
    else if (i == 2)
    {
        if (str.trimmed().isEmpty())
        {
            m_webView_edit->SetInputValue(str);
        }
    }
    else if (i == 3)
    {

        //todo 9999
     //   m_extWidget->AddRefeererMsg(str);
    }
}



void UIYL_EditFunwidget::R_OnSendBtnMenuClick()
{
    ui.m_my_chat_sendBtn->ShowMenu();
}

void UIYL_EditFunwidget::SendMessageByBody(QString body, QString actionId) //发送文字消息 2
{
    if (!body.isEmpty())
    {
        // CZC：特殊处理一下，判断结尾是否存在换行
        body = body.trimmed();
        if (body.endsWith("<br/></p>"))
        {
            int nPos = body.lastIndexOf("<br/></p>");
            if (nPos != -1)
            {
                body = body.mid(0, nPos) + "</p>";
            }
        }
        if (body.isEmpty())
        {
            UICommUtilsHelper::ShowCommonBox(tr("提示"), tr("不能发送空白消息!"));
        }
        else if (m_nCurTextSize > 500)
        {
            UICommUtilsHelper::ShowCommonBox(tr("提示"), tr("输入字符不能超过500个,请调整后发送!"));
        }
        else if (body.count() > Rich_Length)
        {
            UICommUtilsHelper::ShowCommonBox(tr("提示"), tr("富文本字符不能超过") + QString::number(Rich_Length) + tr("个,请调整后发送!"));
        }
        else
        {
            //todo 9999
            //this->m_extWidget->SendMsg(m_inputType,m_referredMsg,body,actionId );
            //todo 待处理
            /*
            if (IsCheckSendMessage(CurrentChatItem()))
            {
                QString vid = CurrentChatItem()->Vid();
                if (!vid.isEmpty())
                {
                    Q_EMIT S_TimeoutAutoRobotReplyMessage(vid, 0);
                }
                QMap<QString, QString> map;
                if (m_inputType == 1)
                {
                    JsonHelper::ParseValueToMapByString(this->m_referredMsg, map);

                    m_inputType = 0;
                }
                UIYL_ChatRecordModel::GetInstance()->SendMsgEx(body, false, actionId, map); //发送文字消息 2
            }
            else if (CurrentChatItem())
            {
                if (CurrentChatItem()->ChatState() == E_CHATITEM_BROWSE || CurrentChatItem()->ChatState() == E_CHATITEM_LEAVE || CurrentChatItem()->ChatState() == E_CHATITEM_CHAT_CLOSE || CurrentChatItem()->ChatState() == E_CHATITEM_MONITOR_CHAT_CLOSE)
                {
                    if (CurrentChatItem()->SearchingType() == "DouYin")
                    {
                        //抖音离线消息发送唤醒
                        Json::Value val;
                        bool        isOk = JsonHelper::GetJsonValueFromBody(CurrentChatItem()->AwakeInfo(), val);
                        if (isOk)
                        {
                            val["message"]    = body.toStdString();
                            QString awakeInfo = QString::fromStdString(val.toStyledString());
                            gIProtocolAgent->DoAwake(CurrentChatItem()->SearchingType(), CurrentChatItem()->Awake(), awakeInfo);
                        }
                    }
                }
            }
            */
            m_webView_edit->SetInputValue("");
            m_webView_edit->setFocus();
            m_referredMsg = "";
        }
    }
}

void UIYL_EditFunwidget::AddHtmlText(QString text)
{
    m_webView_edit->SetInputValue(text); //可以发送富文本
    m_webView_edit->setFocus();
}



QString UIYL_EditFunwidget::GetCurrentVid()
{
    MChatItemData *itemData = UIPlatCommonDataModel::Instance()->CurrentChatItem();
    if (itemData)
        return itemData->Vid();
    return "";
}

void UIYL_EditFunwidget::R_LoginStateChanged(ELOGINSTATUS oldStatus, ELOGINSTATUS newStatus)
{
    switch (newStatus)
    {
        case E_LOGIN_NONE:
            break;
        case E_LOGIN_PRE:
            break;
        case E_LOGIN_DOING:
            break;
        case E_LOGIN_OK_MAINTCP:
            if (oldStatus == E_LOGIN_DOING)
            {

                ShowMask();
            }
            break;
        case E_LOGIN_PRE_MONITORTCP:
            break;
        case E_LOGIN_ERROR:

            break;
        case E_LOGIN_ERROR_MAINTCP:
        case E_LOGIN_RETRY_MAINTCP:
            break;
        case E_LOGIN_ERROR_MONITORTCP:
        case E_LOGIN_RETRY_MONITORTCP:
            break;
        case E_LOGIN_ALLOK:
            break;
        case E_LOGIN_LOGOUT:
            break;
    }
}


bool UIYL_EditFunwidget::eventFilter(QObject *obj, QEvent *e)
{
    // CZC：显隐伸缩按钮
    if (obj == ui.widget_showWindowAndToolbar)
    {
        if (m_IsUnfold)
        {
            if (e->type() == QEvent::Enter)
            {
                ui.m_btn_UnfoldOrCollapse->show();
            }
            else if (e->type() == QEvent::Leave)
            {
                ui.m_btn_UnfoldOrCollapse->hide();
            }
        }
    }

    return QWidget::eventFilter(obj, e);
}
void UIYL_EditFunwidget::resizeEvent(QResizeEvent *event)
{
    if (m_mask)
    {
        m_mask->setGeometry(0, 0, this->width(), this->height());
    }

    int nWidgetH = ui.widget_edit->height();
	if( m_webView_edit )m_webView_edit->setFixedHeight(nWidgetH);

    Q_EMIT S_Panel_PredictiveInput_SizeChange();
}
// CZC：候补机器人
void UIYL_EditFunwidget::InitAlternateRobot()
{
    //todo 待处理
    //m_currentItem = nullptr;

    connect(CommonSignalService::Instance(), SIGNAL(S_JoininChat(AI_CHAT_STATUS, QString, QString, QString, int, int)), this, SLOT(R_JoininChat(AI_CHAT_STATUS, QString, QString, QString, int, int)));
    //todo 9999
    //connect(this, SIGNAL(S_TimeoutAutoRobotReplyMessage(QString, int)), CommonSignalService::Instance()->AiSign(), SIGNAL(S_TimeoutAutoRobotReplyMessage(QString, int)));
}
// CZC：禁用人工接管
void UIYL_EditFunwidget::InitManualTakeOver()
{
    m_mask        = new QWidget(this);
    QLabel *label = new QLabel(m_mask);
    label->setText(tr("设置中心禁用了人工接管"));
    label->setAlignment(Qt::AlignCenter);
    QHBoxLayout *hLayout = new QHBoxLayout(m_mask);
    hLayout->addWidget(label);
    m_mask->setLayout(hLayout);
    m_mask->setStyleSheet("QWidget{background-color: rgba(255, 255, 255, 150);}");
    m_mask->hide();
}
void UIYL_EditFunwidget::ShowMask()
{
    //设置中心禁用了人工接管，窗口添加遮罩，所有功能都不能使用
    bool hsManualTakeOver = gSessionService->HasPermissionByKey("b512243a-ec37-4207-8190-fd6cc26dbd98");
    if (hsManualTakeOver && m_mask)
    {
        m_mask->setGeometry(0, 0, this->width(), this->height());
        m_mask->show();
        m_mask->setFocus();
    }
}
//【曹振超】输入功能区展开和收起
void UIYL_EditFunwidget::InitUnfoldOrCollapse()
{
    m_IsUnfold = true;
    ui.m_btn_UnfoldOrCollapse->hide();
    ui.widget_showWindowAndToolbar->installEventFilter(this);
    connect(ui.m_btn_UnfoldOrCollapse, SIGNAL(clicked()), this, SLOT(R_Btn_UnfoldOrCollapse_Clicked()));
}

void UIYL_EditFunwidget::R_Btn_UnfoldOrCollapse_Clicked()
{
    if (m_IsUnfold)
    {
        ui.m_btn_UnfoldOrCollapse->setIcon(QIcon(":/YL/Resources/YL/EditToolbar/btn_editFunWidget_Unfold.png"));

        ui.m_rc_chat_bottomPanel->hide();
        ui.widget_edit->hide();
    }
    else
    {
        ui.m_btn_UnfoldOrCollapse->setIcon(QIcon(":/YL/Resources/YL/EditToolbar/btn_editFunWidget_Collapse.png"));
        ui.m_btn_UnfoldOrCollapse->hide();

        ui.m_rc_chat_bottomPanel->show();
        ui.widget_edit->show();
    }
    m_IsUnfold = !m_IsUnfold;
    Q_EMIT S_ShowEditFunWidget(m_IsUnfold);
    gIProtocolAgent->DoUploadOperatorLog("online_client_conversation_17");
}
//【曹振超】富文本编辑框
void UIYL_EditFunwidget::InitShowToolbarPanel()
{
    m_nCurTextSize  = 0;
    m_IsShowToolbar = 1;
    m_strSelectText = "";
    SetBtnShowToolbarStyle();


    connect(m_webView_edit, SIGNAL(S_OnShortCutKey(QString)), this, SLOT(R_OnShortCutKey(QString)));
    connect(m_webView_edit, SIGNAL(S_OnRichTextChanged(QString)), this, SLOT(R_OnRichTextChanged(QString)));
    connect(m_webView_edit, SIGNAL(S_OnRichTextChanged(QString)), this, SLOT(R_ConversationTextChangedEx(QString)));
    connect(m_webView_edit, SIGNAL(S_SetInputType(int)), this, SLOT(R_SetInputType(int)));
    connect(m_webView_edit, SIGNAL(S_RightBtnClicked(QString, QString)), this, SLOT(R_RightBtnClicked(QString, QString)));
    connect(ui.m_btn_showToolbarPanel, SIGNAL(clicked()), this, SLOT(R_Btn_ShowToolbarPanel_Clicked()));
    connect(this, SIGNAL(S_KeyUp()), CommonSignalService::Instance(), SIGNAL(S_KeyUp()));
    connect(this, SIGNAL(S_KeyDown()), CommonSignalService::Instance(), SIGNAL(S_KeyDown()));

    m_actionCopy = new QAction(nullptr);
    m_actionCopy->setText(tr("复制"));
    m_actionCopy->setData("copy");
    connect(m_actionCopy, SIGNAL(triggered()), this, SLOT(R_OnActionClicked()));

    m_actionPaste = new QAction(nullptr);
    m_actionPaste->setText(tr("粘贴"));
    m_actionPaste->setData("paste");
    connect(m_actionPaste, SIGNAL(triggered()), this, SLOT(R_OnActionClicked()));

    m_pMenu = new QMenu(this);
}

void UIYL_EditFunwidget::R_Btn_ShowToolbarPanel_Clicked()
{
    gIProtocolAgent->DoUploadOperatorLog("online_client_conversation_09");
    //显隐录入区原生工具栏
    if (m_IsShowToolbar == 1)
    {
        m_IsShowToolbar = 0;
    }
    else
    {
        m_IsShowToolbar = 1;
    }
    SetBtnShowToolbarStyle();
    m_webView_edit->HiddenTools(m_IsShowToolbar);
}

void UIYL_EditFunwidget::R_OnShortCutKey(QString key)
{
    //todo 9999
#if 0
    CommInfo *commInfo = gUIPersonalConfigManager->GetUserOtherSetting();
    if (commInfo)
    {
        //处理发送按键
        QString strSendType = commInfo->GetValueByKey("sendtype").toLower();
        if (strSendType == "ctrl_enter")
        {
            strSendType = "ctrl+13";
        }
        else //新账号没有该配置项时,默认回车发送消息
        {
            strSendType = "13";
        }

        if (strSendType == key)
        {
            //发送
            R_OnSendBtnClick();
        }
        else if (key == "38")
        {
            //小键盘：上
            Q_EMIT S_KeyUp();
        }
        else if (key == "40")
        {
            //小键盘：下
            Q_EMIT S_KeyDown();
        }
        else if (key == "ctrl+shift+86")
        {
            //todo 待处理
			/*
            //发送图片
            if (IsCheckSendMessage(this->CurrentChatItem()))
            {
                UIOpertorSettingInfo *operInfo = gUIPersonalConfigYLService->OpertorSettingInfo();
                if ((operInfo) && (operInfo->SendImagePreview() == 0))
                {
                    QString filepath = UtilsHelper::GetDownLoadFileDir();
                    filepath += QString("\\%1.png").arg(QTime::currentTime().msec() % 10000);
                    QClipboard *board = QApplication::clipboard();
                    if (board)
                    {
                        if (!board->image().isNull())
                        {
                            board->image().save(filepath, "png", 1);
                            Q_EMIT S_OnSendFile(this->CurrentChatItem(), filepath, E_MSGTYPE_SCREENSHOTS);
                        }
                    }
                }
                else
                {
                    CommonSignalService::Instance()->DoSendClpboardImage(this->CurrentChatItem());
                }
                if (operInfo)
                    delete operInfo;
                operInfo = NULL;
            }
			*/
        }
        else if ((key.indexOf("ctrl") != -1) || (key.indexOf("shift") != -1) || (key.indexOf("alt") != -1))
        {
            //常用语快捷键
            key = key.remove(QRegExp("\\s"));
            key = key.toLower();

            QString     keyCode = "";
            QStringList list    = key.split("+");
            for (int i = 0; i < list.size(); ++i)
            {
                QString temp = list.at(i);
                if (temp.indexOf("ctrl") != -1)
                {
                    keyCode = temp.replace("ctrl", "Ctrl + ");
                }
                if (temp.indexOf("shift") != -1)
                {
                    keyCode += temp.replace("shift", "Shift + ");
                }
                if (temp.indexOf("alt") != -1)
                {
                    keyCode += temp.replace("alt", "Alt + ");
                }
            }
            keyCode += list.at(list.size() - 1);

            if (keyCode.lastIndexOf("+") != -1)
            {
                int     nPos    = keyCode.lastIndexOf("+") + 2;
                int     nValue  = keyCode.mid(nPos).toInt();
                QString strChar = (QChar) nValue;
                keyCode         = keyCode.mid(0, nPos) + strChar;

                QString strContent = gUICommonWordManager->GetContentByHotkey(keyCode);
                if (!strContent.isEmpty())
                {
                    m_webView_edit->SetInputValue("");
                    m_webView_edit->SetInputValue(strContent);
                    m_webView_edit->setFocus();
                }
            }
        }
    }
#endif
}

void UIYL_EditFunwidget::R_OnRichTextChanged(QString txt)
{
    m_nCurTextSize = txt.count();

	//todo 5296 
    QString intelligentInput = UICommonDataModel::GetInstance()->GetSystemConfigByKey(ConstStringHelper::m_cIntelligentInput, "0"); //是否开启智能输入
    if (intelligentInput == "1")
    {
        //匹配常用语

        //todo 9999
        //m_webView_edit->Js_AddSearchCommonWordList(UIChatCommonWordListModel::GetInstance()->AddSearchCommonWordListByKey(txt));
    }
}

void UIYL_EditFunwidget::R_RightBtnClicked(QString selecetText, QString pos)
{
    m_pMenu->clear();
    m_strSelectText = selecetText;

    if (!selecetText.isEmpty())
    {
        m_pMenu->addAction(m_actionCopy);
    }

    QClipboard *clip = QApplication::clipboard();
    if (!clip->text().isEmpty())
    {
        m_pMenu->addAction(m_actionPaste);
    }
    if (!m_pMenu->isEmpty())
    {
        m_pMenu->exec(QCursor::pos());
    }
}

void UIYL_EditFunwidget::R_ConversationTextChangedEx(QString txt)
{
    //todo 待处理
	/*
    MChatItemData *dataObj = CurrentChatItem();
    if (dataObj && dataObj->ChatState() == E_CHATITEM_CHAT && !txt.isEmpty())
    {
        ConversionController::GetInstance()->SendComposingToVisitor(dataObj->Chatid()); //发送客服正在输入状态
    }
	*/
}
void UIYL_EditFunwidget::R_OnActionClicked()
{
    QAction    *action = (QAction *) this->sender();
    QString     key    = action->data().toString();
    QClipboard *clip   = QApplication::clipboard();

    if (key == "copy")
    {
        clip->setText(m_strSelectText);
    }
    else if (key == "paste")
    {
        m_webView_edit->InsertHtmlValue(clip->text());
        m_webView_edit->setFocus();
    }
}
void UIYL_EditFunwidget::SetBtnShowToolbarStyle()
{
    QString strStyle = "";
    if (m_IsShowToolbar == 1)
    {
        //常规模式
        strStyle = "QPushButton{border:0px;}QPushButton:hover,QPushButton:pressed{border-image: url(:/YL/Resources/YL/InputFun/btn_hover.png);}";
    }
    else
    {
        //选中模式
        strStyle = "QPushButton,QPushButton:hover,QPushButton:pressed{border-image: url(:/YL/Resources/YL/InputFun/btn_hover.png);}";
    }
    ui.m_btn_showToolbarPanel->setStyleSheet(strStyle);
}
void UIYL_EditFunwidget::SetHotKey()
{
    //todo 9999
    /*
    UIShowSettingInfo *personInfo = gUIPersonalConfigYLService->ShowSettingInfo();
    CommInfo          *commInfo   = gUIPersonalConfigManager->GetUserOtherSetting();

    if (personInfo && commInfo)
    {
        QString strScreenShot       = personInfo->KeyScreenShot().toLower();
        QString strCommonWordSearch = personInfo->KeyCommonWordSearch().toLower();
        QString strOpenVisitor      = personInfo->KeyOpenCard().toLower();
        QString strTopClient        = personInfo->KeyActiveMainWnd().toLower();
        QString strChangeChat       = personInfo->KeyChatChange().toLower();
        QString strSendType         = commInfo->GetValueByKey("sendtype").toLower();

        QString strKeys = "";
        FormatString(strKeys, strScreenShot);
        FormatString(strKeys, strCommonWordSearch);
        FormatString(strKeys, strOpenVisitor);
        FormatString(strKeys, strTopClient);
        FormatString(strKeys, strChangeChat);
        FormatString(strKeys, strSendType);

        if (!strKeys.isEmpty())
        {
            strKeys += ",ctrl+shift+86";
        }
        else
        {
            strKeys += "ctrl+shift+86";
        }
        m_webView_edit->SetShortCutKeyFiter(strKeys);

        delete personInfo;
        personInfo = nullptr;
    }
    */
}
void UIYL_EditFunwidget::FormatString(QString &keys, QString str)
{
    if (!str.isEmpty())
    {
        str = str.remove(QRegExp("\\s"));
        if (str.indexOf("enter") == -1)
        {
            QStringList list = str.split("+");
            for (int i = 0; i < list.size(); ++i)
            {
                if ((list.at(i).indexOf("ctrl") == -1) && (list.at(i).indexOf("shift") == -1) && (list.at(i).indexOf("alt") == -1))
                {
                    QByteArray byte    = list.at(i).toUtf8();
                    int        keyCode = byte.at(0);
                    list[i]            = QString::number(keyCode);
                }
            }
            QString strTemp = list.join("+");

            if (!keys.isEmpty())
            {
                keys += ",";
            }
            keys += strTemp;
        }
        else
        {
            if (str == "enter")
            {
                if (!keys.isEmpty())
                {
                    keys += ",";
                }
                keys += "13";
            }
            else
            {
                QStringList list    = str.split("_");
                QString     strTemp = list[0] + "+" + "13";
                if (!keys.isEmpty())
                {
                    keys += ",";
                }
                keys += strTemp;
            }
        }
    }
}


void UIYL_EditFunwidget::R_SetInputType(int type)
{
    m_inputType = type;
}
//////////////////////////////////////////////////////////////////////////



void UIYL_EditFunwidget::R_LoadUi(int type )
{
    m_webView_edit = PlatUiService::GetInstance()->GetInputUi();
    connect(m_webView_edit, &UIYL_InputTextPanelInterface::S_GetInputValue, this, &UIYL_EditFunwidget::R_GetInputValue);
    ui.gridLayout->addWidget(m_webView_edit, 0, 0, 1, 1);

    m_my_chat_toolBarPanel = PlatUiService::GetInstance()->GetChatToolBar();
    ui.m_my_chat_toolBarPanelLayout->addWidget(m_my_chat_toolBarPanel);


    //增加插件
    if( PlatUiService::GetInstance()->GetPluginConstruction() )
        PlatUiService::GetInstance()->GetPluginConstruction()->ConstructionEditPlugin(this);


}

void UIYL_EditFunwidget::InitExWidget()
{
    //todo 9999
    /*
    m_extWidget = new UIYL_EditExtWidget(this);
    m_extWidget->AddBottomButton(ui.m_robot_returnBtn,ui.m_robot_joinInBtn,ui.m_transchatInBtn);
    m_extWidget->AddRightButton(ui.m_btn_alternateRobot_showOrHide);
    */


}

UIYL_InputTextPanelInterface * UIYL_EditFunwidget::GetEditPanel()
{
    return m_webView_edit;
}

void UIYL_EditFunwidget::SetReferredMsg(const QString referredMsg )
{
    m_referredMsg = referredMsg;
}

int UIYL_EditFunwidget::IsShowToolbar()
{
    return m_IsShowToolbar;
}

void UIYL_EditFunwidget::ProcessRefeererMsg(QString vid, QString searchName)
{
    m_webView_edit->SetInputValue(m_inputTextMap[vid]);
    m_referredMsg = m_inputRefeererMap[vid];
    m_webView_edit->AddReferredMsg(m_referredMsg);
    m_webView_edit->SetSearchEngineName(searchName);
    m_webView_edit->setFocus();
}

void UIYL_EditFunwidget::ProcessRefeererMsgEx(QString oldVid,QString vid,QString str, QString searchName)
{
    m_inputTextMap.insert(oldVid, str);
    m_inputRefeererMap.insert(m_currentVid, this->m_referredMsg);

    // CZC：此处改为webView方式，不用禁用，因为不会抢焦点并影响到快捷键

    ProcessRefeererMsg(vid,searchName);
}
