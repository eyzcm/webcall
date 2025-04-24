#include "UI_Widget_CommonWord_Panel.h"
#include "control/CommonSignalService.h"
#include "control/IUIController.h"
#include "control/geasyliaoframework.h"
#include "service/DbUiWebcallService.h"
#include "model/UICommonDataModel.h"
#include "model\YL_Model\UI_Model_CommonWord.h"
#include "uimessagebox.h"
#include "protocol_biz/webcallprotocolservice.h"
#include "session/el_sessionservice.h"
#include <QToolTip>

UI_Widget_CommonWord_Panel::UI_Widget_CommonWord_Panel(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    setMouseTracking(true);
    m_TextChanged = true;

    m_Sarea = new EL_TopScrollArea(this);
    m_Sarea->setSize(QSize(200, 220));
    m_Sarea->setVisible(false);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(Qt::gray);
    shadow->setBlurRadius(10);
    m_Sarea->setGraphicsEffect(shadow);
    connect(m_Sarea, &EL_TopScrollArea::S_ScrollArea_hide, this, &UI_Widget_CommonWord_Panel::R_ScrollArea_hide);

    m_contact = new MContactItemData();
    m_contact->Userid("company");
    m_contact->Name(tr("公司"));

    m_commonWordTreeSearchResultWidget = new UIYL_CommonWordTreeSearchResultWidget(this);
    m_commonWordTreeSearchResultWidget->hide();

    Init();
    //设置常用语是否可拖拽
    ui.treeView->CanDragDrop(true);
}

UI_Widget_CommonWord_Panel::~UI_Widget_CommonWord_Panel()
{
}

void UI_Widget_CommonWord_Panel::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    if (!ui.personalCommonword->isChecked() && !ui.companyCommonword->isChecked()) //页面首次显示时默认显示个人常用语
    {
        Q_EMIT ui.personalCommonword->clicked();
    }
}

void UI_Widget_CommonWord_Panel::Init()
{
    InitModel();

    connect(ui.edit_search, SIGNAL(textChanged(const QString &)), this, SLOT(R_OnCommonWordTreeSearchLineTextChanged(const QString &)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_GetCommonWordColleageListResult()), this, SLOT(R_ShowColleagueList()));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_SetBtnDeleteState(bool)), this, SLOT(R_SetBtnDeleteState(bool)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_DeleteCommonWordComplete()), this, SLOT(R_RefreshommonWordShowMode()));      //删除常用语后  刷新UI显示模式
    connect(CommonSignalService::GetInstance(), SIGNAL(S_AddCommonWordcomplete()), this, SLOT(R_RefreshommonWordShowMode()));         //添加常用语后  刷新UI显示模式
    connect(CommonSignalService::GetInstance(), SIGNAL(S_Commonword_UIRefresh_complete()), this, SLOT(R_RefreshommonWordShowMode())); //获取常用语后  刷新UI显示模式
    connect(gIProtocolAgent, SIGNAL(S_GetUserPermissionListResult(QString, QString, QString)), this, SLOT(R_GetUserPermissionListResult(QString, QString, QString)));

    R_SetEditWidgetVisible(false);
    connect(ui.treeView, SIGNAL(S_BatchEditState(bool)), this, SLOT(R_SetEditWidgetVisible(bool)));
    connect(ui.treeView, SIGNAL(S_AllCheckState(bool)), this, SLOT(R_AllCheckState(bool)));
    connect(ui.cbox_allCheck, SIGNAL(clicked()), this, SLOT(R_AllCheck_Clicked()));
    connect(ui.tbtn_delete, SIGNAL(clicked()), this, SLOT(R_DeleteCommonWord()));

    R_SetBtnDeleteState(false);                        //默认删除 按钮为未选中状态
    ui.personalCommonword->setWindowTitle("personal"); //个人常用语
    connect(ui.personalCommonword, SIGNAL(clicked()), this, SLOT(R_CustomSelectChanged()));


    m_showtypeMenu = new QMenu(this);
    connect(m_showtypeMenu, &QMenu::triggered, this, &UI_Widget_CommonWord_Panel::R_OnActionClicked);

    QAction *t = new QAction;
    t->setText(tr("只展示标题"));
    t->setData(Enum_YL_CtrlName_T);
    m_showtypeMenu->addAction(t);

    QAction *c = new QAction;
    c->setText(tr("只展示内容"));
    c->setData(Enum_YL_CtrlName_C);
    m_showtypeMenu->addAction(c);

    QAction *tc = new QAction;
    tc->setText(tr("标题 + 内容"));
    tc->setData(Enum_YL_CtrlName_TC);
    m_showtypeMenu->addAction(tc);

}

