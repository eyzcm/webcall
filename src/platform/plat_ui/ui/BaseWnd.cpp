#include "BaseWnd.h"

#include "utils/ConstStringHelper.h"
#include "utils/DefineHelper.h"


#include "utils/utilshelper.h"

#include <QApplication>
#ifdef _MSC_VER
#include <windows.h>
#endif
#include <QWidget>
#include <QScreen>
#include <QDebug>
#include <ui_BaseWnd.h>


BaseWnd::BaseWnd(QWidget *parent) : MyWidgetBase(parent)
  , ui(new Ui::BaseWndClass)
{
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

    ui->m_maxBtn->setVisible(!m_IsMaxisize);
    ui->m_storeBtn->setVisible(m_IsMaxisize);

    SetTitle(false);
    SetBottomPanel(false);
    SetCommonBtnDefault();

    connect(ui->widget_head, SIGNAL(S_OnMouseDoubleClicked(QMouseEvent *)), this, SLOT(R_OnMouseDoubleClicked(QMouseEvent *)));
    connect(ui->widget_head, SIGNAL(S_OnMouseMoveEvent(QMouseEvent *)), this, SLOT(R_OnMouseMoveEvent(QMouseEvent *)));
    connect(ui->widget_head, SIGNAL(S_OnMouseReleaseEvent(QMouseEvent *)), this, SLOT(R_OnMouseReleaseEvent(QMouseEvent *)));

    connect(ui->m_closeBtn, SIGNAL(clicked()), this, SLOT(R_OnCloseBtn_Clicked()));
    connect(ui->m_minBtn, SIGNAL(clicked()), this, SLOT(R_OnMinBtn_Clicked()));
    connect(ui->m_maxBtn, SIGNAL(clicked()), this, SLOT(R_OnMaxBtn_Clicked()));
    connect(ui->m_storeBtn, SIGNAL(clicked()), this, SLOT(R_OnStoreBtn_Clicked()));


    SetWndTitle("Easy liao");
    // InitSystemTrayIcon(false);

    // CZC:设置按钮提示信息
    ui->m_minBtn->setToolTip(tr("最小化"));
    ui->m_storeBtn->setToolTip(tr("向下还原"));
    ui->m_maxBtn->setToolTip(tr("最大化"));
    ui->m_closeBtn->setToolTip(tr("关闭"));

    ui->btnUpdate->setVisible(false);
}

BaseWnd::~BaseWnd()
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

bool BaseWnd::nativeEvent(const QByteArray &eventType, void *message, long *result)
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
#else
//todo 9999
 #endif   
    return QWidget::nativeEvent(eventType, message, result);
}

void BaseWnd::R_OnCloseBtn_Clicked()
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
			m_centerWidget->deleteLater();
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
    else
    {
        if (m_centerWidget)
        {
            //m_centerWidget->deleteLater();
            m_centerWidget = nullptr;
        }

        Q_EMIT S_OnClose();
        this->close();
        this->deleteLater();
    }
}

void BaseWnd::R_OnMinBtn_Clicked()
{
    showMinimized();
    overrideWindowState(Qt::WindowMinimized);
}

void BaseWnd::R_OnMaxBtn_Clicked()
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

}

void BaseWnd::R_OnStoreBtn_Clicked()
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

void BaseWnd::SetWndTitle(QString title)
{
    ui->m_titleTip->setText(title);
    DoSetWindowTitle(title);
}

void BaseWnd::SetWndIcon(const QString &iconUrl)
{
    QIcon icon;
    icon.addFile(iconUrl, QSize(), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);
}
void BaseWnd::AddContentWidget(QWidget *wnd, bool isVisible /*=true*/)
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

void BaseWnd::RemoveCenterWidget()
{
    if (m_centerWidget)
    {
        ui->m_contentLayout->removeWidget(m_centerWidget);
        disconnect(m_centerWidget, SIGNAL(S_Closed()), this, SLOT(close()));
        disconnect(m_centerWidget, SIGNAL(S_HideWndAndDelCenterWidget()), this, SLOT(R_OnCloseBtn_Clicked()));


        m_centerWidget = nullptr;
    }
}

void BaseWnd::DeleteCenterWidget()
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

