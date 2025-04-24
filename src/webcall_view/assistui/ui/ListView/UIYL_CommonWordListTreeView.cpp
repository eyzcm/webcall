#include "UIYL_CommonWordListTreeView.h"
#include "utils/DefineHelper.h"
#include "control/CommonSignalService.h"
#include "service/DbUiWebcallService.h"

UIYL_CommonWordListTreeView::UIYL_CommonWordListTreeView(QWidget* parent /*= NULL*/) : QListView(parent)
{
    m_UIYL_CommonWordListTreeViewModel = new UIYL_CommonWordListViewModel(QStringList() << "id", this);
    this->setModel(m_UIYL_CommonWordListTreeViewModel);

    m_UIYL_CommonWordListTreeViewItemDelegate = new UIYL_CommonWordListViewItemDelegate(this);
    this->setItemDelegate(m_UIYL_CommonWordListTreeViewItemDelegate);

    this->setSelectionBehavior(QListView::SelectRows);  //一次选中整行
    this->setSelectionMode(QListView::SingleSelection); //单选，配合上面的整行就是一次选单行
    this->setFocusPolicy(Qt::NoFocus);                  //去掉鼠标移到单元格上时的虚线框
    this->setMouseTracking(true);

    Init();
}

UIYL_CommonWordListTreeView::~UIYL_CommonWordListTreeView()
{
}

void UIYL_CommonWordListTreeView::Update(QString type, QObject* obj)
{
    if (StringCompare(type, "CLEARALL"))
    {
        if (m_UIYL_CommonWordListTreeViewModel)
        {
            m_UIYL_CommonWordListTreeViewModel->ClearAll();
            Q_EMIT S_SetSearchResultNull(true);
        }
    }
    else if (StringCompare(type, "ADDCOMMONWORD"))
    {
        MCommonWordData* objData = (MCommonWordData*)obj;
        if (m_UIYL_CommonWordListTreeViewModel)
        {
            m_UIYL_CommonWordListTreeViewModel->AddCommonWord(objData);
            Q_EMIT S_SetSearchResultNull(false);
        }
    }
    else
    {
        Q_ASSERT(true);
    }
}

void UIYL_CommonWordListTreeView::RemoveUpdate(QString type, QString body)
{
    Q_ASSERT(false);
}

void UIYL_CommonWordListTreeView::Init()
{
    connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(R_OnTreeItemClicked(const QModelIndex&)));
    connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(R_OnTreeItemdoubleClicked(const QModelIndex&)));
    connect(this, SIGNAL(S_OnClickSearchCommonWordItem(QString)), CommonSignalService::GetInstance(), SIGNAL(S_OnClickSearchCommonWordItem(QString)));
    connect(this, SIGNAL(S_OnDoubleClickSearchCommonWordItem(QString)), CommonSignalService::GetInstance(), SIGNAL(S_OnDoubleClickSearchCommonWordItem(QString)));
}

void UIYL_CommonWordListTreeView::R_OnTreeItemClicked(const QModelIndex& index)
{
    YL_TreeViewItem_CommonWord* tempObj = static_cast<YL_TreeViewItem_CommonWord*>(index.internalPointer());
    if (tempObj)
    {
        Q_EMIT S_OnClickSearchCommonWordItem(tempObj->ID());
    }
}

void UIYL_CommonWordListTreeView::R_OnTreeItemdoubleClicked(const QModelIndex& index)
{
    //从底层获取当前是否勾选常用语双击选项
    UIOpertorSettingInfo* info = gUIPersonalConfigYLService->OpertorSettingInfo();
    if (info->DoubleClickCommonWord() == 1)
    {
        YL_TreeViewItem_CommonWord* tempObj = static_cast<YL_TreeViewItem_CommonWord*>(index.internalPointer());
        if (tempObj)
        {
            Q_EMIT S_OnDoubleClickSearchCommonWordItem(tempObj->ID());
        }
    }
}