void UI_Widget_CommonWord_Panel::InitModel()
{
    UI_Model_CommonWord::GetInstance()->RegisterObserver(ui.treeView);
}

void UI_Widget_CommonWord_Panel::SetSearchEditGetFocus()
{
    ui.edit_search->setFocus();
}

//设置编辑模式面板显隐
void UI_Widget_CommonWord_Panel::R_SetEditWidgetVisible(bool visible)
{
    ui.cbox_allCheck->setChecked(false);
    R_SetBtnDeleteState(true);
    ui.widget_edit->setVisible(visible);
}

void UI_Widget_CommonWord_Panel::R_AllCheck_Clicked()
{
    ui.treeView->AllCheck_Clicked(ui.cbox_allCheck->isChecked());
    R_SetBtnDeleteState(ui.cbox_allCheck->isChecked());
}

void UI_Widget_CommonWord_Panel::R_DeleteCommonWord()
{
    UIMessageBox *wnd = new UIMessageBox;
    wnd->setWindowTitle(tr("提示"));
    wnd->SetContentTip(tr("确定删除已选常用语?"));
    wnd->SetCancelBtnVisible(false);
    if (wnd->exec() == 1)
    {
        R_SureBatchDelete(true);
    }
}

//设置全选按钮状态
void UI_Widget_CommonWord_Panel::R_AllCheckState(bool check)
{
    ui.cbox_allCheck->setChecked(check);
}

//确定批量删除常用语
void UI_Widget_CommonWord_Panel::R_SureBatchDelete(bool sure)
{
    if (sure)
    {
        ui.treeView->DeleteCommonWord();
        R_AllCheckState(false);
        R_SetBtnDeleteState(false);
        on_tbtn_cancel_clicked();
    }
}

void UI_Widget_CommonWord_Panel::R_SetBtnDeleteState(bool show)
{
    ui.tbtn_delete->setEnabled(show);
}

void UI_Widget_CommonWord_Panel::R_GetUserPermissionListResult(QString code, QString reason, QString body)
{
    Q_UNUSED(code);
    Q_UNUSED(reason);
    Q_UNUSED(body);
    CommInfo otherInfo(*gUIPersonalConfigManager->GetUserOtherSetting());
    QString  data = otherInfo.GetValueByKey("commonwordshowType");
    ui.treeView->UpdateView((Common_Expressions)data.toInt());

    ui.companyCommonword->setVisible(gSessionService->HasPermissionByKey("c_commonword_company")); //是否显示公司常用语按钮
}

QString UI_Widget_CommonWord_Panel::GetStringByFontMetrics(QString text)
{
    QFont font(tr("微软雅黑"));
    font.setPointSize(10);
    QFontMetrics fontMetrics(font);
    text = fontMetrics.elidedText(text, Qt::ElideRight, m_Sarea->width() - 20);
    return text;
}

void UI_Widget_CommonWord_Panel::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    if (m_commonWordTreeSearchResultWidget->isVisible())
    {
        m_commonWordTreeSearchResultWidget->setGeometry(0, ui.widget_search_rect->height(), width(), height() - ui.widget_search_rect->height());
    }
}

void UI_Widget_CommonWord_Panel::R_OnCommonWordTreeSearchLineTextChanged(const QString &text)
{
    if (m_TextChanged)
    {
        m_TextChanged = false;
        // if (text.isEmpty())
        // {
        //     m_commonWordTreeSearchResultWidget->hide();
        // }
        // else
        // {
        //     m_commonWordTreeSearchResultWidget->SearchText(text);
        //     m_commonWordTreeSearchResultWidget->setGeometry(0, ui.widget_search_rect->height(), width(), height() - ui.widget_search_rect->height());
        //     m_commonWordTreeSearchResultWidget->show();
        // }

        UI_Model_CommonWord::GetInstance()->setProperty("queryKey",text);


        UI_Model_CommonWord::GetInstance()->R_SetCommonWordinfo("");

        CommInfo otherInfo(*gUIPersonalConfigManager->GetUserOtherSetting());
        QString  data = otherInfo.GetValueByKey("commonwordshowType");
        ui.treeView->UpdateView((Common_Expressions)data.toInt());
        m_TextChanged = true;
    }
}

