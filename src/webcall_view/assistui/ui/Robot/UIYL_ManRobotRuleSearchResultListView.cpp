#include "UIYL_ManRobotRuleSearchResultListView.h"
#include "utils/DefineHelper.h"
#include "data/MUIManRobotRuleData.h"
#include "control/CommonSignalService.h"

UIYL_ManRobotRuleSearchResultListView::UIYL_ManRobotRuleSearchResultListView(QWidget* parent /*= NULL*/) : QListView(parent)
{
    m_UIYL_ManRobotRuleSearchResultListViewModel = new UIYL_ManRobotRuleSearchResultListViewModel(QStringList() << "id", this);
    this->setModel(m_UIYL_ManRobotRuleSearchResultListViewModel);

    m_UIYL_ManRobotRuleSearchResultListViewItemDelegate = new UIYL_ManRobotRuleSearchResultListViewItemDelegate(this);
    this->setItemDelegate(m_UIYL_ManRobotRuleSearchResultListViewItemDelegate);

    this->setSelectionBehavior(QListView::SelectRows);  //一次选中整行
    this->setSelectionMode(QListView::SingleSelection); //单选，配合上面的整行就是一次选单行
    this->setFocusPolicy(Qt::NoFocus);                  //去掉鼠标移到单元格上时的虚线框
    this->setMouseTracking(true);
    CanDragDrop(true);
    Init();
}

UIYL_ManRobotRuleSearchResultListView::~UIYL_ManRobotRuleSearchResultListView()
{
}

void UIYL_ManRobotRuleSearchResultListView::Update(QString type, QObject* obj)
{
    if (StringCompare(type, "ClearSearchAllPersonalManRobotList"))
    {
        if (m_UIYL_ManRobotRuleSearchResultListViewModel)
        {
            m_UIYL_ManRobotRuleSearchResultListViewModel->ClearAll();
            Q_EMIT S_SetSearchResultNull(true);
        }
    }
    else if (StringCompare(type, "AddSearchUIManRobotRuleData"))
    {
        MUIManRobotRuleData* objData = (MUIManRobotRuleData*)obj;
        if (m_UIYL_ManRobotRuleSearchResultListViewModel)
        {
            m_UIYL_ManRobotRuleSearchResultListViewModel->AddManRobotRuleData(objData);
            Q_EMIT S_SetSearchResultNull(false);
        }
    }
    else
    {
        Q_ASSERT(true);
    }
}

void UIYL_ManRobotRuleSearchResultListView::RemoveUpdate(QString type, QString body)
{
    // donothing.
}

void UIYL_ManRobotRuleSearchResultListView::Init()
{
    connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(R_OnTreeItemClicked(const QModelIndex&)));
    connect(this, SIGNAL(S_OnClickSearchItem(QString)), CommonSignalService::GetInstance(), SIGNAL(S_OnClickManRobotRuleSearchItem(QString)));
}

void UIYL_ManRobotRuleSearchResultListView::R_OnTreeItemClicked(const QModelIndex& index)
{
    YL_ManRobotRuleListItem* yl_mainTreeItemObj = static_cast<YL_ManRobotRuleListItem*>(index.internalPointer());
    if (yl_mainTreeItemObj)
    {
        Q_EMIT S_OnClickSearchItem(yl_mainTreeItemObj->Id());
    }
}

void UIYL_ManRobotRuleSearchResultListView::InitData()
{
}

void UIYL_ManRobotRuleSearchResultListView::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("dragId"))
        event->accept();
    else
        event->ignore();
}

