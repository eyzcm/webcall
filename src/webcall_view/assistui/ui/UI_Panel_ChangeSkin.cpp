#include "UI_Panel_ChangeSkin.h"
#include "QMetaObject"
#include "control/CommonSignalService.h"
#include "service/DbUiWebcallService.h"
#include "session/el_sessionservice.h"

UI_Panel_ChangeSkin::UI_Panel_ChangeSkin(QWidget *parent) : QWidget(parent), m_CurBtn(nullptr)
{
    ui.setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_ShowModal, true);
    Init();
    connect(gSessionService->LoginSignal(), SIGNAL(S_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)), this, SLOT(R_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_CurrentSkinChanged()), this, SLOT(R_ResetCurrentSkin()));
}

UI_Panel_ChangeSkin::~UI_Panel_ChangeSkin()
{
}

void UI_Panel_ChangeSkin::Init()
{
    QMap<QString, UISkinInfo *> mapSkin = gUIPersonalConfigYLService->GetSkinMap();
    GetCustomStructByDb(mapSkin);
    CreateChangeBtns();
}

void UI_Panel_ChangeSkin::GetCustomStructByDb(QMap<QString, UISkinInfo *> &mapSkin)
{
    foreach (UISkinInfo *info, mapSkin)
    {
        if (!m_mapSkinInfo.contains(info->GroupId()))
        {
            QMap<QString, UISkinInfo *> mapTemp;
            mapTemp.insert(info->Key(), info);
            m_mapSkinInfo.insert(info->GroupId(), mapTemp);
            m_mapGroupName.insert(info->GroupId(), info->GroupName());
            QVector<UISkinInfo *> vecInfo;
            vecInfo.push_back(info);
            m_mapSkinInfoByGroupName.insert(info->GroupName(), vecInfo);
        }
        else
        {
            if (!m_mapSkinInfo[info->GroupId()].contains(info->Key()))
            {
                m_mapSkinInfo[info->GroupId()].insert(info->Key(), info);
            }
            if (m_mapSkinInfoByGroupName.contains(info->GroupName()))
            {
                m_mapSkinInfoByGroupName[info->GroupName()].push_back(info);
            }
        }
    }
}

void UI_Panel_ChangeSkin::CreateChangeBtns()
{
    foreach (QString name, m_mapGroupName)
    {
        SwitchButton *btn = new SwitchButton;
        btn->setFixedSize(60, 30);
        btn->setObjectName(name);
        btn->setText(name);
        connect(btn, SIGNAL(clicked()), this, SLOT(R_ChangeBtn_Clicked()));
        ui.horizontalLayout->addWidget(btn);
        CreateStackedWidget(btn);
        if (name == m_mapGroupName[0])
        {
            InitSwitchButton(btn, true);
            ui.stackedWidget->setCurrentWidget(m_mapChangeBtnBindWidget[btn]);
        }
        else
        {
            InitSwitchButton(btn, false);
        }
    }
    ui.horizontalLayout->addStretch();
}

void UI_Panel_ChangeSkin::CreateStackedWidget(SwitchButton *btn)
{
    QGridLayout *btnLayout = new QGridLayout;
    btnLayout->setMargin(10);
    btnLayout->setHorizontalSpacing(10);
    btnLayout->setVerticalSpacing(10);

    QWidget *scrollWidget = new QWidget;
    scrollWidget->setLayout(btnLayout);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setFocusPolicy(Qt::NoFocus);
    scrollArea->setStyleSheet("QScrollArea{border:0px;background-color: rgba(255, 255, 255, 0);}");
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QVBoxLayout *widgetLayout = new QVBoxLayout;
    widgetLayout->setMargin(0);
    widgetLayout->setSpacing(0);
    widgetLayout->addWidget(scrollArea);

    QWidget *widget = new QWidget;
    widget->setFocusPolicy(Qt::NoFocus);
    widget->setLayout(widgetLayout);
    ui.stackedWidget->addWidget(widget);

    m_mapChangeBtnBindWidget.insert(btn, widget);

    if (m_mapSkinInfoByGroupName.contains(btn->objectName()))
    {
        QVector<UISkinInfo *> vecSkinInfo = m_mapSkinInfoByGroupName[btn->objectName()];
        for (int i = 0; i < vecSkinInfo.size(); ++i)
        {
            int row = i / 4;
            int col = i % 4;
            CreateSkinBtns(btnLayout, row, col, vecSkinInfo.at(i));
        }
    }
}

