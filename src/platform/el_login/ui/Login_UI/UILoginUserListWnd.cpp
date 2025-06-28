#include "UILoginUserListWnd.h"
#include "service/DbUiUserInfoInterface.h"
#include "session/el_sessionservice.h"
#include "Widget_Login_UserList.h"
#include <QListWidgetItem>

UILoginUserListWnd::UILoginUserListWnd(QWidget* parent) : QWidget(parent), m_searchKey(""), m_listUserInfo(QList<CommInfo*>())
{
    ui.setupUi(this);
    // this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint); // 去掉标题栏,去掉任务栏显示，窗口置顶

    connect(ui.m_userWordListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(R_OnItemClicked(QListWidgetItem*)));
    ui.m_userWordListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui.m_userWordListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    UILoginUserListWnd::installEventFilter(this);
    Init();
}

UILoginUserListWnd::~UILoginUserListWnd()
{
}

void UILoginUserListWnd::MoveNextItem()
{
    int nCount  = ui.m_userWordListWidget->count();
    int nCurRow = ui.m_userWordListWidget->currentRow();
    if (nCurRow < nCount - 1)
    {
        ui.m_userWordListWidget->setCurrentRow(nCurRow + 1);
    }
}

void UILoginUserListWnd::MovePreItem()
{
    int nCurRow = ui.m_userWordListWidget->currentRow();
    if (nCurRow > 0)
    {
        ui.m_userWordListWidget->setCurrentRow(nCurRow - 1);
    }
}

void UILoginUserListWnd::EnterCurrentItem()
{
    QListWidgetItem* item = ui.m_userWordListWidget->currentItem();
    R_OnItemClicked(item);
}

bool UILoginUserListWnd::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == this)
    {
        //窗口停用，变为不活动的窗口
        if (QEvent::WindowDeactivate == event->type())
        {
            //曹振超 2019年9月17日
            Q_EMIT S_UnActive();

            hide(); //或者关闭窗口,加入自己的操作.
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

void UILoginUserListWnd::Init()
{

    m_listUserInfo = gSessionService->GetAccountList();
}

void UILoginUserListWnd::SetSearchKey(QString key)
{
    m_searchKey = key.toLower();
    AddAccountItems();
}

void UILoginUserListWnd::AddAccountItems()
{
    ui.m_userWordListWidget->clear();

    int nIndex = 0;
    Q_FOREACH (CommInfo* info, m_listUserInfo)
    {
        if (info)
        {
            QString strNickName = info->GetValueByKey("nickname").toLower();
            QString strUserName = info->GetValueByKey("username").toLower();
            if (!strUserName.isEmpty())
            {
                if ((strNickName.indexOf(m_searchKey) != -1) || (strUserName.indexOf(m_searchKey) != -1))
                {
                    AddAccountItem(info, nIndex);
                    nIndex++;
                }
            }
        }
    }
}

void UILoginUserListWnd::AddAccountItem(CommInfo* info, int nIndex)
{
    QListWidgetItem* item = new QListWidgetItem();
    item->setSizeHint(QSize(260, 54));
    ui.m_userWordListWidget->addItem(item);

    Widget_Login_UserList* pLogin_User = new Widget_Login_UserList;

    QString name = QString("%1(%2)").arg(info->GetValueByKey("username")).arg(info->GetValueByKey("nickname"));
    pLogin_User->SetContent(name, info->GetValueByKey("username"), nIndex);
    ui.m_userWordListWidget->setItemWidget(item, pLogin_User);
    connect(pLogin_User, SIGNAL(Signal_DeleteRow(QString)), this, SLOT(Slot_DeleteRow(QString)));

    //默认选中第一行
    if (nIndex == 0)
    {
        // ui.m_userWordListWidget->setItemSelected(item, true);
        ui.m_userWordListWidget->setCurrentRow(0);
    }
}

void UILoginUserListWnd::Show()
{
    AddAccountItems();
    this->show();
    this->setFocusPolicy(Qt::FocusPolicy::NoFocus);
}

void UILoginUserListWnd::Slot_DeleteRow(QString userName)
{
    //删除账号信息。
    gSessionService->DelectAccount(userName);

    ui.m_userWordListWidget->clear();
    Init();
    Show();
}

void UILoginUserListWnd::R_OnItemClicked(QListWidgetItem* item)
{
    Widget_Login_UserList* curItem = (Widget_Login_UserList*)ui.m_userWordListWidget->itemWidget(item);
    if (curItem)
    {
        QString   userName = curItem->GetUserName();
        CommInfo* account  = gSessionService->GetAccountById(userName);
        /*if (account)
        {
                InitLoginWnd(account, false);
        }*/
        Q_EMIT S_OnItemClick(account);
        this->hide();
    }
}
