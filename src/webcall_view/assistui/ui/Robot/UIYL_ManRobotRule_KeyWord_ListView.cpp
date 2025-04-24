#include "UIYL_ManRobotRule_KeyWord_ListView.h"

#include "data/MUIManRobotRuleData.h"
#include "control/CommonSignalService.h"
#include "service/DbUiWebcallService.h"
#include "protocol_biz/webcallprotocolservice.h"

UIYL_ManRobotRule_KeyWord_ListView::UIYL_ManRobotRule_KeyWord_ListView(QWidget* parent) : QListView(parent), m_nListViewType(1)
{
    this->setFocusPolicy(Qt::NoFocus);
    this->setMouseTracking(true);

    Init();
}

UIYL_ManRobotRule_KeyWord_ListView::~UIYL_ManRobotRule_KeyWord_ListView()
{
}

void UIYL_ManRobotRule_KeyWord_ListView::Update(QString type, QObject* obj)
{
    if (StringCompare(type, "ClearAllUserKeyWordManRobotList"))
    {
        if (m_pModel)
        {
            m_pModel->Clear_RuleItem();
            Q_EMIT S_ListViewHasData(false);
        }
    }
    else if (StringCompare(type, "AddUIUserKeyWordManRobotRuleData"))
    {
        MUIManRobotRuleData* objData = (MUIManRobotRuleData*)obj;
        if (m_pModel)
        {
            m_pModel->Add_RuleItem(objData);
            Q_EMIT S_ListViewHasData(true);
        }
    }
    else if (StringCompare(type, "SelectUIUserKeyWordManRobotRuleData"))
    {
        MUIManRobotRuleData* objData = (MUIManRobotRuleData*)obj;
        if (m_pModel)
        {
            if (objData)
            {
                YL_ManRobotRuleListItem* curItem = m_pModel->GetListItemById(objData->Id());
                int                      row     = curItem->Row();

                QModelIndex index = m_pModel->index(row, 0);
                if (index.isValid())
                {
                    R_ListItem_Clicked(index);
                }
                else
                {
                    Q_EMIT S_RuleKeyWordListItem_Clicked("");
                }
            }
            else
            {
            }
        }
    }
    else if (StringCompare(type, "SelectFirstUIUserKeyWordManRobotRuleData"))
    {
        if (m_pModel->rowCount() > 0)
        {
            QModelIndex index = m_pModel->index(0, 0);
            if (index.isValid())
            {
                R_ListItem_Clicked(index);
            }
            else
            {
                Q_EMIT S_RuleKeyWordListItem_Clicked("");
            }
        }
        else
        {
            Q_EMIT S_RuleKeyWordListItem_Clicked("");
        }
    }
    else if (StringCompare(type, "ClearSearchAllUserManRobotList"))
    {
        if (m_pModel)
        {
            m_pModel->Clear_RuleItem();
            Q_EMIT S_SetSearchResultNull(true);
        }
    }
    else if (StringCompare(type, "AddSearchUIUserRuleData"))
    {
        MUIManRobotRuleData* objData = (MUIManRobotRuleData*)obj;
        if (m_pModel)
        {
            m_pModel->Add_RuleItem(objData);
            Q_EMIT S_SetSearchResultNull(false);
        }
    }
    else
    {
        Q_ASSERT(true);
    }
}

void UIYL_ManRobotRule_KeyWord_ListView::RemoveUpdate(QString type, QString body)
{
    if (StringCompare(type, "DeleteUserKeyWordManRobotRule"))
    {
        QString id = body;
        if (m_pModel)
        {
            m_pModel->Delete_RuleItem(id);
        }
    }
}

void UIYL_ManRobotRule_KeyWord_ListView::Init()
{
    connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(R_ListItem_Clicked(const QModelIndex&)));
    connect(this, SIGNAL(S_RuleKeyWordListItem_Clicked(QString)), CommonSignalService::GetInstance(), SIGNAL(S_ManRule_KeyWordItem_Clicked(QString)));
    connect(this, SIGNAL(S_SearchListItem_Clicked(QString)), CommonSignalService::GetInstance(), SIGNAL(S_ManRule_KeyWordSearchItem_Clicked(QString)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_ManRule_KeyWordSearchItem_Clicked(QString)), this, SLOT(R_SearchListItem_Clicked(QString)));
}

void UIYL_ManRobotRule_KeyWord_ListView::InitData()
{
}

void UIYL_ManRobotRule_KeyWord_ListView::SetListViewType(int type)
{
    m_nListViewType = type;

    m_pModel = new UIYL_ManRobotRule_KeyWord_ListViewModel(QStringList() << "id", this);
    this->setModel(m_pModel);

    m_pDelegate = new UIYL_ManRobotRule_KeyWord_ListViewItemDelegate(m_nListViewType, this);
    connect(m_pDelegate, SIGNAL(S_BtnDeleteListItem_Clicked(QString, YL_ManRobotRuleListItem*)), this, SLOT(R_BtnDeleteListItem_Clicked(QString, YL_ManRobotRuleListItem*)));
    this->setItemDelegate(m_pDelegate);

    if (m_nListViewType == 1)
    {
        CanDragDrop(true);
    }
    else if (m_nListViewType == 2)
    {
        CanDragDrop(false);
    }
}

