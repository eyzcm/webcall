#include "UI_Widget_AuxiliaryFun_Panenl.h"
#include "Common/UI_Widget_SystemPrompt.h"
#include "utils/ConstStringHelper.h"
#include "utils/EnumDefine.h"
#include "utils/UICommUtilsHelper.h"
#include "session/el_sessionservice.h"
#include "control/CommonSignalService.h"
#include "control/IUIController.h"
#include "control/geasyliaoframework.h"
#include "data/CommInfo.h"
#include "service/DbUiWebcallService.h"
#include "model/UICommonDataModel.h"
#include "protocol_biz/webcallprotocolservice.h"
#include "session/el_sessionservice.h"
#include "ui\Common\uilockwnd.h"
#include "ui/MyIconStyle.h"
#include "data/UICompanyBaseInfo.h"
#include "utils/utilshelper.h"
#include <QAction>
#include <QDesktopServices>
#include <QMenu>
#include <QProcess>
#include <QUrl>

UI_Widget_AuxiliaryFun_Panenl::UI_Widget_AuxiliaryFun_Panenl(QWidget *parent) : QWidget(parent), m_nUnreadNotice(0)
{
    m_skinMenu     = NULL;
    m_customerMenu = nullptr;
    m_topWnd       = false;
    //过渡页显示进度
    gLoginTransitionsPage->SendSchedule("90", tr("初始化辅助功能区..."));

    ui.setupUi(this);

    connect(ui.btn_calculator, SIGNAL(clicked()), this, SLOT(R_Btn_Clicked()));
    connect(ui.btn_changeSkin, SIGNAL(clicked()), this, SLOT(R_Btn_Clicked()));
    connect(ui.btn_lockScreen, SIGNAL(clicked()), this, SLOT(R_Btn_Clicked()));
    connect(ui.btn_resetScale, SIGNAL(clicked()), this, SLOT(R_Btn_Clicked()));
    connect(ui.btn_msgPrompt, SIGNAL(clicked()), this, SLOT(R_Btn_Clicked()));
    connect(ui.btn_TopWnd, SIGNAL(clicked()), this, SLOT(R_Btn_Clicked()));
    connect(ui.btn_consultCustomerOnline, SIGNAL(clicked()), this, SLOT(R_ShowCustomerMenu()));

    connect(this, SIGNAL(S_SetMainWndTop(bool)), CommonSignalService::GetInstance(), SIGNAL(S_SetMainWndTop(bool)));
    connect(this, SIGNAL(S_LockScreen_Clicked()), CommonSignalService::GetInstance(), SIGNAL(S_TopRight_LockScreen_Clicked()));
    connect(this, SIGNAL(S_ResetScale_Clicked()), CommonSignalService::GetInstance(), SIGNAL(S_TopRight_ResetScale_Clicked()));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_GetUnreadAnnouncementResult(QString, QString, QString, int)), this, SLOT(R_GetUnreadAnnouncementResult(QString, QString, QString, int)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_ReadOneNotice()), this, SLOT(R_ReadOneNotice()));


    connect(CommonSignalService::GetInstance(), &CommonSignalService::S_SetUnreadMsgNum, this, &UI_Widget_AuxiliaryFun_Panenl::R_SetUnreadMsgNum);

    InitSkinMenu();
    InitCustomerMenu();
}

UI_Widget_AuxiliaryFun_Panenl::~UI_Widget_AuxiliaryFun_Panenl()
{
}

QPoint UI_Widget_AuxiliaryFun_Panenl::getNoticepos()
{
    QPoint pt = mapToGlobal(ui.btn_msgPrompt->pos());
    QPoint pos;
    pos.setX(pt.x() + ui.btn_msgPrompt->width() / 2);
    pos.setY(pt.y() + ui.btn_msgPrompt->height());
    return pos;
}

