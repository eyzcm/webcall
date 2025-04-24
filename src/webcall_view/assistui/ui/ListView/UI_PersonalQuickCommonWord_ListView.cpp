#include "UI_PersonalQuickCommonWord_ListView.h"
#include "utils/DefineHelper.h"
#include "control/CommonSignalService.h"
#include "service/DbUiWebcallService.h"
#include "protocol_biz/webcallprotocolservice.h"

UI_PersonalQuickCommonWord_ListView::UI_PersonalQuickCommonWord_ListView(QWidget* parent /*= NULL*/) : QListView(parent)
{
    Init();
}

UI_PersonalQuickCommonWord_ListView::~UI_PersonalQuickCommonWord_ListView()
{
}

void UI_PersonalQuickCommonWord_ListView::Update(QString type, QObject* obj)
{
    if (StringCompare(type, "DisplayPersonalQuickCommonWord"))
    {
        MPersonalQuickCommonWordData* objData = (MPersonalQuickCommonWordData*)obj;
        if (objData)
        {
            m_pListViewItemModel->AddPersonalQuickCommonWord(objData);
        }
    }
    else if (StringCompare(type, "ClearPersonalQuickCommonWord"))
    {
        m_pListViewItemModel->ClearAll();
    }
    else
    {
        Q_ASSERT(true);
    }
}

void UI_PersonalQuickCommonWord_ListView::RemoveUpdate(QString type, QString body)
{
    if (StringCompare(type, "DelQuickReplyCommonWord"))
    {
        m_pListViewItemModel->RemoveItemById(body);
    }
    else
    {
        Q_ASSERT(false);
    }
}

void UI_PersonalQuickCommonWord_ListView::Init()
{
    m_pListViewItemModel = new UI_PersonalQuickCommonWord_ListViewItemModel(QStringList() << "id", this);
    this->setModel(m_pListViewItemModel);

    m_pListViewItemDelegate = new UI_PersonalQuickCommonWord_ListViewItemDelegate(this);
    this->setItemDelegate(m_pListViewItemDelegate);

    this->setSelectionBehavior(QListView::SelectRows);  //一次选中整行
    this->setSelectionMode(QListView::SingleSelection); //单选，配合上面的整行就是一次选单行
    this->setFocusPolicy(Qt::NoFocus);                  //去掉鼠标移到单元格上时的虚线框
    this->setMouseTracking(true);

    connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(R_ListItemClicked(const QModelIndex&)));
    connect(m_pListViewItemDelegate, SIGNAL(S_BtnClicked(QString, YL_PersonalQuickTreeViewItem_CommonWord*)), this, SLOT(R_BtnClicked(QString, YL_PersonalQuickTreeViewItem_CommonWord*)));

    connect(this, SIGNAL(S_ListItemDoubleClicked(MPersonalQuickCommonWordData*)), CommonSignalService::GetInstance(), SIGNAL(S_OnDoubleClick_PersonalQuickCommonWord(MPersonalQuickCommonWordData*)));
    connect(this, SIGNAL(S_ModifyPersonalQuickCommonWord(MPersonalQuickCommonWordData*)), CommonSignalService::GetInstance(), SIGNAL(S_ModifyPersonalQuickCommonWord(MPersonalQuickCommonWordData*)));
    connect(this, SIGNAL(S_RemovePersonalQuickCommonWord(QString)), CommonSignalService::GetInstance(), SIGNAL(S_RemovePersonalQuickCommonWord(QString)));

    m_pDoubleClicked = false;
    m_pClickedItem   = nullptr;

    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(R_TimeOut()));
}

void UI_PersonalQuickCommonWord_ListView::R_ListItemClicked(const QModelIndex& index)
{
    m_pClickedItem = nullptr;
    if (!m_pDoubleClicked)
    {
        m_pDoubleClicked = true;
        m_pTimer->start(300);
        m_pClickedItem = (YL_PersonalQuickTreeViewItem_CommonWord*)index.internalPointer();
    }
    else
    {
        m_pTimer->stop();
        m_pDoubleClicked = false;

        //从底层获取当前是否勾选常用语双击选项

        YL_PersonalQuickTreeViewItem_CommonWord* itemObj = (YL_PersonalQuickTreeViewItem_CommonWord*)index.internalPointer();
        if (itemObj)
        {
            YL_PersonalQuickTreeViewItemLeafNodeData_CommonWord* itemData = (YL_PersonalQuickTreeViewItemLeafNodeData_CommonWord*)itemObj->Ptr();
            if (itemData)
            {
                MPersonalQuickCommonWordData* data = new MPersonalQuickCommonWordData(*itemData);
                Q_EMIT S_ListItemDoubleClicked(data);
                gIProtocolAgent->DoUploadOperatorLog("online_client_conversation_32");
            }
        }
    }
}

void UI_PersonalQuickCommonWord_ListView::R_BtnClicked(QString type, YL_PersonalQuickTreeViewItem_CommonWord* item)
{

    // todo shizhenyu
    if (type == "Close")
    {
        if (item)
        {
            Q_EMIT S_RemovePersonalQuickCommonWord(item->Id());
        }
    }
    else if (type == "Modify")
    {
        if (item)
        {
            YL_PersonalQuickTreeViewItemLeafNodeData_CommonWord* itemData = (YL_PersonalQuickTreeViewItemLeafNodeData_CommonWord*)item->Ptr();
            if (itemData)
            {
                MPersonalQuickCommonWordData* data = new MPersonalQuickCommonWordData(*itemData);
                Q_EMIT S_ModifyPersonalQuickCommonWord(data);
            }
        }
    }
}

void UI_PersonalQuickCommonWord_ListView::R_TimeOut()
{
    m_pDoubleClicked = false;
    m_pTimer->stop();
    if (m_pClickedItem)
    {
        //单击
    }
}