void UI_Widget_CommonWord_Panel::R_ShowColleagueList() //常用列表语获取完成
{
    QList<QPushButton *> m_list;
    QString              sy = "QPushButton\
    {\
            padding-bottom : 10px;\
            padding-top : 10px;\
            padding-left : 19px;\
            text-align : left;\
            font-size: 14px;  \
            color : rgba(96, 98, 102, 255);\
            background-color: rgba(255, 255, 255, 0);\
    }    \
        QPushButton::hover\
    {   \
            background-color: rgba(248, 248, 253, 255);\
    }  ";

    QPushButton *button = new QPushButton(m_Sarea);
    button->setStyleSheet(sy);
    button->setFixedSize(180, 34);
    button->setText(tr("公司"));
    button->setWindowTitle("company"); //公司常用语
    button->setFocusPolicy(Qt::NoFocus);
    connect(button, &QPushButton::clicked, this, &UI_Widget_CommonWord_Panel::R_CustomSelectChanged);
    m_list.append(button);

    //向m_Sarea中追加列表项
    QList<MContactItemData *> memberList = UICommonDataModel::GetInstance()->GetCommonwordUser(); // GetColleagueMemberList();
    Q_FOREACH (MContactItemData *obj, memberList)
    {
        if (obj)
        {
            QPushButton *button = new QPushButton(m_Sarea);
            button->setStyleSheet(sy);
            button->setFixedSize(180, 34);
            button->setText(GetStringByFontMetrics(obj->Name()));
            button->setWindowTitle(obj->Userid());
            button->setFocusPolicy(Qt::NoFocus);
            connect(button, &QPushButton::clicked, this, &UI_Widget_CommonWord_Panel::R_CustomSelectChanged);
            m_list.append(button);

            MContactItemData *contactData = new MContactItemData();
            contactData->InitData(obj->GetUUid(), obj->Vid(), obj->Userid(), obj->Chatid(), obj->Name(), obj->Groupid(), obj->DepartmentId(), obj->NoteType());
            m_mapCustomer.insert(obj->Userid(), contactData);
        }
    }
    UICommonDataModel::GetInstance()->ClearColleagueMemberList(memberList);

    m_Sarea->updataPushButton(m_list);
}

void UI_Widget_CommonWord_Panel::R_CustomSelectChanged()
{
    MContactItemData *contact = new MContactItemData();

    QString      id;
    QPushButton *pb = (QPushButton *)sender();
    if (pb)
    {
        id = pb->windowTitle();
        if (m_mapCustomer.contains(id)) //同事常用语
        {
            contact = m_mapCustomer[id];
        }
        else
        {
            contact->Userid(id);
            contact->Name(pb->text());
        }
    }
    else //默认显示个人
    {
        id = "personal";
        contact->Userid(id);
        contact->Name(tr("个人"));
    }
    RefreshUIbyMContactItemData(contact);
}

void UI_Widget_CommonWord_Panel::RefreshUIbyMContactItemData(MContactItemData *contact)
{
    if (contact->Name() == tr("个人"))
    {
        ui.personalCommonword->setChecked(true);
        ui.companyCommonword->setChecked(false);
    }
    else
    {
        m_contact = contact;
        ui.companyCommonword->setText(contact->Name());
        ui.personalCommonword->setChecked(false);
        ui.companyCommonword->setChecked(true);
    }

    UI_Model_CommonWord::GetInstance()->SetCustomerData(contact);
    UICommonWordInfo *customerInfo;
    QString           id; //查询常用语id 默认空,代表查询公司
    if (contact->Name() == tr("公司"))
    {
        //        customerInfo = gUICommonWordManager->CompanyCommonWordInfo();
        customerInfo = (UICommonWordInfo *)gUICommonWordManager->GetCommonWordInfoById(contact->Userid(), "UICommonWordInfo", UI_Model_CommonWord::GetInstance());
    }
    else
    {
        customerInfo = (UICommonWordInfo *)gUICommonWordManager->GetCommonWordInfoById(contact->Userid(), "UICommonWordInfo", UI_Model_CommonWord::GetInstance());
        id           = contact->Userid();
    }

    if (customerInfo)
    {
        UI_Model_CommonWord::GetInstance()->R_SetCommonWordinfo(contact->Userid());
    }
    else
    {
        ui.stackedWidget->setCurrentIndex(1);
        ui.page_treeView->setStyleSheet("QWidget#page_treeView{background-color: rgba(255, 255, 255, 0);image: url(:/YL/Resources/YL/MainTree/icon_searchResultNull.png);}");
        IUIController::GetInstance()->GetCustomCommonWord(id);
    }

    //关闭批量编辑模式
    ui.treeView->CloseEditModel();
    //选择后,隐藏下拉列表
    m_Sarea->setVisible(false);
}

