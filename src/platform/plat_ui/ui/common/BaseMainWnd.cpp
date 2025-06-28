#include "stdafx.h"
#include "BaseMainWnd.h"
#include "UI_Widget_Lock.h"


#include "utils/ConstStringHelper.h"
#include "utils/DefineHelper.h"
#include <QMessageBox>
#include "ui/common/UI_WebWidget.h"

#include "common/UI_WebWidget.h"
#include "data/UINewVersionInfo.h"

#include "protocol/httpcmd.h"
#include "session/el_menunavinterface.h"
#include "session/el_sessionservice.h"
#include "BaseWnd.h"
#include "interface/easyliaomainwnd.h"

#include "ui_baseMainWnd.h"
#include "ui/pop/ui_upgrade_reminder.h"

#include "utils/utilshelper.h"
#include <QApplication>
#include <QScreen>
#include "server/platuiservice.h"
#include "data/UIMenuDataInfo.h"

#ifdef _MSC_VER
#include <windows.h>
#endif

bool BaseMainWnd::m_isLoadLockScreen = false;

BaseMainWnd::BaseMainWnd(QWidget *parent)
    : IBaseMainWnd(parent)
{
    ui = new Ui::BaseMainWndClass;
    ui->setupUi(this);
    m_currentPresenceStatus = E_ONLINE;
    setWindowFlags(windowFlags() | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
    ui->m_topGridLayout->setMargin(GetShadowSize());

    m_isCanMax        = true;
    m_twinkleTimer    = nullptr;
    m_isTwinkle       = false;
    m_systrayIcon     = nullptr;
    m_IsMaxisize      = false;
    m_closeType       = 0;
    m_centerWidget    = nullptr;
    m_firstpoit       = true;
    m_Update_reminder = true;
    m_menunav         = nullptr;

    ui->m_maxBtn->setVisible(!m_IsMaxisize);
    ui->m_storeBtn->setVisible(m_IsMaxisize);

    SetTitle(false);
    SetCommonBtnDefault();

    connect(ui->widget_head, SIGNAL(S_OnMouseDoubleClicked(QMouseEvent *)), this, SLOT(R_OnMouseDoubleClicked(QMouseEvent *)));
    connect(ui->widget_head, SIGNAL(S_OnMouseMoveEvent(QMouseEvent *)), this, SLOT(R_OnMouseMoveEvent(QMouseEvent *)));
    connect(ui->widget_head, SIGNAL(S_OnMouseReleaseEvent(QMouseEvent *)), this, SLOT(R_OnMouseReleaseEvent(QMouseEvent *)));
    connect(ui->m_closeBtn, SIGNAL(clicked()), this, SLOT(R_OnCloseBtn_Clicked()));
    connect(ui->m_minBtn, SIGNAL(clicked()), this, SLOT(R_OnMinBtn_Clicked()));
    connect(ui->m_maxBtn, SIGNAL(clicked()), this, SLOT(R_OnMaxBtn_Clicked()));
    connect(ui->m_storeBtn, SIGNAL(clicked()), this, SLOT(R_OnStoreBtn_Clicked()));
    connect(ui->btnUpdate, SIGNAL(clicked()), this, SLOT(R_BtnUpdate_Clicked()));

    SetWndTitle("Easy liao");

    // CZC:设置按钮提示信息
    ui->m_minBtn->setToolTip(tr("最小化"));
    ui->m_storeBtn->setToolTip(tr("向下还原"));
    ui->m_maxBtn->setToolTip(tr("最大化"));
    ui->m_closeBtn->setToolTip(tr("关闭"));
    ui->btnUpdate->setVisible(false);
    m_widgetPages["webcall_online"] = 0;
    ui->m_leftmenuNav->setVisible(false);
}

BaseMainWnd::~BaseMainWnd()
{
    if (m_systrayIcon)
    {
        m_systrayIcon->deleteLater();
        m_systrayIcon = nullptr;
    }
    if (m_twinkleTimer)
    {
        m_twinkleTimer->deleteLater();
        m_twinkleTimer = nullptr;
    }
}

bool BaseMainWnd::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
#ifdef _MSC_VER    
    if (message)
    {
        MSG *msg = static_cast<MSG *>(message);
        if (msg->message == WM_HOTKEY)
        {
            const quint32 keycode     = HIWORD(msg->lParam);
            const quint32 modifiers   = LOWORD(msg->lParam);
            quint32       qtKey       = UtilsHelper::NativeKeyCode(keycode);
            quint32       qtModifiers = UtilsHelper::NativeModifiers(modifiers);
            Q_EMIT S_ActivateShortcut(qtKey, qtModifiers);
            return true;
        }
        else if (msg->message == WM_GETMINMAXINFO)
        {
        }
    }
#endif    
    return QWidget::nativeEvent(eventType, message, result);
}