void UIYL_ManRobotRule_KeyWord_ListView::CanDragDrop(bool val)
{
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

QStringList UIYL_ManRobotRule_KeyWord_ListView::GetIdList()
{
    QStringList retList;
    if (m_pModel)
    {
        retList = m_pModel->GetIdList();
    }
    return retList;
}

void UIYL_ManRobotRule_KeyWord_ListView::R_ListItem_Clicked(const QModelIndex& index)
{
    YL_ManRobotRuleListItem* ruleListItem = static_cast<YL_ManRobotRuleListItem*>(index.internalPointer());
    if (ruleListItem)
    {
        if (m_nListViewType == 1)
        {
            SetCurrentItemData(ruleListItem->Id());
            Q_EMIT S_RuleKeyWordListItem_Clicked(ruleListItem->Id());
            this->setCurrentIndex(index);
        }
        else if (m_nListViewType == 2)
        {
            Q_EMIT S_SearchListItem_Clicked(ruleListItem->Id());
        }
    }
}

void UIYL_ManRobotRule_KeyWord_ListView::R_SearchListItem_Clicked(QString id)
{
    if (m_pModel && (m_nListViewType == 1))
    {
        YL_ManRobotRuleListItem* item = m_pModel->GetListItemById(id);
        if (item)
        {
            SetCurrentItemData(id);
            YL_ManRobotRuleListItem* pItem = item->ParentItem();
            if (pItem)
            {
                int         leafRow = item->Row();
                QModelIndex index   = m_pModel->index(leafRow, 0);
                this->setCurrentIndex(index);
            }
        }
    }
}

void UIYL_ManRobotRule_KeyWord_ListView::R_BtnDeleteListItem_Clicked(QString type, YL_ManRobotRuleListItem* item)
{
    if (type == "Close")
    {
        if (item)
        {
            gIProtocolAgent->DoDeleteUserAiRule("keyword", item->Id());
        }
    }
}

void UIYL_ManRobotRule_KeyWord_ListView::SetCurrentItemData(QString id)
{
    m_id = id;
}

void UIYL_ManRobotRule_KeyWord_ListView::leaveEvent(QEvent* event)
{
    this->mouseMoveEvent((QMouseEvent*)event);
}

void UIYL_ManRobotRule_KeyWord_ListView::mousePressEvent(QMouseEvent* event)
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

void UIYL_ManRobotRule_KeyWord_ListView::mouseMoveEvent(QMouseEvent* event)
{
    if ((event->buttons() & Qt::LeftButton) && m_IsLeftMouse)
    {
        QModelIndex theDragIndex = indexAt(event->pos());
        if (theDragIndex.isValid())
        {
            YL_ManRobotRuleListItem* theDragItem = (YL_ManRobotRuleListItem*)m_pModel->itemFromIndex(theDragIndex);
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
    else
    {
        QAbstractItemView::mouseMoveEvent(event);
    }
}

void UIYL_ManRobotRule_KeyWord_ListView::mouseReleaseEvent(QMouseEvent* event)
{
    m_IsRightMouse = false;
    m_IsLeftMouse  = false;

    if (event->button() == Qt::RightButton)
    {
        m_IsRightMouse = true;
    }
    QAbstractItemView::mouseReleaseEvent(event);
}

void UIYL_ManRobotRule_KeyWord_ListView::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("dragId"))
        event->accept();
    else
        event->ignore();
}

void UIYL_ManRobotRule_KeyWord_ListView::dragMoveEvent(QDragMoveEvent* event)
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

void UIYL_ManRobotRule_KeyWord_ListView::dropEvent(QDropEvent* event)
{
    //获取当前选中项的 index、item
    QString dragId = "";
    if (event->mimeData()->hasFormat("dragId"))
    {
        dragId = event->mimeData()->data("dragId");
    }

    YL_ManRobotRuleListItem* curItem = m_pModel->GetListItemById(dragId);

    //获取当前鼠标悬浮项的 index、item
    QModelIndex              hoverIndex = this->indexAt(event->pos());
    YL_ManRobotRuleListItem* hoverItem  = m_pModel->itemFromIndex(hoverIndex);

    m_pModel->Move_RuleItem(curItem, hoverItem);

    if (curItem && hoverItem)
    {
        if (curItem->Id() == hoverItem->Id())
        {
            // do nothing/
        }
        else
        {
            int nIndex = hoverIndex.row() + 1;

            QModelIndex selIndex = m_pModel->index(nIndex, 0);
            if (selIndex.isValid())
            {
                this->setCurrentIndex(selIndex);
                R_ListItem_Clicked(selIndex);
            }
            else
            {
                QModelIndex tempIndex = m_pModel->index(nIndex - 1, 0);
                this->setCurrentIndex(tempIndex);
                R_ListItem_Clicked(tempIndex);
            }
        }
    }

    Q_EMIT S_DragItemChanged();
    m_IsDrag = false;

    event->setDropAction(Qt::MoveAction);
    event->accept();
}