void UIYL_ManRobotRuleSearchResultListView::dragMoveEvent(QDragMoveEvent* event)
{

    if (event->mimeData()->hasFormat("dragId"))
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void UIYL_ManRobotRuleSearchResultListView::dropEvent(QDropEvent* event)
{
    //获取当前选中项的 index、item
    QString dragId = "";
    if (event->mimeData()->hasFormat("dragId"))
    {
        dragId = event->mimeData()->data("dragId");
    }

    YL_ManRobotRuleListItem* curItem = m_UIYL_ManRobotRuleSearchResultListViewModel->GetMainTreeItemById(dragId);

    //获取当前鼠标悬浮项的 index、item
    QModelIndex              hoverIndex = this->indexAt(event->pos());
    YL_ManRobotRuleListItem* hoverItem  = m_UIYL_ManRobotRuleSearchResultListViewModel->itemFromIndex(hoverIndex);

    m_UIYL_ManRobotRuleSearchResultListViewModel->MoveItem(curItem, hoverItem);

    m_IsDrag = false;

    event->setDropAction(Qt::MoveAction);
    event->accept();
}

void UIYL_ManRobotRuleSearchResultListView::CanDragDrop(bool val)
{
    this->setMouseTracking(true);
    if (val)
    {
        this->setAcceptDrops(true);
        this->setDragEnabled(true);
        this->setDragDropMode(QAbstractItemView::DragDrop);
    }
    else
    {
        this->setAcceptDrops(false);
        this->setDragEnabled(false);
        this->setDragDropMode(QAbstractItemView::NoDragDrop);
    }
}

void UIYL_ManRobotRuleSearchResultListView::mousePressEvent(QMouseEvent* event)
{
    m_IsRightMouse = false;
    if (event->button() == Qt::RightButton)
    {
        m_IsRightMouse = true;
    }
    else
    {
        m_IsLeftMouse = true;
    }

    QAbstractItemView::mousePressEvent(event);
}

void UIYL_ManRobotRuleSearchResultListView::mouseReleaseEvent(QMouseEvent* event)
{
    m_IsRightMouse = false;
    m_IsLeftMouse  = false;
    if (event->button() == Qt::RightButton)
    {
        m_IsRightMouse = true;

        QModelIndex index = this->indexAt(event->pos());
        if (!index.isValid())
        {
            //空白处右键
        }
        else
        {
        }
    }
    else
    {
    }

    QAbstractItemView::mouseReleaseEvent(event);
}

void UIYL_ManRobotRuleSearchResultListView::mouseMoveEvent(QMouseEvent* event)
{
    if ((event->buttons() & Qt::LeftButton) && m_IsLeftMouse)
    {
        QModelIndex theDragIndex = indexAt(event->pos());
        if (theDragIndex.isValid())
        {
            YL_ManRobotRuleListItem* theDragItem = (YL_ManRobotRuleListItem*)m_UIYL_ManRobotRuleSearchResultListViewModel->itemFromIndex(theDragIndex);
            if (theDragItem)
            {
                QMimeData* mimeData = new QMimeData;
                mimeData->setData("dragId", theDragItem->Id().toLocal8Bit());

                // 180*26
                QPixmap pixmap_bg(":/YL/Resources/YL/CommonWord/bg_drag.png");
                if (!pixmap_bg.isNull())
                {
                    QRect rect_icon(10, 7, 14, 14);
                    QRect rect_text(rect_icon.right() + 10, rect_icon.top() - 2, pixmap_bg.width() - (rect_icon.right() + 10), pixmap_bg.height());
                    QFont font_text(tr("微软雅黑"), 9);

                    QString strText = "";
                    QPixmap pixmap_icon;
                    if (theDragItem->Leaf())
                    {
                        YL_ManRobotRuleLeafNodeItemData* leafNode = (YL_ManRobotRuleLeafNodeItemData*)theDragItem->Ptr();
                        strText                                   = leafNode->Name();
                        pixmap_icon.load(":/YL/Resources/YL/CommonWord/icon_threeLevel.png");
                    }

                    QPainter painter(&pixmap_bg);
                    painter.setFont(font_text);
                    painter.setPen(QColor(55, 55, 55));

                    painter.drawPixmap(rect_icon, pixmap_icon);
                    painter.drawText((const QRectF)(rect_text), strText);

                    QDrag* drag = new QDrag(this);
                    drag->setMimeData(mimeData);
                    drag->setPixmap(pixmap_bg);
                    drag->setHotSpot(QPoint(pixmap_bg.width() / 2, pixmap_bg.height() / 2));
                    drag->exec(Qt::MoveAction);
                }
            }
        }
    }
    QAbstractItemView::mouseMoveEvent(event);
}