void BaseMainWnd::R_OnCloseBtn_Clicked()
{
    if (m_closeType == 1)
    {
        Q_EMIT S_OnClose();
        this->close();
    }
    else if (m_closeType == 2)
    {
        Q_EMIT S_OnQuit();
    }
    else if (m_closeType == 3)
    {
        Q_EMIT S_OnQuit();
    }
    else if (m_closeType == 4)
    {
        //人机结合新增窗口
        if (m_centerWidget)
        {
            m_centerWidget->setParent(nullptr);
            delete m_centerWidget;
            m_centerWidget = nullptr;
        }
        Q_EMIT S_OnClose();
        this->repaint();
        this->hide();
    }
    else if (m_closeType == 5)
    {
        //对话监控
        this->hide();
    }
    else if (m_closeType == 6)
    {
        Q_EMIT S_OnClose();
        this->close();
    }
    else
    {
        if (m_centerWidget)
        {
            delete m_centerWidget;
            m_centerWidget = nullptr;
        }
        Q_EMIT S_OnClose();
        this->close();
        this->deleteLater();
    }
}

void BaseMainWnd::R_OnMinBtn_Clicked()
{
    showMinimized();
    overrideWindowState(Qt::WindowMinimized);
}

void BaseMainWnd::R_OnMaxBtn_Clicked()
{
    m_IsMaxisize = true;
    ui->m_maxBtn->setVisible(!m_IsMaxisize);
    ui->m_storeBtn->setVisible(m_IsMaxisize);

    SetMargin(0);
    SetDragEnable(false);
    SetShadowEnable(false);
    m_rect = geometry();

    //以下代码处理 副屏中软件全屏时会遮挡任务栏问题
    int index = QApplication::desktop()->screenNumber(this);
    if (index < 0)
    {
        index = 0;
    }
    QScreen *sc = QGuiApplication::screens()[index];
    QRect    rc = sc->availableGeometry();
    overrideWindowState(Qt::WindowMaximized);
    setGeometry(rc);
    SetMoveEnable(false);
    QTimer::singleShot(300, this, SLOT(R_ShowUserCheck()));
}

void BaseMainWnd::R_OnStoreBtn_Clicked()
{
    m_IsMaxisize = false;
    ui->m_maxBtn->setVisible(!m_IsMaxisize);
    ui->m_storeBtn->setVisible(m_IsMaxisize);
    SetMargin(GetShadowSize());
    SetDragEnable(true);
    SetShadowEnable(true);
    overrideWindowState(Qt::WindowNoState);
    setGeometry(m_rect);
    SetMoveEnable(true);
}

void BaseMainWnd::SetWndTitle(QString title)
{
    ui->m_titleTip->setText(title);
    DoSetWindowTitle(title);
}

void BaseMainWnd::SetWndIcon(const QString &iconUrl)
{
    QIcon icon;
    icon.addFile(iconUrl, QSize(), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);
}

void BaseMainWnd::AddContentWidget(QWidget *wnd, bool isVisible /*=true*/)
{
    if (wnd)
    {
        resize(wnd->size());
        m_centerWidget = wnd;

        ui->m_contentLayout->addWidget(wnd);
        wnd->setVisible(isVisible);

        connect(wnd, SIGNAL(S_Closed()), this, SLOT(close()));
        connect(wnd, SIGNAL(S_HideWndAndDelCenterWidget()), this, SLOT(R_OnCloseBtn_Clicked()));
    }
}

void BaseMainWnd::AddMenuNavWidget(El_MenuNavInterface *wnd, bool isVisible /*=true*/)
{
    if (wnd)
    {
        //resize(wnd->size());
        ui->m_leftmenuLayout->addWidget(wnd);

        wnd->setVisible(isVisible);
        if (m_menunav)
        {
            disconnect(m_menunav, SIGNAL(s_navClick(QString)), this, SLOT(R_clickMenuNav(QString)));
        }
        m_menunav = wnd;
        connect(m_menunav, SIGNAL(s_navClick(QString)), this, SLOT(R_clickMenuNav(QString)));


		QMap<QString, UIMenuDataInfo *> map = wnd->GetMenuData();
		if (map.find("webcall_web") != map.end())
		{
			//if (map["webcall_web"]->Opentype() == 2)
			{
				R_clickMenuNav("webcall_web");
			}
		}
        //暂时屏蔽掉 腾讯版本tx
        ui->m_leftmenuNav->setVisible(true);
    }

}