QWidget *BaseWnd::ContentWidget()
{
    return m_centerWidget;
}

bool BaseWnd::HasCenterWidget()
{
    if (m_centerWidget)
    {
        return true;
    }
    return false;
}


void BaseWnd::R_BubbleTipShowMessage(QString title, QString msg)
{
    if (m_systrayIcon && m_systrayIcon->supportsMessages())
    {
        m_systrayIcon->showMessage(title, msg, QSystemTrayIcon::Information, 8000);
    }
}

void BaseWnd::WndClear()
{
    if (m_systrayIcon)
    {
        m_systrayIcon->hide();
        m_systrayIcon->deleteLater();
        m_systrayIcon = nullptr;
    }
}

void BaseWnd::InitSystemTrayIcon(bool isLogined)
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
    // StartTwinkle();
}

void BaseWnd::CreateTrayIcon(bool isLogined)
{
    //设置右键点击时弹出的菜单
    m_trayIconMenu = new QMenu();
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

void BaseWnd::CreateTrayAction(QMenu *menu, const QString &iconUri, const QString &text, const QString &data)
{
    QAction *action = new QAction(QIcon(iconUri), text, this);
    connect(action, SIGNAL(triggered()), this, SLOT(R_OnTrayMenuClicked()));
    action->setData(QVariant(data));
    menu->addAction(action);
}

void BaseWnd::R_OnTrayMenuClicked()
{
    QAction *action = (QAction *)this->sender();
    QString  data   = action->data().toString();
    if (StringCompare(ConstStringHelper::m_cOnline, data) || StringCompare(ConstStringHelper::m_cBusy, data) || StringCompare(ConstStringHelper::m_cLeave, data))
    {
        auto stateData = UtilsHelper::GetPresenceStatusByString(data);
        SetUserStateByKeepAlive(stateData);
        Q_EMIT S_OnSetState(data);
    }
    else if (StringCompare(ConstStringHelper::m_cOpenMainWnd, data))
    {
        R_OnOpenMainWndWnd();
    }
    else if (StringCompare(ConstStringHelper::m_cQuit, data))
    {
        Q_EMIT S_OnQuit();
    }
    else
    {
        // do noting;
        Q_ASSERT(false);
    }
}

void BaseWnd::SetSystrayIconTooltip(QString tip)
{
    if (m_systrayIcon)
    {
        m_systrayIcon->setToolTip(tip);
    }
}

void BaseWnd::R_OnOpenMainWndWnd()
{
    showNormal();
    activateWindow();
}

void BaseWnd::R_OnIconActivated(QSystemTrayIcon::ActivationReason reason)
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


            arouseBaseWnd();
        }
        break;
        default:
            break;
    }
}

void BaseWnd::SetDeleteOnClose(bool isOk)
{
    this->setAttribute(Qt::WA_DeleteOnClose, isOk);
}

void BaseWnd::SetUserStateByKeepAlive(EPRESENCESTATUS state)
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
    }
}

void BaseWnd::SetWndModel()
{
    this->setWindowModality(Qt::ApplicationModal);
}

void BaseWnd::SetMinBtn(bool isVisible)
{
    ui->m_minBtn->setVisible(isVisible);
}

void BaseWnd::SetCloseBtn(bool isVisible)
{
    ui->m_closeBtn->setVisible(isVisible);
}

void BaseWnd::SetMaxBtn(bool isVisible)
{
    SetDragEnable(isVisible);
    m_isCanMax = isVisible;
    ui->m_maxBtn->setVisible(isVisible);
    ui->m_storeBtn->setVisible(isVisible);
}

void BaseWnd::SetBottomPanel(bool isVisible)
{
    ui->m_bottomPanel->setVisible(isVisible);
}

void BaseWnd::SetTitle(bool visible)
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

void BaseWnd::SetMargin(int margin)
{
    ui->m_topGridLayout->setMargin(margin);
}

void BaseWnd::R_OnMouseDoubleClicked(QMouseEvent *event)
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

