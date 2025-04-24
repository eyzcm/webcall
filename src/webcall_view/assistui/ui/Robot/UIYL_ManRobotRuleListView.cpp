#include "UIYL_ManRobotRuleListView.h"
#include "utils/DefineHelper.h"
#include "data/MUIManRobotRuleData.h"
#include "control/CommonSignalService.h"
#include "service/DbUiWebcallService.h"
#include "protocol_biz/webcallprotocolservice.h"

UIYL_ManRobotRuleListView::UIYL_ManRobotRuleListView(QWidget *parent /*= NULL*/) : QListView(parent)
{
    m_UIYL_ManRobotRuleListViewModel = new UIYL_ManRobotRuleListViewModel(QStringList() << "id", this);
    this->setModel(m_UIYL_ManRobotRuleListViewModel);

    m_UIYL_ManRobotRuleListViewItemDelegate = new UIYL_ManRobotRuleListViewItemDelegate(this);
    this->setItemDelegate(m_UIYL_ManRobotRuleListViewItemDelegate);

    connect(m_UIYL_ManRobotRuleListViewItemDelegate, SIGNAL(S_BtnClicked(QString, YL_ManRobotRuleListItem *)), this, SLOT(R_BtnClicked(QString, YL_ManRobotRuleListItem *)));

    this->setSelectionBehavior(QListView::SelectRows);  //一次选中整行
    this->setSelectionMode(QListView::SingleSelection); //单选，配合上面的整行就是一次选单行
    this->setFocusPolicy(Qt::NoFocus);                  //去掉鼠标移到单元格上时的虚线框
    this->setMouseTracking(true);
    CanDragDrop(true);
    Init();
}

UIYL_ManRobotRuleListView::~UIYL_ManRobotRuleListView()
{
}

void UIYL_ManRobotRuleListView::Update(QString type, QObject *obj)
{
    if (StringCompare(type, "ClearAllPersonalManRobotList"))
    {
        if (m_UIYL_ManRobotRuleListViewModel)
        {
            m_UIYL_ManRobotRuleListViewModel->ClearAll();
        }
    }
    else if (StringCompare(type, "AddUIManRobotRuleData"))
    {
        MUIManRobotRuleData *objData = (MUIManRobotRuleData *)obj;
        if (m_UIYL_ManRobotRuleListViewModel)
        {
            m_UIYL_ManRobotRuleListViewModel->AddManRobotRuleData(objData);
        }
    }

    else if (StringCompare(type, "SelectUIManRobotRuleData"))
    {
        MUIManRobotRuleData *objData = (MUIManRobotRuleData *)obj;
        if (m_UIYL_ManRobotRuleListViewModel)
        {
            if (objData)
            {
                YL_ManRobotRuleListItem *curItem = m_UIYL_ManRobotRuleListViewModel->GetMainTreeItemById(objData->Id());
                int                      row     = curItem->Row();
                QModelIndex              index   = m_UIYL_ManRobotRuleListViewModel->index(row, 0);
                if (index.isValid())
                {
                    R_OnTreeItemClicked(index);
                }
                else
                {
                    Q_EMIT S_OnClickManRobotRule("");
                }
            }
            else
            {
            }
        }
    }
    else if (StringCompare(type, "SelectFirstUIManRobotRuleData"))
    {
        if (m_UIYL_ManRobotRuleListViewModel->rowCount() > 0)
        {
            QModelIndex index = m_UIYL_ManRobotRuleListViewModel->index(0, 0);
            if (index.isValid())
            {
                R_OnTreeItemClicked(index);
            }
            else
            {
                Q_EMIT S_OnClickManRobotRule("");
            }
        }
        else
        {
            Q_EMIT S_OnClickManRobotRule("");
        }
    }
    else
    {
        Q_ASSERT(true);
    }
}

void UIYL_ManRobotRuleListView::RemoveUpdate(QString type, QString body)
{
    if (StringCompare(type, "DeletePersonalManRobotRule"))
    {
        QString id = body;
        if (m_UIYL_ManRobotRuleListViewModel)
        {
            m_UIYL_ManRobotRuleListViewModel->DeleteManRobotRuleItem(id);
        }
    }
}

void UIYL_ManRobotRuleListView::Init()
{
    connect(this, SIGNAL(clicked(const QModelIndex &)), this, SLOT(R_OnTreeItemClicked(const QModelIndex &)));
    connect(this, SIGNAL(S_OnClickManRobotRule(QString)), CommonSignalService::GetInstance(), SIGNAL(S_OnClickManRobotRule(QString)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_OnClickManRobotRuleSearchItem(QString)), this, SLOT(R_OnClickManRobotRuleSearchItem(QString)));
}

void UIYL_ManRobotRuleListView::R_OnTreeItemClicked(const QModelIndex &index)
{
    YL_ManRobotRuleListItem *yl_manRobotRuleObj = static_cast<YL_ManRobotRuleListItem *>(index.internalPointer());
    if (yl_manRobotRuleObj)
    {
        SetCurrentItemData(yl_manRobotRuleObj->Id());
        Q_EMIT S_OnClickManRobotRule(yl_manRobotRuleObj->Id());
    }
    this->setCurrentIndex(index);
}

void UIYL_ManRobotRuleListView::InitData()
{
}

void UIYL_ManRobotRuleListView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("dragId"))
        event->accept();
    else
        event->ignore();
}

void UIYL_ManRobotRuleListView::dragMoveEvent(QDragMoveEvent *event)
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

