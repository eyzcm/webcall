#include "UIYL_ManRobotRuleSearchResultListViewItemDelegate.h"
#include "UIYL_ManRobotRuleListViewModel.h"
#include "YL_ManRobotRuleLeafNodeItemData.h"
#include "utils/utilshelper.h"
#include <QApplication>
#include <QMouseEvent>
#include <QToolTip>
#include <QTreeView>

UIYL_ManRobotRuleSearchResultListViewItemDelegate::UIYL_ManRobotRuleSearchResultListViewItemDelegate(QObject *parent) : QStyledItemDelegate(parent), m_pButton(new QPushButton()), m_nSpacing(4), m_nWidth(15), m_nHeight(15)
{
    // 设置按钮正常、划过、按下样式
    m_pButton->setStyleSheet("QPushButton{border-image: url(:/YL/Resources/YL/MainTree/chatbutton/btn_chatlist_normal.png);}\
                                                         QPushButton:hover{border-image: url(:/YL/Resources/YL/MainTree/chatbutton/btn_chatList_hover.png);}\
                                                         QPushButton:pressed{border-image: url(:/YL/Resources/YL/MainTree/chatbutton/btn_chatList_hover.png);}");
}
UIYL_ManRobotRuleSearchResultListViewItemDelegate::~UIYL_ManRobotRuleSearchResultListViewItemDelegate()
{
}

QSize UIYL_ManRobotRuleSearchResultListViewItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(0, 30);
}

bool UIYL_ManRobotRuleSearchResultListViewItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return QStyledItemDelegate::editorEvent(event, model, option, index);
    m_nType             = -1;
    QMouseEvent *pEvent = static_cast<QMouseEvent *>(event);
    m_mousePoint        = pEvent->pos();

    // if (m_chatListDisplayStyle == 0)
    //{
    //	return false;
    // }

    bool bRepaint = false;

    YL_ManRobotRuleListItem *yl_mainTreeItemObj = ItemFromIndex(index);
    bool                     isRet              = true;
    do
    {
        if (yl_mainTreeItemObj)
        {
            if (yl_mainTreeItemObj->NodeState() == Enum_YL_NodeState_Select)
            {
                //选中的叶子节点
                isRet = false;
            }
        }
    } while (false);
    /*if (isRet)
    {
            return bRepaint;
    }*/

    YL_ManRobotRuleLeafNodeItemData *leafData = (YL_ManRobotRuleLeafNodeItemData *)yl_mainTreeItemObj->Ptr();

    QStringList buttonList = GetButtonListByData(leafData);
    int         nCount     = buttonList.count();
    int         nTop       = option.rect.top() + (option.rect.height() - m_nHeight) / 2;

    // 还原鼠标样式
    QApplication::restoreOverrideCursor();

    for (int i = 0; i < nCount; ++i)
    {
        QStyleOptionButton button;
        button.rect = QRect(option.rect.left() + option.rect.width() - m_nWidth * (i + 1) + m_nSpacing * i - 12, nTop, m_nWidth, m_nHeight);

        // 鼠标位于按钮之上
        if (!button.rect.contains(m_mousePoint))
            continue;

        bRepaint = true;
        switch (event->type())
        {
                // 鼠标滑过
            case QEvent::MouseMove:
            {
                // 设置鼠标样式为手型
                // QApplication::setOverrideCursor(Qt::PointingHandCursor);

                m_nType     = 0;
                QString key = buttonList.at(i);
                if (key.indexOf("Close", 0, Qt::CaseInsensitive) != -1)
                {
                    QToolTip::showText(pEvent->globalPos(), tr("删除"));
                }
                else
                {
                    Q_ASSERT(false);
                }

                break;
            }
                // 鼠标按下
            case QEvent::MouseButtonPress:
            {
                m_nType = 1;
                break;
            }
                // 鼠标释放
            case QEvent::MouseButtonRelease:
            {
                Q_EMIT S_BtnClicked(buttonList.at(i), yl_mainTreeItemObj);
                break;
            }
            default:
                break;
        }
    }

    return bRepaint;
}

void UIYL_ManRobotRuleSearchResultListViewItemDelegate::DoDrawPixmap(QPainter *painter, const QStyleOptionViewItem &option, QRect rect, QPixmap &pixmap, QString pixname) const
{
    QStyleOptionViewItem tempOption = option;
    tempOption.decorationAlignment  = Qt::AlignCenter | Qt::AlignVCenter;
    tempOption.decorationSize       = QSize(rect.width(), rect.height());

    pixmap = UtilsHelper::ChangeQPixmapDPI(pixmap, pixname, tempOption.decorationSize);

    QApplication::style()->drawItemPixmap(painter, rect, tempOption.decorationAlignment, pixmap);
}

void UIYL_ManRobotRuleSearchResultListViewItemDelegate::DoDrawText(QPainter *painter, const QStyleOptionViewItem &option, QRect rect, QString text) const
{
    QStyleOptionViewItem tempOption = option;
    tempOption.displayAlignment     = Qt::AlignCenter;

    //绘制文本
    QApplication::style()->drawItemText(painter, rect, tempOption.displayAlignment, QApplication::palette(), true, text);
}

