#include "UIYL_CommonWordListViewItemDelegate.h"
#include "UI_TreeViewItemDelegate_CommonWord.h"

#include <utils/UtilsHelper.h>

UIYL_CommonWordListViewItemDelegate::UIYL_CommonWordListViewItemDelegate(QObject* parent) : QStyledItemDelegate(parent), m_IsEditState(false)
{
}
UIYL_CommonWordListViewItemDelegate::~UIYL_CommonWordListViewItemDelegate()
{
}

void UIYL_CommonWordListViewItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    //获取节点指针
    YL_TreeViewItem_CommonWord* itemObj = static_cast<YL_TreeViewItem_CommonWord*>(index.internalPointer());
    if (itemObj && itemObj->IsLeaf())
    {
        QFont oldfont = painter->font();
        QFont newfont = oldfont;
        newfont.setPointSize(UtilsHelper::ptconvertpx(newfont.pointSize()));
        painter->setFont(newfont);
        //设置节点鼠标事件类型，以及开始绘制的左侧偏移量
        itemObj->SetLevel(1);
        this->SetNodeState(painter, option, index, itemObj);

        //计算 左侧 图标、文字 的位置

        QRect rectLeftIcon = QRect(option.rect.left() + Space, option.rect.top() + OffsetY, ImgWidth, ImgHeight);
        QRect rectText     = QRect(rectLeftIcon.right() + Space, option.rect.top() + OffsetY, option.rect.width() - Space - SpaceRight, TextHeight);

        //绘制叶子节点
        YL_TreeViewItemLeafNodeData_CommonWord* dataObj = (YL_TreeViewItemLeafNodeData_CommonWord*)itemObj->Ptr();
        if (dataObj)
        {
            //绘制左侧顶头图标(下拉箭头、目录图标、常用语图标)
            QString strLeftIconPath = GetLeftIconPath(itemObj->IsLeaf(), itemObj->Level(), false);
            DrawItemNode(painter, rectLeftIcon, Enum_YL_ResourceType_Pixmap, strLeftIconPath);

            //绘制表头文字
            QString strCommonWord = dataObj->Title();
            if (strCommonWord.isEmpty())
            {
                strCommonWord = dataObj->Content();
            }
            QString strText = FormatTextLength(strCommonWord, rectText, painter, option);
            DrawItemNode(painter, rectText, Enum_YL_ResourceType_Text, strText);
        }
        painter->setFont(oldfont);
    }
}

QSize UIYL_CommonWordListViewItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(0, 24);
}

//设置节点的鼠标事件状态（正常、悬浮、点击）
void UIYL_CommonWordListViewItemDelegate::SetNodeState(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index, YL_TreeViewItem_CommonWord* treeViewItemObj) const
{
    QStyleOptionViewItem tempOption = option;
    if (tempOption.state & QStyle::State_MouseOver)
    {
        treeViewItemObj->SetNodeState(Enum_YL_NodeState_Hover);
    }
    else if (option.state & QStyle::State_Selected)
    {
        treeViewItemObj->SetNodeState(Enum_YL_NodeState_Select);
    }
    else
    {
        treeViewItemObj->SetNodeState(Enum_YL_NodeState_Normal);
    }
    painter->fillRect(option.rect, treeViewItemObj->GetItemBgColor());
}

//根据层级，获取 item 开始绘制的向左偏移量
int UIYL_CommonWordListViewItemDelegate::GetOffsetX(YL_TreeViewItem_CommonWord* treeViewItemObj) const
{
    int nOffsetX = 0;
    int nLevel   = treeViewItemObj->Level();
    if (nLevel < 5)
    {
        nOffsetX = nLevel * 20 - (nLevel * 5 + (nLevel - 1) * 5);
    }
    else
    {
        nOffsetX = 35;
    }
    return nOffsetX;
}

