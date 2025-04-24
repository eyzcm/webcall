#include "UI_QuickCommonWord_ListView.h"
#include "utils/DefineHelper.h"
#include "control/CommonSignalService.h"
#include "service/DbUiWebcallService.h"

UI_QuickCommonWord_ListView::UI_QuickCommonWord_ListView(QWidget* parent /*= NULL*/) : QListView(parent)
{
    Init();
}

UI_QuickCommonWord_ListView::~UI_QuickCommonWord_ListView()
{
}

void UI_QuickCommonWord_ListView::Update(QString type, QObject* obj)
{
    if (StringCompare(type, "DisplayQuickCommonWord"))
    {
        MCommonWordData* quickCommonWordData = (MCommonWordData*)obj;
        if (quickCommonWordData)
        {
            m_pListViewItemModel->AddQuickCommonWord(quickCommonWordData);
        }
    }
    else if (StringCompare(type, "ClearQuickCommonWord"))
    {
        m_pListViewItemModel->ClearAll();
    }
    else
    {
        Q_ASSERT(true);
    }
}

void UI_QuickCommonWord_ListView::RemoveUpdate(QString type, QString body)
{
    Q_ASSERT(false);
}

void UI_QuickCommonWord_ListView::Init()
{
    m_pListViewItemModel = new UI_QuickCommonWord_ListViewItemModel(QStringList() << "id", this);
    this->setModel(m_pListViewItemModel);

    m_pListViewItemDelegate = new UI_QuickCommonWord_ListViewItemDelegate(this);
    this->setItemDelegate(m_pListViewItemDelegate);

    this->setSelectionBehavior(QListView::SelectRows);  //一次选中整行
    this->setSelectionMode(QListView::SingleSelection); //单选，配合上面的整行就是一次选单行
    this->setFocusPolicy(Qt::NoFocus);                  //去掉鼠标移到单元格上时的虚线框
    this->setMouseTracking(true);

    connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(R_ListItemClicked(const QModelIndex&)));
    connect(this, SIGNAL(S_ListItemClicked(MCommonWordData*)), CommonSignalService::GetInstance(), SIGNAL(S_OnCommonWordTreeItemClick(MCommonWordData*)));
    connect(this, SIGNAL(S_ListItemDoubleClicked(MCommonWordData*)), CommonSignalService::GetInstance(), SIGNAL(S_OnCommonWordTreeItemDoubleClick(MCommonWordData*)));

    m_pDoubleClicked = false;
    m_pClickedItem   = nullptr;

    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(R_TimeOut()));
}

void UI_QuickCommonWord_ListView::R_ListItemClicked(const QModelIndex& index)
{
    m_pClickedItem = nullptr;
    if (!m_pDoubleClicked)
    {
        m_pDoubleClicked = true;
        m_pTimer->start(300);
        m_pClickedItem = (YL_TreeViewItem_CommonWord*)index.internalPointer();
    }
    else
    {
        m_pTimer->stop();
        m_pDoubleClicked = false;

        //从底层获取当前是否勾选常用语双击选项
        UIOpertorSettingInfo* info = gUIPersonalConfigYLService->OpertorSettingInfo();
        if (info->DoubleClickCommonWord() == 1)
        {
            YL_TreeViewItem_CommonWord* itemObj = (YL_TreeViewItem_CommonWord*)index.internalPointer();
            if (itemObj)
            {
                YL_TreeViewItemLeafNodeData_CommonWord* itemData = (YL_TreeViewItemLeafNodeData_CommonWord*)itemObj->Ptr();
                if (itemData)
                {
                    MCommonWordData* data = GetMCommonWordDataByItemLeafNodeData(itemData);
                    Q_EMIT S_ListItemDoubleClicked(data);
                    gUICommonWordManager->AddCommonWordCount(itemData->Id());
                }
            }
        }
    }
}

void UI_QuickCommonWord_ListView::R_TimeOut()
{
    m_pDoubleClicked = false;
    m_pTimer->stop();
    if (m_pClickedItem)
    {
        YL_TreeViewItemLeafNodeData_CommonWord* itemData = (YL_TreeViewItemLeafNodeData_CommonWord*)m_pClickedItem->Ptr();
        if (itemData)
        {
            MCommonWordData* data = GetMCommonWordDataByItemLeafNodeData(itemData);
            Q_EMIT S_ListItemClicked(data);
            gUICommonWordManager->AddCommonWordCount(itemData->Id());
        }
    }
}

UICommonWordInfo* UI_QuickCommonWord_ListView::Get_UICommonWordInfo_By_ItemLeafNodeData(YL_TreeViewItemLeafNodeData_CommonWord* itemData)
{
    UICommonWordInfo* info = new UICommonWordInfo;
    if (itemData)
    {
        info->Id(itemData->Id());
        info->Pid(itemData->Pid());
        info->Title(itemData->Title());
        info->Content(itemData->Content());
        info->HotKey(itemData->HotKey());
        info->IsDir(itemData->Dir());
    }
    return info;
}

MCommonWordData* UI_QuickCommonWord_ListView::GetMCommonWordDataByItemLeafNodeData(YL_TreeViewItemLeafNodeData_CommonWord* itemData)
{
    MCommonWordData* data = new MCommonWordData;
    if (itemData)
    {
        data->Id(itemData->Id());
        data->Title(itemData->Title());
        data->Pid(itemData->Pid());
        data->Content(itemData->Content());
        data->HotKey(itemData->HotKey());
        data->Dir(itemData->Dir());
    }
    return data;
}