void UIYL_ManRobotRuleSearchResultListViewItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    YL_ManRobotRuleListItem *YL_ManRobotRuleListItemObj = static_cast<YL_ManRobotRuleListItem *>(index.internalPointer());
    if (YL_ManRobotRuleListItemObj)
    {
        QStyleOptionViewItem tempOption = option;

        if (tempOption.state & QStyle::State_MouseOver)
        {
            YL_ManRobotRuleListItemObj->SetNodeState(Enum_YL_NodeState_Hover);
        }
        else if (option.state & QStyle::State_Selected)
        {
            YL_ManRobotRuleListItemObj->SetNodeState(Enum_YL_NodeState_Select);
        }
        else
        {
            YL_ManRobotRuleListItemObj->SetNodeState(Enum_YL_NodeState_Normal);
        }
        painter->fillRect(tempOption.rect, YL_ManRobotRuleListItemObj->GetItemBgColor());

        if (YL_ManRobotRuleListItemObj->Leaf())
        {
            YL_ManRobotRuleLeafNodeItemData *dataObj = (YL_ManRobotRuleLeafNodeItemData *)YL_ManRobotRuleListItemObj->Ptr();
            if (tempOption.state & QStyle::State_MouseOver)
            {
                paintButton(painter, option, dataObj);
            }

            //叶子节点

            DoDrawManRobotRuleLeafNode(YL_ManRobotRuleListItemObj, dataObj, tempOption, painter);
        }
        else
        {
            Q_ASSERT(false);
        }
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

void UIYL_ManRobotRuleSearchResultListViewItemDelegate::DoDrawManRobotRuleLeafNode(YL_ManRobotRuleListItem *item, YL_ManRobotRuleLeafNodeItemData *nodeItem, const QStyleOptionViewItem &option, QPainter *painter) const
{
    if (nodeItem)
    {

        QString strName = nodeItem->Name();
        int     nLeft_x = option.rect.left() + 5;
        int     nTop_y  = option.rect.top();

        QRect rectName = QRect(nLeft_x, nTop_y, option.rect.width() - nLeft_x, option.rect.height());
        DoDrawNameText(painter, option, rectName, strName);
    }
}

void UIYL_ManRobotRuleSearchResultListViewItemDelegate::DoDrawNameText(QPainter *painter, const QStyleOptionViewItem &option, QRect rect, QString text) const
{
    QStyleOptionViewItem tempOption = option;
    tempOption.displayAlignment     = Qt::AlignLeft | Qt::AlignVCenter;
    tempOption.textElideMode        = Qt::ElideMiddle;

    QFontMetrics fontMetrics = painter->fontMetrics();
    text                     = fontMetrics.elidedText(text, tempOption.textElideMode, rect.width());

    //绘制文本
    QApplication::style()->drawItemText(painter, rect, tempOption.displayAlignment, QApplication::palette(), true, text);
}

QStringList UIYL_ManRobotRuleSearchResultListViewItemDelegate::GetButtonListByData(YL_ManRobotRuleLeafNodeItemData *data) const
{
    QStringList retList;
    if (data == nullptr)
    {
        return retList;
    }

    if (data->State())
    {
        retList.push_back("Close");
    }
    return retList;
}

void UIYL_ManRobotRuleSearchResultListViewItemDelegate::paintButton(QPainter *painter, const QStyleOptionViewItem &option, YL_ManRobotRuleLeafNodeItemData *leafData) const
{
    return;
    QStringList buttonList = GetButtonListByData(leafData);
    // 计算按钮显示区域
    int nCount = buttonList.count();
    int nTop   = option.rect.top() + (option.rect.height() - m_nHeight) / 2;

    for (int i = 0; i < nCount; ++i)
    {
        // 绘制按钮
        QStyleOptionButton button;
        button.rect = QRect(option.rect.left() + option.rect.width() - m_nWidth * (i + 1) - m_nSpacing * i - 12, nTop, m_nWidth, m_nHeight);
        button.state |= QStyle::State_Enabled;
        button.iconSize = QSize(15, 15);

        if (button.rect.contains(m_mousePoint))
        {
            if (m_nType == 0)
            {
                button.state |= QStyle::State_MouseOver;
            }
            else if (m_nType == 1)
            {
                button.state |= QStyle::State_Sunken;
            }
        }

        QString key = buttonList.at(i);

        if (key.indexOf("Close", 0, Qt::CaseInsensitive) != -1)
        {
            button.icon = QIcon(QString(":/YL/Resources/YL/MainTree/chatbutton/close.png"));
        }
        else
        {
            Q_ASSERT(false);
        }

        QWidget *pWidget = m_pButton.data();
        pWidget->style()->drawControl(QStyle::CE_PushButton, &button, painter, pWidget);
    }
}

YL_ManRobotRuleListItem *UIYL_ManRobotRuleSearchResultListViewItemDelegate::ItemFromIndex(const QModelIndex &index) const
{
    if (!index.isValid())
        return NULL;
    YL_ManRobotRuleListItem *item = static_cast<YL_ManRobotRuleListItem *>(index.internalPointer());

    return item;
}