//获取左侧顶头图标路径
QString UIYL_CommonWordListViewItemDelegate::GetLeftIconPath(bool isLeaf, int nLevel, bool isExpend) const
{
    QString strRes = "";
    if (!isLeaf)
    {
        if (nLevel == 1)
        {
            strRes = ":/YL/Resources/YL/CommonWord/btn_oneLevel_normal.png";
            // if (treeViewItemObj->ChildCount() > 0)	//如果没有子节点则不能展开 展开按钮
            {
                if (!isExpend)
                {
                    strRes = ":/YL/Resources/YL/CommonWord/btn_oneLevel_press.png";
                }
            }
        }
        else
        {
            strRes = ":/YL/Resources/YL/CommonWord/icon_twoLevel.png";
        }
    }
    else
    {
        strRes = ":/YL/Resources/YL/CommonWord/icon_threeLevel.png";
    }

    return strRes;
}

//获取右侧功能按钮图标路径
QString UIYL_CommonWordListViewItemDelegate::GetRightBtnIconPath(EYLUI_CtrlName ctrlType) const
{
    QString strRightBtn = "";
    switch (ctrlType)
    {
            //批量删除选中框
        case Enum_YL_CtrlName_Checked:
        {
            break;
        }

            //批量导入
        case Enum_YL_CtrlName_BatchImport:
        {
            strRightBtn = ":/YL/Resources/YL/CommonWord/btn_batchAdd_normal.png";
            break;
        }

            //批量删除
        case Enum_YL_CtrlName_BatchDelete:
        {
            if (!m_IsEditState)
            {
                strRightBtn = ":/YL/Resources/YL/CommonWord/btn_BatchEdit_Normal.png";
            }
            else
            {
                strRightBtn = ":/YL/Resources/YL/CommonWord/btn_QuitEditState_Normal.png";
            }

            break;
        }

            //添加常用语
        case Enum_YL_CtrlName_AddCommonWord:
        {
            strRightBtn = ":/YL/Resources/YL/CommonWord/btn_add_normal.png";
            break;
        }

            //修改常用语
        case Enum_YL_CtrlName_ModifyCommonWord:
        {
            strRightBtn = ":/YL/Resources/YL/CommonWord/btn_edit_normal.png";
            break;
        }

            //删除常用语
        case Enum_YL_CtrlName_DeleteCommonWord:
        {
            strRightBtn = ":/YL/Resources/YL/CommonWord/btn_delete_normal.png";
            break;
        }
    }

    return strRightBtn;
}

//文字超过预定长度，根据格式转换文字长度
QString UIYL_CommonWordListViewItemDelegate::FormatTextLength(QString strText, QRect rectText, QPainter* painter, const QStyleOptionViewItem& option) const
{
    QString strResult = strText;

    QStyleOptionViewItem tempOption = option;
    tempOption.displayAlignment     = Qt::AlignLeft | Qt::AlignVCenter;
    tempOption.textElideMode        = Qt::ElideMiddle;

    QFontMetrics fontMetrics = painter->fontMetrics();
    strResult                = fontMetrics.elidedText(strText, tempOption.textElideMode, rectText.width());

    return strResult;
}

//根据类型绘制item
void UIYL_CommonWordListViewItemDelegate::DrawItemNode(QPainter* painter, QRect rect, EYLUI_ResourceType resType, QString resource) const
{
    switch (resType)
    {
        case Enum_YL_ResourceType_Pixmap:
        {
            QPixmap pixmap;
            pixmap.load(resource);
            painter->drawPixmap(rect, pixmap);
            break;
        }
        case Enum_YL_ResourceType_Text:
        {
            painter->drawText(rect, resource);
            break;
        }
        case Enum_YL_ResourceType_Btn:
        {
            QPixmap pixmap;
            pixmap.load(resource);
            painter->drawPixmap(rect, pixmap);
            break;
        }
    }
}

//设置当前批量编辑状态
void UIYL_CommonWordListViewItemDelegate::SetBatchEditState(bool editState)
{
    m_IsEditState = editState;
}