void UI_Widget_AuxiliaryFun_Panenl::CreateChatOnline()
{
    CommInfo *commInfo = UICommonDataModel::GetInstance()->GetIconInfoByKey("console");
    if (commInfo)
    {
        QString retStr = commInfo->GetValueByKey("auth");
        if (!retStr.isEmpty())
        {
            QString strUrl        = "";
            QString strDomainName = retStr.mid(0, retStr.indexOf("console/"));
            if (strDomainName.indexOf("prd19") != -1)
            {
                strUrl = strDomainName + "live/chat.do?c=10793&g=31985&ext=";
            }
            else if (strDomainName.indexOf("group") != -1)
            {
                strUrl = strDomainName + "live/chat.do?c=35078&g=30895&ext=";
            }
            else //其他环境默认使用 prd19
            {
                strUrl = "https://prd19.easyliao.com/live/chat.do?c=10793&g=31985&ext=";
            }

            if (!strUrl.isEmpty())
            {
                QString            strCompanyName = "";
                UICompanyBaseInfo *companyInfo    = gUIOtherServiceManager->CompanyBaseInfoInfoDup();
                if (companyInfo)
                {
                    strCompanyName = companyInfo->CompanyName();
                    delete companyInfo;
                    companyInfo = nullptr;
                }

                QString strCompanyID     = gSessionService->UserInfo()->CompanyId();
                QString strClientVersion = gUISystemConfigManager->GetCurrentVersion();
                QString strSeatsCount    = gSessionService->GetLicenceSetting()->LimitOnlineCount();

                QString expired         = gSessionService->GetLicenceSetting()->Expired();
                QString strSeatsDueDate = UtilsHelper::GetDateTimeStringByMSecs(expired, "yyyy-MM-dd");

                QString strCustomerType = tr("综合版");
                QString strExt          = QString("#params:K_CompanyName,%1,K_CompanyID,%2,K_ClientVersion,%3,K_CustomerType,%4,K_SeatsCount,%5,K_SeatsDueDate,%6").arg(strCompanyName, strCompanyID, strClientVersion, strCustomerType, strSeatsCount, strSeatsDueDate);

                strUrl += UtilsHelper::EncodeUrlByString(strExt);
                QUrl url = QUrl::fromEncoded(strUrl.toUtf8());
                QDesktopServices::openUrl(url);
            }
        }
    }
}