void BaseMainWnd::RemoveCenterWidget()
{
    if (m_centerWidget)
    {
        ui->m_contentLayout->removeWidget(m_centerWidget);
        disconnect(m_centerWidget, SIGNAL(S_Closed()), this, SLOT(close()));
        disconnect(m_centerWidget, SIGNAL(S_HideWndAndDelCenterWidget()), this, SLOT(R_OnCloseBtn_Clicked()));

        m_centerWidget = nullptr;
    }
}

void BaseMainWnd::DeleteCenterWidget()
{
    if (m_centerWidget)
    {
        ui->m_contentLayout->removeWidget(m_centerWidget);
        disconnect(m_centerWidget, SIGNAL(S_Closed()), this, SLOT(close()));
        disconnect(m_centerWidget, SIGNAL(S_HideWndAndDelCenterWidget()), this, SLOT(R_OnCloseBtn_Clicked()));

        if (m_centerWidget)
        {
            delete m_centerWidget;
            m_centerWidget = nullptr;
        }
    }
}

QWidget *BaseMainWnd::ContentWidget()
{
    return m_centerWidget;
}

bool BaseMainWnd::HasCenterWidget()
{
    if (m_centerWidget)
    {
        return true;
    }
    return false;
}

void BaseMainWnd::AddExtendBtnWidget(QWidget *wnd, bool isVisible)
{
    connect(gIProtocolAgent, SIGNAL(S_GetSoftVersionResult(QString, QString, QString)), this, SLOT(R_GetSoftVersionResult(QString, QString, QString)));
    ui->m_extendBtnLayout->addWidget(wnd);
    wnd->setVisible(isVisible);
}

void BaseMainWnd::R_BubbleTipShowMessage(QString title, QString msg)
{
    if (m_systrayIcon && m_systrayIcon->supportsMessages())
    {
        m_systrayIcon->showMessage(title, msg, QSystemTrayIcon::Information, 8000);
    }
}

void BaseMainWnd::WndClear()
{
    if (m_systrayIcon)
    {
        m_systrayIcon->hide();
        m_systrayIcon->deleteLater();
        m_systrayIcon = nullptr;
    }
}

void BaseMainWnd::InitSystemTrayIcon(bool isLogined)
{
    if (!QSystemTrayIcon::isSystemTrayAvailable()) //判断系统是否支持系统托盘图标
    {
        return;
    }
    //建立系统托盘图标
    QIcon icon = QIcon(":/Resources/Image/easy.ico");
    setWindowIcon(icon);
    if (m_systrayIcon == nullptr)
    {
        m_systrayIcon = new QSystemTrayIcon(this);
        SetUserStateByKeepAlive(m_currentPresenceStatus);
        CreateTrayIcon(isLogined);
        m_systrayIcon->show();
        connect(m_systrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(R_OnIconActivated(QSystemTrayIcon::ActivationReason)));
    }
}

void BaseMainWnd::CreateTrayIcon(bool isLogined)
{
    //设置右键点击时弹出的菜单
    m_trayIconMenu = new QMenu();
    connect(m_trayIconMenu, SIGNAL(aboutToShow()), this, SLOT(R_aboutToShow()));
    if (isLogined)
    {
        CreateTrayAction(m_trayIconMenu, "", tr("打开主窗口"), ConstStringHelper::m_cOpenMainWnd);
        CreateTrayAction(m_trayIconMenu, ":/Resources/SVG/a_online_state.svg", tr("在线"), ConstStringHelper::m_cOnline);
        CreateTrayAction(m_trayIconMenu, ":/Resources/SVG/a_busy_state.svg", tr("忙碌"), ConstStringHelper::m_cBusy);
        CreateTrayAction(m_trayIconMenu, ":/Resources/SVG/a_leave_state.svg", tr("离开"), ConstStringHelper::m_cLeave);
        CreateTrayAction(m_trayIconMenu, "", tr("退出"), ConstStringHelper::m_cQuit);
    }
    else
    {
        QString titleStr = QString("Easy Liao");
        m_systrayIcon->setToolTip(titleStr);
        CreateTrayAction(m_trayIconMenu, "", tr("打开主窗口"), ConstStringHelper::m_cOpenMainWnd);
        CreateTrayAction(m_trayIconMenu, "", tr("退出"), ConstStringHelper::m_cQuit);
    }
    SetSystrayIconTooltip(this->windowTitle());
    m_systrayIcon->setContextMenu(m_trayIconMenu);
}

void BaseMainWnd::CreateTrayAction(QMenu *menu, const QString &iconUri, const QString &text, const QString &data)
{
    QAction *action = new QAction(QIcon(iconUri), text, this);
    connect(action, SIGNAL(triggered()), this, SLOT(R_OnTrayMenuClicked()));
    action->setData(QVariant(data));
    menu->addAction(action);
}