void UIYL_ManRobotRuleListView::dropEvent(QDropEvent *event)
{
    //获取当前选中项的 index、item
    QString dragId = "";
    if (event->mimeData()->hasFormat("dragId"))
    {
        dragId = event->mimeData()->data("dragId");
    }

    YL_ManRobotRuleListItem *curItem = m_UIYL_ManRobotRuleListViewModel->GetMainTreeItemById(dragId);

    //获取当前鼠标悬浮项的 index、item
    QModelIndex              hoverIndex = this->indexAt(event->pos());
    YL_ManRobotRuleListItem *hoverItem  = m_UIYL_ManRobotRuleListViewModel->itemFromIndex(hoverIndex);

    m_UIYL_ManRobotRuleListViewModel->MoveItem(curItem, hoverItem);

    if (curItem && hoverItem)
    {
        if (curItem->Id() == hoverItem->Id())
        {
            // do nothing/
        }
        else
        {
            int nIndex = hoverIndex.row() + 1;

            QModelIndex selIndex = m_UIYL_ManRobotRuleListViewModel->index(nIndex, 0);
            if (selIndex.isValid())
            {
                this->setCurrentIndex(selIndex);
                R_OnTreeItemClicked(selIndex);
            }
            else
            {
                QModelIndex tempIndex = m_UIYL_ManRobotRuleListViewModel->index(nIndex - 1, 0);
                this->setCurrentIndex(tempIndex);
                R_OnTreeItemClicked(tempIndex);
            }
        }
    }

    Q_EMIT S_DragItemChanged();

    m_IsDrag = false;

    event->setDropAction(Qt::MoveAction);
    event->accept();
}

void UIYL_ManRobotRuleListView::CanDragDrop(bool val)
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

void UIYL_ManRobotRuleListView::mousePressEvent(QMouseEvent *event)
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

void UIYL_ManRobotRuleListView::mouseReleaseEvent(QMouseEvent *event)
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
            /*YL_ManRobotRuleListItem* itemObj = (YL_ManRobotRuleListItem*)m_UIYL_ManRobotRuleListViewModel->itemFromIndex(index);
                        if (itemObj)
                        {
            }*/
        }
    }
    else
    {
    }

    QAbstractItemView::mouseReleaseEvent(event);
}

void UIYL_ManRobotRuleListView::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && m_IsLeftMouse)
    {
        QModelIndex theDragIndex = indexAt(event->pos());
        if (theDragIndex.isValid())
        {
            YL_ManRobotRuleListItem *theDragItem = (YL_ManRobotRuleListItem *)m_UIYL_ManRobotRuleListViewModel->itemFromIndex(theDragIndex);
            if (theDragItem)
            {
                QMimeData *mimeData = new QMimeData;
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
                        YL_ManRobotRuleLeafNodeItemData *leafNode = (YL_ManRobotRuleLeafNodeItemData *)theDragItem->Ptr();
                        strText                                   = leafNode->Name();
                        pixmap_icon.load(":/YL/Resources/YL/CommonWord/icon_threeLevel.png");
                    }

                    QPainter painter(&pixmap_bg);
                    painter.setFont(font_text);
                    painter.setPen(QColor(55, 55, 55));

                    painter.drawPixmap(rect_icon, pixmap_icon);
                    painter.drawText((const QRectF)(rect_text), strText);

                    QDrag *drag = new QDrag(this);
                    drag->setMimeData(mimeData);
                    drag->setPixmap(pixmap_bg);
                    drag->setHotSpot(QPoint(pixmap_bg.width() / 2, pixmap_bg.height() / 2));
                    drag->exec(Qt::MoveAction);
                }
            }
        }
    }
    else
    {
        QAbstractItemView::mouseMoveEvent(event);
    }
}

void UIYL_ManRobotRuleListView::leaveEvent(QEvent *event)
{
    this->mouseMoveEvent((QMouseEvent *)event);
}

void UIYL_ManRobotRuleListView::R_OnClickManRobotRuleSearchItem(QString id)
{
    if (m_UIYL_ManRobotRuleListViewModel)
    {
        SetCurrentItemData(id);
        if (m_UIYL_ManRobotRuleListViewModel->FindMainTreeItemById(id))
        {
            YL_ManRobotRuleListItem *item = m_UIYL_ManRobotRuleListViewModel->GetMainTreeItemById(id);
            if (item)
            {
                if (item->Leaf())
                {
                    YL_ManRobotRuleListItem *pItem = item->ParentItem();

                    if (pItem)
                    {
                        int         leafRow = item->Row();
                        QModelIndex index   = m_UIYL_ManRobotRuleListViewModel->index(leafRow, 0);
                        this->setCurrentIndex(index);
                    }
                }
                else
                {
                    Q_ASSERT(false);
                }
            }
        }
    }
}

void UIYL_ManRobotRuleListView::SetCurrentItemData(QString id)
{
    m_id = id;
}

QStringList UIYL_ManRobotRuleListView::GetIdList()
{
    QStringList retList;
    if (m_UIYL_ManRobotRuleListViewModel)
    {
        retList = m_UIYL_ManRobotRuleListViewModel->GetIdList();
    }
    return retList;
}

void UIYL_ManRobotRuleListView::R_BtnClicked(QString type, YL_ManRobotRuleListItem *item)
{
    if (type == "Close")
    {
        if (item)
        {
            gIProtocolAgent->DoDeleteAiRule(item->Id());
        }
    }
}