void UI_Widget_AuxiliaryFun_Panenl::R_Btn_Clicked()
{
    QPushButton *pBtn = (QPushButton *)this->sender();
    if (pBtn == ui.btn_calculator)
    {
        QProcess::startDetached("calc\n");
        gIProtocolAgent->DoUploadOperatorLog("online_client_conversation_35");
    }
    else if (pBtn == ui.btn_msgPrompt)
    {
        Q_EMIT S_ShowAnnouncement_Details_Clicked();
    }
    else if (pBtn == ui.btn_changeSkin)
    {
        ShowSkinMenu();
    }
    else if (pBtn == ui.btn_lockScreen)
    {
        UILockWnd *lockWnd = new UILockWnd();
        connect(lockWnd, SIGNAL(S_LockScreen_Clicked()), this, SIGNAL(S_LockScreen_Clicked()));
        lockWnd->show();
        gIProtocolAgent->DoUploadOperatorLog("online_client_conversation_36");
    }
    else if (pBtn == ui.btn_resetScale)
    {
        Q_EMIT S_ResetScale_Clicked();
        gIProtocolAgent->DoUploadOperatorLog("online_client_conversation_37");
    }
    else if (pBtn == ui.btn_TopWnd)
    {
        m_topWnd = !m_topWnd;
        if (m_topWnd)
        {
            //置顶图标
            ui.btn_TopWnd->setToolTip(tr("取消置顶"));
            ui.btn_TopWnd->setIcon(QIcon(":/Resources/SVG/btn_topwnd_hover.svg"));
            CommonSignalService::GetInstance()->DoShowToastMsg(tr("置顶成功！"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Common, QPoint());
            gIProtocolAgent->DoUploadOperatorLog("online_client_conversation_45");
        }
        else
        {
            //非置顶图标
            ui.btn_TopWnd->setToolTip(tr("置顶"));
            ui.btn_TopWnd->setIcon(QIcon(":/Resources/SVG/btn_topwnd_normal.svg"));
            CommonSignalService::GetInstance()->DoShowToastMsg(tr("取消置顶成功！"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Common, QPoint());
        }
        Q_EMIT S_SetMainWndTop(m_topWnd);
    }
    else
    {
        Q_ASSERT(false);
    }
}

void UI_Widget_AuxiliaryFun_Panenl::R_GetUnreadAnnouncementResult(QString code, QString reason, QString body, int count)
{
    m_nUnreadNotice = count;

    QString style = "QPushButton{border:0px;}"
                    "QPushButton:hover, QPushButton:pressed{border-image: url(:/YL/Resources/YL/TopRight/Btn_MouseHover.png);}";
    if (count > 0)
    {
        style = "QPushButton{border:0px;border-image: url(:/YL/Resources/YL/TopRight/icon_msgPrompt_normal.png);}"
                "QPushButton:hover, QPushButton:pressed{border-image: url(:/YL/Resources/YL/TopRight/icon_msgPrompt_hover.png);}";
    }
    ui.btn_msgPrompt->setStyleSheet(style);
}

void UI_Widget_AuxiliaryFun_Panenl::R_ReadOneNotice()
{
    m_nUnreadNotice--;
    if (m_nUnreadNotice < 1)
    {
        QString style = "QPushButton{border:0px;}"
                        "QPushButton:hover, QPushButton:pressed{border-image: url(:/YL/Resources/YL/TopRight/Btn_MouseHover.png);}";
        ui.btn_msgPrompt->setStyleSheet(style);
    }
}

void UI_Widget_AuxiliaryFun_Panenl::CreateAction(QMenu *menu, const QString &iconUri, const QString &text, const QString &data, int flag)
{
    {
        QAction *action = new QAction(QIcon(iconUri), text, this);
        connect(action, SIGNAL(triggered()), this, SLOT(R_OnMenuClicked()));
        action->setData(QVariant(data));
        menu->addAction(action);
    }
    if (flag == 1)
    {
        QAction *action = new QAction(menu);
        action->setSeparator(true);
        menu->addAction(action);
    }
}

void UI_Widget_AuxiliaryFun_Panenl::InitSkinMenu()
{
    QMenu *statemenu = new QMenu();
    CreateAction(statemenu, ":/Resources/SVG/menu_changeskin.svg", tr("皮肤"), "skin", 1);
    CreateAction(statemenu, ":/Resources/SVG/menu_changebackcolor.svg", tr("底色"), "backcolor");
    m_skinMenu = statemenu;
}

void UI_Widget_AuxiliaryFun_Panenl::InitCustomerMenu()
{
    QMenu *statemenu = new QMenu();
    CreateAction(statemenu, ":/Resources/SVG/menu_submitfeedback.svg", tr("提交反馈"), "submit");
    CreateAction(statemenu, ":/Resources/SVG/menu_viewfeedback.svg", tr("反馈记录"), "view");
    CreateAction(statemenu, ":/Resources/SVG/menu_onlinecustomer.svg", tr("在线客服"), "onlinehelp");
    CreateAction(statemenu, ":/Resources/SVG/menu_hotguide.svg", tr("Hot指南"), "hotguide");
    CreateAction(statemenu, ":/Resources/SVG/menu_novicetutorial.svg", tr("新手教程"), "novicetutorial");
    m_customerMenu = statemenu;
}

QString UI_Widget_AuxiliaryFun_Panenl::GetUrl(QString tempUri)
{
    static QString format = "?c=%2&userName=%3&version=%4&s=%5&platform=client&check=%6";
    QString        strUrl = format.arg(gSessionService->UserInfo()->CompanyId()).arg(gSessionService->UserInfo()->UserId()).arg(gUISystemConfigManager->GetCurrentVersion()).arg(QString("%1").arg(QDateTime::currentDateTime().toMSecsSinceEpoch())).arg(gSessionService->Token());
    strUrl                = tempUri + strUrl;
    return strUrl;
}

void UI_Widget_AuxiliaryFun_Panenl::OpenUrl(QString urltag)
{
    QString url    = gEasyLiaoFramework->GetHtmlNameByKey(urltag);
    QString strUrl = GetUrl(url);
    if (urltag == "uploadfeedback")
    {
        //增加手机号
        QString phone = gSessionService->GetValueByKey("phone");
        strUrl += QString("&phone=%1").arg(phone);
    }
    IUIController::GetInstance()->DoGotoUrl(strUrl);
}

void UI_Widget_AuxiliaryFun_Panenl::ShowNoticeUI()
{
    if (m_skinMenu)
    {
        QPoint pt = mapToGlobal(ui.btn_msgPrompt->pos());
        QPoint pos;
        pos.setX(pt.x());
        pos.setY(pt.y() + this->height());
        m_skinMenu->exec(pos);
    }
}

void UI_Widget_AuxiliaryFun_Panenl::R_OnMenuClicked()
{
    QAction *action = (QAction *)this->sender();
    QString  data   = action->data().toString();

    if (data == "skin")
    {
        Q_EMIT S_ChangeSkin_Clicked(0);
    }
    else if (data == "backcolor")
    {
        Q_EMIT S_ChangeSkin_Clicked(1);
    }
    else if (data == "submit")
    {
        OpenUrl("uploadfeedback");
    }
    else if (data == "view")
    {
        OpenUrl("viewfeedback");
    }
    else if (data == "onlinehelp")
    {
        CreateChatOnline();
    }
    else if (data == "hotguide")
    {
        Q_EMIT S_ShowNotice_Clicked();
    }
    else if (data == "novicetutorial")
    {
        //新手助手
        Q_EMIT S_Assistant_Clicked();
    }
    gIProtocolAgent->DoUploadOperatorLog("online_client_conversation_38");
}

void UI_Widget_AuxiliaryFun_Panenl::ShowSkinMenu()
{
    if (m_skinMenu)
    {
        QPoint pt = mapToGlobal(ui.btn_changeSkin->pos());
        QPoint pos;
        pos.setX(pt.x());
        pos.setY(pt.y() + this->height());
        m_skinMenu->exec(pos);
    }
}

void UI_Widget_AuxiliaryFun_Panenl::R_ShowCustomerMenu()
{
    if (m_customerMenu)
    {
        QPoint pt = mapToGlobal(ui.btn_consultCustomerOnline->pos());
        QPoint pos;
        pos.setX(pt.x());
        pos.setY(pt.y() + this->height());
        m_customerMenu->exec(pos);
    }
}

void UI_Widget_AuxiliaryFun_Panenl::R_SetUnreadMsgNum(int c, QString title, QString content, QString time)
{
    ui.btn_msgPrompt->setCount(c);
    CommonSignalService::GetInstance()->DoShowAnnouncementNotice(title, content, time);
}