void BaseMainWnd::R_OnTrayMenuClicked()
{
    QAction *action = (QAction *) this->sender();
    QString  data   = action->data().toString();
    if (StringCompare(ConstStringHelper::m_cOnline, data) || StringCompare(ConstStringHelper::m_cBusy, data) || StringCompare(ConstStringHelper::m_cLeave, data))
    {
        EPRESENCESTATUS state = UtilsHelper::GetPresenceStatusByString(data);
        R_StatusChange(state);
    }
    else if (StringCompare(ConstStringHelper::m_cOpenMainWnd, data))
    {
        gIProtocolAgent->DoUploadOperatorLog("online_client_conversation_50");
        R_OnOpenMainWndWnd();
    }
    else if (StringCompare(ConstStringHelper::m_cQuit, data))
    {
        Q_EMIT S_OnQuit();
        gIProtocolAgent->DoUploadOperatorLog("online_client_conversation_49");
    }
    else
    {
        // do noting;
        Q_ASSERT(false);
    }
}

void BaseMainWnd::R_StatusChange(EPRESENCESTATUS state)
{
//        SetUserStateByKeepAlive(E_WAITING);
    CommonSignalService::Instance()->DoStateChange(m_currentPresenceStatus, state);
}

void BaseMainWnd::SetSystrayIconTooltip(QString tip)
{
    if (m_systrayIcon)
    {
        m_systrayIcon->setToolTip(tip);
    }
}