void BaseWnd::R_OnMouseMoveEvent(QMouseEvent *event)
{
    int nY = QCursor::pos().y();
    if (m_isCanMax && m_IsMaxisize && nY < Response_Height)
    {
        QPoint point   = event->pos();
        double dXScale = double(this->width() - point.x()) / (double)this->width();
        R_OnStoreBtn_Clicked();

        //将窗口定位在鼠标位置
        int nPosX      = event->globalX();
        int nLeftWidth = this->width() * (1 - dXScale);
        int nX         = nPosX - nLeftWidth;
        this->move(nX, event->globalPos().y() - event->pos().y());
    }
}

void BaseWnd::R_OnMouseReleaseEvent(QMouseEvent *event)
{
    if (m_isCanMax && event->globalY() == 0)
    {
        R_OnMaxBtn_Clicked();
    }
}

void BaseWnd::R_OnResetRegion(const QPoint &cursorGlobalPoint)
{
    this->Region(cursorGlobalPoint);
}

void BaseWnd::DoSetWindowTitle(QString title)
{
    setWindowTitle(title);
}

void BaseWnd::HideMaxBtn()
{
    SetMaxBtn(false);
}

void BaseWnd::HideMinBtn()
{
    SetMinBtn(false);
}

void BaseWnd::SetModal()
{
    SetWndModel();
}

void BaseWnd::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
}

void BaseWnd::showEvent(QShowEvent *event)
{
    if (m_firstpoit)
    {
        m_firstpoit = false;
        m_poit.setX(x());
        m_poit.setY(y());
        qDebug() << this->objectName() << m_poit;
    }
    else
    {
        arouseBaseWnd();
    }
    QWidget::showEvent(event);
}

void BaseWnd::arouseBaseWnd()
{
    if (ContentWidget())
    {
        QList<QScreen *> list = QGuiApplication::screens();

    }
}

QRect BaseWnd::getWorkarea()
{
    QRect re;
    int ah = ui->m_topGridLayout->margin();
    re.setX(ui->m_workarea->geometry().x() + ah);
    re.setY(ui->m_workarea->geometry().y() + ah);
    re.setSize(ui->m_workarea->geometry().size());
    return re;
}

int BaseWnd::getmargin()
{
    return ui->m_topGridLayout->margin();
}

void BaseWnd::ShowMaxWnd()
{
    show();
    R_OnMaxBtn_Clicked();
    Q_EMIT S_SetPanelScale();
}

void BaseWnd::SetTitleStyle(const QString &style)
{
    QString uri = QString("QWidget#m_titlePanel%1").arg(style);
    ui->m_titlePanel->setStyleSheet(uri);
}

void BaseWnd::SetBackPanelIcon(const QString &iconUrl)
{
    QString uri = QString("QWidget#m_bkPanel{border-image: url(%1);}").arg(iconUrl);
    ui->m_bkPanel->setStyleSheet(uri);
}

void BaseWnd::SetTitlePanelIcon(const QString &iconUrl)
{
    QString uri = QString("QWidget#m_titleIcon{image: url(%1);}").arg(iconUrl);
    ui->m_titleIcon->setStyleSheet(uri);
}

void BaseWnd::SetTitleIconVisible(bool isVisible)
{
    ui->m_titleIcon->setVisible(isVisible);
}

void BaseWnd::SetTitleTextVisible(bool isVisible)
{
    ui->m_titleTip->setVisible(isVisible);
}

void BaseWnd::SetTitlePanelVisible(bool isVisible)
{
    ui->m_titlePanel->setVisible(isVisible);
}

int BaseWnd::GetCloseType() const
{
    return m_closeType;
}

void BaseWnd::SetCloseType(int val)
{
    m_closeType = val;


}

void BaseWnd::SetTitleTextStyle(const QString &style)
{
    QString uri = QString("QLabel#m_titleTip%1").arg(style);
    ui->m_titleTip->setStyleSheet(uri);
}

void BaseWnd::SetCommonButtonStyle(QString type, QString btnName)
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

void BaseWnd::SetCommonBtnDefault()
{
    SetCommonButtonStyle("black", "min");
    SetCommonButtonStyle("black", "max");
    SetCommonButtonStyle("black", "store");
    SetCommonButtonStyle("black", "close");
}


void BaseWnd::HideCloseBtn()
{
    this->SetCloseBtn(false);
}