void UI_Panel_ChangeSkin::CreateSkinBtns(QGridLayout *btnLayout, int row, int col, UISkinInfo *info)
{
    UI_Btn_Skin *btn = new UI_Btn_Skin(info);
    btn->setFixedSize(80, 45);
    connect(btn, SIGNAL(clicked()), this, SLOT(R_SkinBtn_Clicked()));
    btnLayout->addWidget(btn, row, col);
    if (!m_mapSkinBtnByImgName.contains(info->Name()))
    {
        m_mapSkinBtnByImgName.insert(info->Name(), btn);
    }
    if (m_pCurSkinInfo == info)
    {
        btn->click();
    }
}

void UI_Panel_ChangeSkin::InitSwitchButton(SwitchButton *btn, bool isCheck)
{
    QString unCheckStyle = "QToolButton{font:12px;color:#333333;border:0;background:#e6e9ef;}QToolButton:hover,QToolButton:pressed{font-weight:bold;background-color:#ffffff;}";
    QString checkStyle   = "QToolButton,QToolButton:hover,QToolButton:pressed{font:12px;font-weight:bold;color:#333333;border:0;background-color:#ffffff;}";
    btn->SetCheckStyle(checkStyle);
    btn->SetUnCheckStyle(unCheckStyle);
    btn->IsBtnChecked(isCheck);
    btn->Init();
}

void UI_Panel_ChangeSkin::SetDefaultSkin(QString strName)
{
    if (m_mapSkinBtnByImgName.contains("bg_PureColor_1"))
    {
        UI_Btn_Skin *btn = m_mapSkinBtnByImgName["bg_PureColor_1"];
        if (btn)
        {
            btn->SetCheck(true);
            btn->update();
            gUIPersonalConfigYLService->CurrentSkin(btn->GetSkinInfo());
            Q_EMIT S_SkinBtn_Clicked(btn->GetFullPath());
        }
    }

    if (m_mapSkinBtnByImgName.contains(strName))
    {
        UI_Btn_Skin *btn = m_mapSkinBtnByImgName[strName];
        if (btn)
        {
            btn->SetCheck(false);
            btn->update();
        }
    }
}

void UI_Panel_ChangeSkin::R_ChangeBtn_Clicked()
{
    SwitchButton *btn = (SwitchButton *)this->sender();
    for (auto it = m_mapChangeBtnBindWidget.begin(); it != m_mapChangeBtnBindWidget.end(); ++it)
    {
        if (btn == it.key())
        {
            ui.stackedWidget->setCurrentWidget(it.value());
            it.key()->IsBtnChecked(true);
        }

        if (btn != it.key())
        {
            it.key()->IsBtnChecked(false);
        }
    }
}

void UI_Panel_ChangeSkin::R_SkinBtn_Clicked()
{
    UI_Btn_Skin *btn = (UI_Btn_Skin *)this->sender();
    btn->SetCheck(true);
    btn->update();
    gUIPersonalConfigYLService->CurrentSkin(btn->GetSkinInfo());
    Q_EMIT S_SkinBtn_Clicked(btn->GetFullPath());

    for (auto it = m_mapSkinBtnByImgName.begin(); it != m_mapSkinBtnByImgName.end(); ++it)
    {
        if (it.value() != btn)
        {
            it.value()->SetCheck(false);
            it.value()->update();
        }
    }
}

void UI_Panel_ChangeSkin::R_ResetCurrentSkin()
{
    if (m_CurBtn)
    {
        m_CurBtn->SetCheck(false);
    }

    m_pCurSkinInfo = gUIPersonalConfigYLService->CurrentSkin();
    if (m_mapSkinBtnByImgName.contains(m_pCurSkinInfo->Name()))
    {
        m_CurBtn = m_mapSkinBtnByImgName[m_pCurSkinInfo->Name()];
        m_CurBtn->SetCheck(true);
    }
}

void UI_Panel_ChangeSkin::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
}
void UI_Panel_ChangeSkin::leaveEvent(QEvent *event)
{
    this->hide();
    QWidget::leaveEvent(event);
}

void UI_Panel_ChangeSkin::R_LoginStateChanged(ELOGINSTATUS oldStatus, ELOGINSTATUS newStatus)
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
                R_ResetCurrentSkin();
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