void BaseMainWnd::R_OnOpenMainWndWnd()
{
    showNormal();
    activateWindow();
    raise();
    if (!getOnTopHint()) //修复,偶尔无法置顶问题
    {
#ifdef _MSC_VER        
        ::SetWindowPos(HWND(this->winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
        ::SetWindowPos(HWND(this->winId()), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
#endif        
    }
}

void BaseMainWnd::R_OnIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    //触发后台图标执行相应事件
    switch (reason)
    {
        case QSystemTrayIcon::Trigger:
            {
                if (this->isMinimized())
                {
                    Q_EMIT S_DoubleClickTrayIcon();
                }
                StopTwinkle();
                R_OnOpenMainWndWnd();
                arouseBaseMainWnd();
            }
            break;
        default:
            break;
    }
}

void BaseMainWnd::R_aboutToShow()
{
    gIProtocolAgent->DoUploadOperatorLog("online_client_conversation_48");
}

void BaseMainWnd::SetDeleteOnClose(bool isOk)
{
    this->setAttribute(Qt::WA_DeleteOnClose, isOk);
}

void BaseMainWnd::SetUserStateByKeepAlive(EPRESENCESTATUS state)
{
    m_currentPresenceStatus = state;
    switch (state)
    {
        case E_ONLINE:
            {
                QIcon icon = QIcon(":/Resources/Image/easy.ico");
                if (m_systrayIcon)
                {
                    m_systrayIcon->setIcon(icon);
                }
            }
            break;
        case E_BUSY:
            {
                QIcon icon = QIcon(":/Resources/Image/easy_busy.ico");
                if (m_systrayIcon)
                {
                    m_systrayIcon->setIcon(icon);
                }
            }
            break;
        case E_LEAVE:
            {
                QIcon icon = QIcon(":/Resources/Image/easy_leave.ico");
                if (m_systrayIcon)
                {
                    m_systrayIcon->setIcon(icon);
                }
            }
            break;
        case E_WAITING:
            {
                QIcon icon = QIcon(":/Resources/Image/easy_switching.ico");
                if (m_systrayIcon)
                {
                    m_systrayIcon->setIcon(icon);
                }
            }
            break;
    }
}

void BaseMainWnd::SetWndModel()
{
    this->setWindowModality(Qt::ApplicationModal);
}

void BaseMainWnd::SetMinBtn(bool isVisible)
{
    ui->m_minBtn->setVisible(isVisible);
}

void BaseMainWnd::SetCloseBtn(bool isVisible)
{
    ui->m_closeBtn->setVisible(isVisible);
}

void BaseMainWnd::SetMaxBtn(bool isVisible)
{
    SetDragEnable(isVisible);
    m_isCanMax = isVisible;
    ui->m_maxBtn->setVisible(isVisible);
    ui->m_storeBtn->setVisible(isVisible);
}

void BaseMainWnd::SetTitle(bool visible)
{
    if (visible)
    {
        ui->m_titlePanel->show();
        ui->widget_head_1->setStyleSheet("QWidget#widget_head_1\n{\nbackground-color: rgba(0, 0, 0,85);\n}");
    }
    else
    {
        ui->m_titlePanel->hide();
        ui->widget_head_1->setStyleSheet("");
    }
}

void BaseMainWnd::SetMargin(int margin)
{
    ui->m_topGridLayout->setMargin(margin);
}

void BaseMainWnd::R_OnMouseDoubleClicked(QMouseEvent *event)
{
    Q_UNUSED(event);
    if (m_isCanMax) //页面可以最大化时,才支持双击操作
    {
        if (m_IsMaxisize)
        {
            R_OnStoreBtn_Clicked();
        }
        else
        {
            R_OnMaxBtn_Clicked();
        }
    }
}

void BaseMainWnd::R_OnMouseMoveEvent(QMouseEvent *event)
{
    int nY = QCursor::pos().y();
    if (m_isCanMax && m_IsMaxisize && nY < Response_Height)
    {
        reductionSize(false);
        QPoint point   = event->pos();
        double dXScale = double(this->width() - point.x()) / (double) this->width();
        R_OnStoreBtn_Clicked();

        //将窗口定位在鼠标位置
        int nPosX      = event->globalX();
        int nLeftWidth = this->width() * (1 - dXScale);
        int nX         = nPosX - nLeftWidth;
        this->move(nX, event->globalPos().y() - event->pos().y());
    }
    else
    {
        reductionSize();
    }
}

void BaseMainWnd::R_OnMouseReleaseEvent(QMouseEvent *event)
{
    if (m_isCanMax && event->globalY() == 0)
    {
        reductionSize(false);
        R_OnMaxBtn_Clicked();
    }
}

void BaseMainWnd::R_OnResetRegion(const QPoint &cursorGlobalPoint)
{
    this->Region(cursorGlobalPoint);
}

void BaseMainWnd::DoSetWindowTitle(QString title)
{
    setWindowTitle(title);
}

void BaseMainWnd::HideMaxBtn()
{
    SetMaxBtn(false);
}

void BaseMainWnd::HideMinBtn()
{
    SetMinBtn(false);
}

void BaseMainWnd::SetModal()
{
    SetWndModel();
}

void BaseMainWnd::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
}

void BaseMainWnd::showEvent(QShowEvent *event)
{
    if (m_firstpoit)
    {
        m_firstpoit = false;
        m_poit.setX(x());
        m_poit.setY(y());
    }
    else
    {
        arouseBaseMainWnd();
    }
    QWidget::showEvent(event);
}

void BaseMainWnd::arouseBaseMainWnd()
{
    if (ContentWidget())
    {
        QList<QScreen *> list = QGuiApplication::screens();
        if (ContentWidget()->objectName() == "EasyLiaoMainWnd")
        {
            //todo 待处理
            EasyLiaoMainWnd *bwnd = gViewService->CreateMainWnd();
            if (bwnd)
            {
                QRect  rect        = bwnd->getCenterwidget()->rect();
                QPoint topLeft     = bwnd->getCenterwidget()->mapToGlobal(rect.topLeft());
                QPoint topRight    = bwnd->getCenterwidget()->mapToGlobal(rect.topRight());
                QPoint bottomRight = bwnd->getCenterwidget()->mapToGlobal(rect.bottomRight());
                QPoint bottomLeft  = bwnd->getCenterwidget()->mapToGlobal(rect.bottomLeft());
                bool   tomax       = true;
                for (int i = 0; i < list.count(); ++i)
                {
                    QScreen *sc = list[i];
                    QRect    rc = sc->availableGeometry();
                    if (rc.contains(topLeft) || rc.contains(topRight) || rc.contains(bottomRight) || rc.contains(bottomLeft))
                    {
                        tomax = false;
                    }
                }
                if (tomax)
                {
                    R_OnMaxBtn_Clicked();
                    //以下代码处理 还原窗口时,默认位置修改
                    int index = QApplication::desktop()->screenNumber(this);
                    if (index < 0)
                    {
                        index = 0;
                    }
                    QScreen *sc = QGuiApplication::screens()[index];
                    QRect    rc = sc->availableGeometry();
                    int      w  = m_rect.width();
                    int      h  = m_rect.height();
                    m_rect.setX(rc.x() + 300);
                    m_rect.setY(rc.y() + 150);
                    m_rect.setWidth(w);
                    m_rect.setHeight(h);
                }
            }
        }
        else
        {
            QRect rect = this->geometry();
            int   w    = rect.width();
            int   h    = rect.height();
            int   x    = rect.x();
            int   y    = rect.y();
            rect.setLeft(x + w * 0.45);
            rect.setRight(x + w - w * 0.45);
            rect.setTop(y + h * 0.45);
            rect.setBottom(y + h - h * 0.45);
            QPoint topLeft     = rect.topLeft();
            QPoint topRight    = rect.topRight();
            QPoint bottomRight = rect.bottomRight();
            QPoint bottomLeft  = rect.bottomLeft();
            for (int i = 0; i < list.count(); ++i)
            {
                QScreen *sc = list[i];
                QRect    rc = sc->availableGeometry();
                if (rc.contains(topLeft) || rc.contains(topRight) || rc.contains(bottomRight) || rc.contains(bottomLeft))
                {
                    return;
                }
            }
            rect = this->geometry();
            rect.setLeft(x + w * 0.2);
            rect.setRight(x + w - w * 0.2);
            rect.setTop(y + h * 0.2);
            rect.setBottom(y + h - h * 0.2);
            topLeft     = rect.topLeft();
            topRight    = rect.topRight();
            bottomRight = rect.bottomRight();
            bottomLeft  = rect.bottomLeft();
            int conp    = 0;
            for (int i = 0; i < list.count(); ++i)
            {
                QScreen *sc = list[i];
                QRect    rc = sc->availableGeometry();
                if (rc.contains(topLeft))
                {
                    ++conp;
                }
                if (rc.contains(topRight))
                {
                    ++conp;
                }
                if (rc.contains(bottomRight))
                {
                    ++conp;
                }
                if (rc.contains(bottomLeft))
                {
                    ++conp;
                }
            }
            if (conp > 1)
            {
                return;
            }
            move(m_poit);
        }
    }
}

QRect BaseMainWnd::getWorkarea()
{
    QRect re;
    int   ah = ui->m_topGridLayout->margin();
    re.setX(ui->m_workarea->geometry().x() + ah);
    re.setY(ui->m_workarea->geometry().y() + ah);
    re.setSize(ui->m_workarea->geometry().size());
    return re;
}

int BaseMainWnd::getmargin()
{
    return ui->m_topGridLayout->margin();
}

void BaseMainWnd::ShowMaxWnd()
{
    show();
    R_OnMaxBtn_Clicked();
    Q_EMIT S_SetPanelScale();
}

void BaseMainWnd::SetTitleStyle(const QString &style)
{
    QString uri = QString("QWidget#m_titlePanel%1").arg(style);
    ui->m_titlePanel->setStyleSheet(uri);
}

void BaseMainWnd::SetBackPanelIcon(const QString &iconUrl)
{

    QString uri = QString("QWidget#m_bkPanel{border-image: url(%1);}").arg(iconUrl);
    ui->m_bkPanel->setStyleSheet(uri);
}

void BaseMainWnd::SetTitlePanelIcon(const QString &iconUrl)
{
    QString uri = QString("QWidget#m_titleIcon{image: url(%1);}").arg(iconUrl);
    ui->m_titleIcon->setStyleSheet(uri);
}

void BaseMainWnd::SetTitleIconVisible(bool isVisible)
{
    ui->m_titleIcon->setVisible(isVisible);
}

void BaseMainWnd::SetTitleTextVisible(bool isVisible)
{
    ui->m_titleTip->setVisible(isVisible);
}

void BaseMainWnd::SetTitlePanelVisible(bool isVisible)
{
    ui->m_titlePanel->setVisible(isVisible);
}

int BaseMainWnd::GetCloseType() const
{
    return m_closeType;
}

void BaseMainWnd::SetCloseType(int val)
{
    m_closeType = val;

    if (!BaseMainWnd::m_isLoadLockScreen)
    {
        disconnect(CommonSignalService::Instance(), SIGNAL(S_TopRight_LockScreen_Clicked()), this, SLOT(R_TopRight_LockScreen_Clicked()));
        connect(CommonSignalService::Instance(), SIGNAL(S_TopRight_LockScreen_Clicked()), this, SLOT(R_TopRight_LockScreen_Clicked()));
        BaseMainWnd::m_isLoadLockScreen = true;
    }
}

void BaseMainWnd::SetTitleTextStyle(const QString &style)
{
    QString uri = QString("QLabel#m_titleTip%1").arg(style);
    ui->m_titleTip->setStyleSheet(uri);
}

void BaseMainWnd::SetCommonButtonStyle(QString type, QString btnName)
{
    QString style = "";

    QString strFormat = "QPushButton{\
                                                border:0px;\
                                                image: url(:/Resources/Image/t_%1_%2_btn.png);\
                                                background-color: rgba(255, 255, 255, 0);\
                                                }\
                                                QPushButton:hover{\
                                                image: url(:/Resources/Image/t_%3_%2_btn.png);\
                                                border-image: url(:/Resources/Image/Btn_%4_MouseHover.png);\
                                                border:0px;\
                                                }\
                                                QPushButton:pressed{\
                                                image: url(:/Resources/Image/t_%3_%2_btn.png);\
                                                border-image: url(:/Resources/Image/Btn_%4_MouseHover.png);\
                                                border:0px;\
                                                }\
                        ";

    if (btnName == "min")
    {
        if (type == "black")
        {
            style = strFormat.arg(type).arg(btnName).arg(type).arg(type);
        }
        else
        {
            style = strFormat.arg(type).arg(btnName).arg(type).arg(type);
        }
        ui->m_minBtn->setStyleSheet(style);
    }
    else if (btnName == "max")
    {
        if (type == "black")
        {
            style = strFormat.arg(type).arg(btnName).arg(type).arg(type);
        }
        else
        {
            style = strFormat.arg(type).arg(btnName).arg(type).arg(type);
        }
        ui->m_maxBtn->setStyleSheet(style);
    }
    else if (btnName == "store")
    {
        if (type == "black")
        {
            style = strFormat.arg(type).arg(btnName).arg(type).arg(type);
        }
        else
        {
            style = strFormat.arg(type).arg(btnName).arg(type).arg(type);
        }
        ui->m_storeBtn->setStyleSheet(style);
    }
    else if (btnName == "close")
    {
        if (type == "black")
        {
            style = strFormat.arg(type).arg(btnName).arg("white").arg("red");
        }
        else
        {
            style = strFormat.arg(type).arg(btnName).arg(type).arg("red");
        }
        ui->m_closeBtn->setStyleSheet(style);
    }
}

void BaseMainWnd::SetCommonBtnDefault()
{
    SetCommonButtonStyle("black", "min");
    SetCommonButtonStyle("black", "max");
    SetCommonButtonStyle("black", "store");
    SetCommonButtonStyle("black", "close");
}

void BaseMainWnd::StartTwinkle()
{
    if (m_twinkleTimer == nullptr)
    {
        m_twinkleTimer = new QTimer();
        m_twinkleTimer->setInterval(500);
        connect(m_twinkleTimer, SIGNAL(timeout()), this, SLOT(R_OnTwinkleTimeOut()));
    }
    if (!m_twinkleTimer->isActive())
    {
        m_twinkleTimer->start();
    }
}

void BaseMainWnd::R_OnTwinkleTimeOut()
{
    m_isTwinkle = !m_isTwinkle;

    if (m_isTwinkle)
    {
        SetUserStateByKeepAlive(m_currentPresenceStatus);
    }
    else
    {
        QIcon icon = QIcon(":/Resources/Image/空白.ico"); // qt5 使用空字符串的话,不是有效的icon所以无法展示出希望的闪动效果
        if (m_systrayIcon)
        {
            m_systrayIcon->setIcon(icon);
        }
    }
}

void BaseMainWnd::R_TopRight_LockScreen_Clicked()
{
    UI_Widget_Lock *wnd = new UI_Widget_Lock(ui->m_bkPanel);
    wnd->setGeometry(ui->m_bkPanel->rect());
    wnd->show();
}

void BaseMainWnd::R_Update_package_download_completed(QString code, QString reason, QByteArray body) //更新包下载完成
{
    HttpCmd *cmd = (HttpCmd *) QObject::sender();
    if (cmd)
    {
        UINewVersionInfo *version = gUISystemConfigManager->NewVersion();
        if (version)
        {
            if (code == "200") //更新包下载成功后,显示升级按钮
            {
                QString fileName = cmd->GetCmdDataByKey("fileName");
                QFile   file(fileName);
                if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
                {
                    file.write(body);
                }
                file.close();
                //ui.btnUpdate->show();
                version->DownloadStatus(EDOWNLOADSTATUS_DOWNLOADSUCCESSFUL); //更新下载状态 成功
                version->Local_filepath(fileName);

                if (m_Update_reminder) //更新强制提醒 只有在软件启动时,提醒一次
                {
                    R_BtnUpdate_Clicked();
                }
            }
            else
            {
                version->DownloadStatus(EDOWNLOADSTATUS_DOWNLOADFAILED); //更新下载状态 失败
            }
        }
        cmd->deleteLater();
        cmd               = NULL;
        m_Update_reminder = false;
        CommonSignalService::Instance()->DoUpdate_package_download_completed(); //用于通知 软件更新页面   UIYL_SoftwareUPRemindPanel
    }
}

void BaseMainWnd::StopTwinkle()
{
    if (m_twinkleTimer)
    {
        m_twinkleTimer->stop();
    }
    m_isTwinkle = false;
    SetUserStateByKeepAlive(m_currentPresenceStatus);
}

void BaseMainWnd::HideCloseBtn()
{
    this->SetCloseBtn(false);
}

void BaseMainWnd::R_ShowUserCheck()
{
    gSessionService->ShowAccountCheck(this);
}

void BaseMainWnd::R_GetSoftVersionResult(QString code, QString reason, QString body)
{
    UINewVersionInfo *version = gUISystemConfigManager->NewVersion();
    if (version)
    {
        QStringList newverlist = version->Version().split(".");

        int newflag = 0;

        if (newverlist.length() == 4)
        {
            QStringList verlist = gUISystemConfigManager->GetCurrentVersion().split(".");
            if (verlist.length() == 4)
            {
                if (newverlist[2].toInt() > verlist[2].toInt())
                {
                    newflag = 1;
                }
                else if ((newverlist[2].toInt() == verlist[2].toInt()) && (newverlist[3].toInt() > verlist[3].toInt()))
                {
                    newflag = 1;
                }
            }
        }
        if (newflag == 0)
        {
            m_Update_reminder = false;
            version->DownloadStatus(EDOWNLOADSTATUS_NOTSTARTED);                       //更新下载状态
            CommonSignalService::Instance()->DoUpdate_package_download_completed(); //用于通知 软件更新页面   UIYL_SoftwareUPRemindPanel
        }

        {
            int val = gUISystemConfigManager->AutoRemindUpdate();
            if (val == 0)
                newflag = 0;
        }
        if (newflag == 1) //有更新时提醒我配置打开
        {
            //下载更新包
            HttpCmd *cmd = new HttpCmd;
            connect(cmd, SIGNAL(S_SourceFinished(QString, QString, QByteArray)), this, SLOT(R_Update_package_download_completed(QString, QString, QByteArray)));
            connect(cmd, SIGNAL(S_DownLoadProgress(qint64, qint64)), CommonSignalService::Instance(), SIGNAL(S_Update_package_download_progress(qint64, qint64)));

            QString fileName = UtilsHelper::GetAppDataUpdateDir() + "/" + version->Version() + ".zip";
            cmd->AddCmdData("fileName", fileName);
            cmd->SetHeader("Content-Type", "application/octet-stream");
            cmd->Uri(version->DownloadUrl());
            cmd->Get();
        }
    }
}

void BaseMainWnd::R_BtnUpdate_Clicked()
{
    BaseWnd *baseWnd = new BaseWnd(0);
    baseWnd->HideMaxBtn();
    baseWnd->HideMinBtn();
    baseWnd->SetWndTitle(tr("软件升级"));
    baseWnd->setFixedSize(400 + 20, 375 + 20); //增加边框阴影

    UI_Upgrade_Reminder *newWnd = new UI_Upgrade_Reminder;
    baseWnd->AddContentWidget(newWnd);
    baseWnd->ShowCenter_Model();
}

QWidget *BaseMainWnd::Widget()
{
    return this;
}

void BaseMainWnd::DoOnOpenMainWndWnd()
{
    R_OnOpenMainWndWnd();
}

void BaseMainWnd::R_clickMenuNav(QString key)
{
    if (key == "exit")
    {
        Q_EMIT S_OnQuit();
        gIProtocolAgent->DoUploadOperatorLog("online_client_conversation_49");
        return;
    }
    if (m_widgetPages.find(key) == m_widgetPages.end())
    {
        UIMenuDataInfo *info = m_menunav->GetMenuData()[key];
        if (info)
        {
            //类型
            UI_WebWidget *web = new UI_WebWidget();
            web->InitData();
            int uino           = ui->stackedWidget->addWidget(web);
            m_widgetPages[key] = uino;
            web->SetUrl(GetUrl(info->Url()));
        }
    }
    SwitchContentPage(m_widgetPages[key]);
}

QString BaseMainWnd::GetUrl(QString tempUri)
{
    static QString format = "?c=%2&userName=%3&version=%4&s=%5&check=%6";
    QString        strUrl = format.arg(gSessionService->UserInfo()->CompanyId()).arg(gSessionService->UserInfo()->UserId()).arg(gUISystemConfigManager->GetCurrentVersion()).arg(QString("%1").arg(QDateTime::currentDateTime().toMSecsSinceEpoch())).arg(gSessionService->Token());
    strUrl                = tempUri.replace("${check}", strUrl);
    strUrl                = tempUri.replace("${access_token}", gSessionService->Token());
    return strUrl;
}

int BaseMainWnd::AddContentPage(QWidget *wnd)
{
    return 0;
}

void BaseMainWnd::SwitchContentPage(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}


void BaseMainWnd::LoadMenuNavWnd()
{
	QTimer::singleShot(2000, this,SLOT(R_LoadMenuNavWnd()));

}


void BaseMainWnd::R_LoadMenuNavWnd()
{
	El_MenuNavInterface * navwnd = gSessionService->GetMenuNavWnd();
	AddMenuNavWidget(navwnd, true);
}