void UI_Widget_CommonWord_Panel::R_RefreshommonWordShowMode()
{
    MContactItemData *contact = UI_Model_CommonWord::GetInstance()->GetCustomerData();
    if (contact)
    {
        UICommonWordInfo *customerInfo;
        if (contact->Name() == tr("公司"))
        {
            //            customerInfo = gUICommonWordManager->CompanyCommonWordInfo();
            customerInfo = (UICommonWordInfo *)gUICommonWordManager->GetCommonWordInfoById(contact->Userid(), "UICommonWordInfo", UI_Model_CommonWord::GetInstance());
        }
        else
        {
            customerInfo = (UICommonWordInfo *)gUICommonWordManager->GetCommonWordInfoById(contact->Userid(), "UICommonWordInfo", UI_Model_CommonWord::GetInstance());
        }
        if (customerInfo)
        {
            ui.page_treeView->setStyleSheet("QWidget#page_treeView{background-color: rgba(255, 255, 255, 0);}");
            if (customerInfo->ChildList().count() > 0)
            {
                ui.stackedWidget->setCurrentIndex(1);
            }
            else if (contact->Name() == tr("个人") || contact->Name() == tr("公司")) //其他客服的常用语无添加导入权限
            {
                ui.stackedWidget->setCurrentIndex(0);
            }
            else
            {
                ui.stackedWidget->setCurrentIndex(1);
                ui.page_treeView->setStyleSheet("QWidget#page_treeView{background-color: rgba(255, 255, 255, 0);image: url(:/YL/Resources/YL/MainTree/icon_searchResultNull.png);}");
            }
        }
        else
        {
            ui.stackedWidget->setCurrentIndex(1);
            ui.page_treeView->setStyleSheet("QWidget#page_treeView{background-color: rgba(255, 255, 255, 0);image: url(:/YL/Resources/YL/MainTree/icon_searchResultNull.png);}");
        }
    }
}

void UI_Widget_CommonWord_Panel::on_companyCommonword_clicked() //选择公司与其他人常用语
{
    QPoint poit = ui.companyCommonword->mapFromGlobal(QCursor::pos());
    QRect  rect(ui.companyCommonword->width() - 14, (ui.companyCommonword->height() - 14) / 2, 14, 14);
    if (rect.contains(poit))
    {
        m_Sarea->move(ui.companyCommonword->x(), ui.widget_search_rect->height() + ui.widget_label->height());
        m_Sarea->setVisible(!m_Sarea->isVisible());
        if (m_Sarea->isVisible())
        {
            ui.companyCommonword->setIcon(QIcon(":/Resources/SVG/commonWord/commonWord_arrow_close.svg"));
        }
        else
        {
            ui.companyCommonword->setIcon(QIcon(":/Resources/SVG/commonWord/commonWord_arrow_open.svg"));
        }
    }
    else
    {
        RefreshUIbyMContactItemData(m_contact);
    }
}

void UI_Widget_CommonWord_Panel::R_ScrollArea_hide()
{
    //失去焦点隐藏 m_Sarea 下拉框时,恢复原本选择状态
    if (ui.personalCommonword->isChecked())
    {
        ui.companyCommonword->setChecked(false);
    }
    else
    {
        ui.companyCommonword->setChecked(true);
    }
    ui.companyCommonword->setIcon(QIcon(":/Resources/SVG/commonWord/commonWord_arrow_open.svg"));
}

void UI_Widget_CommonWord_Panel::on_tbtn_cancel_clicked() //取消批量编辑
{
    ui.treeView->BatchEdit();
}

void UI_Widget_CommonWord_Panel::on_addCommonWord_clicked() //新增常用语
{
    ui.treeView->showPopupUI(Enum_YL_CtrlName_AddCommonWord, true);
}

void UI_Widget_CommonWord_Panel::on_batchImport_clicked() //批量导入
{
    ui.treeView->showPopupUI(Enum_YL_CtrlName_BatchImport, true);
}

void UI_Widget_CommonWord_Panel::on_findtoolButton_clicked()
{
    R_OnCommonWordTreeSearchLineTextChanged(ui.edit_search->text());
}


void UI_Widget_CommonWord_Panel::on_tbtn_showt_clicked() //常用语展示方式配置
{
    //    CommInfo otherInfo(*gUIPersonalConfigManager->GetUserOtherSetting());
    //    QString  data = otherInfo.GetValueByKey("commonwordshowType");

    QPoint pt = ui.tbtn_showt->mapToGlobal(ui.tbtn_showt->rect().bottomLeft());
    m_showtypeMenu->exec(pt);
}

void UI_Widget_CommonWord_Panel::R_OnActionClicked(QAction *act)
{
    QString  data = act->data().toString();
    CommInfo otherInfo(*gUIPersonalConfigManager->GetUserOtherSetting());
    otherInfo.SetValueByKey("commonwordshowType", data);
    gUIPersonalConfigManager->SetUserOtherSetting(otherInfo);
    ui.treeView->UpdateView((Common_Expressions)data.toInt()); //刷新页面
}
