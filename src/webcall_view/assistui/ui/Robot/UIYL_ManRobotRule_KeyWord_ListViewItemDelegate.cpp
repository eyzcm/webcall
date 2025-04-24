#include "UIYL_ManRobotRule_KeyWord_ListViewItemDelegate.h"

UIYL_ManRobotRule_KeyWord_ListViewItemDelegate::UIYL_ManRobotRule_KeyWord_ListViewItemDelegate(int listType, QObject *parent) : QStyledItemDelegate(parent), m_nListViewType(listType), m_nWidth(15), m_nHeight(15), m_nSpacing(4), m_nBtnType(-1), m_pButton(new QPushButton())
{
    m_pButton->setStyleSheet("QPushButton{border-image: url(:/YL/Resources/YL/MainTree/chatbutton/btn_chatlist_normal.png);}			\
                                                          QPushButton:hover{border-image: url(:/YL/Resources/YL/MainTree/chatbutton/btn_chatList_hover.png);}		\
                                                          QPushButton:pressed{border-image: url(:/YL/Resources/YL/MainTree/chatbutton/btn_chatList_hover.png);}");
}

UIYL_ManRobotRule_KeyWord_ListViewItemDelegate::~UIYL_ManRobotRule_KeyWord_ListViewItemDelegate()
{
    if (m_pButton)
    {
        m_pButton->deleteLater();
    }
}

void UIYL_ManRobotRule_KeyWord_ListViewItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    YL_ManRobotRuleListItem *ruleListItem = static_cast<YL_ManRobotRuleListItem *>(index.internalPointer());
    if (ruleListItem)
    {
        YL_ManRobotRuleLeafNodeItemData *dataObj = (YL_ManRobotRuleLeafNodeItemData *)ruleListItem->Ptr();
        if (dataObj)
        {
            DoDrawItemBgColor(painter, option, ruleListItem);
            DoDrawManRobotRuleLeafNode(ruleListItem, dataObj, option, painter);

            if (m_nListViewType == 1)
            {
                if (option.state & QStyle::State_MouseOver)
                {
                    paintButton(painter, option, dataObj);
                }
            }
        }
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QSize UIYL_ManRobotRule_KeyWord_ListViewItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(0, 30);
}

bool UIYL_ManRobotRule_KeyWord_ListViewItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    bool bRepaint = false;

    if (m_nListViewType != 1)
    {
        return bRepaint;
    }

    YL_ManRobotRuleListItem *ruleListItem = ItemFromIndex(index);
    if (ruleListItem)
    {
        YL_ManRobotRuleLeafNodeItemData *leafData = (YL_ManRobotRuleLeafNodeItemData *)ruleListItem->Ptr();
        if (leafData)
        {
            // 还原鼠标样式
            QApplication::restoreOverrideCursor();

            m_nBtnType = -1;

            QMouseEvent *pEvent = static_cast<QMouseEvent *>(event);
            m_mousePoint        = pEvent->pos();

            QStringList buttonList = GetButtonListByData(leafData);
            int         nCount     = buttonList.count();
            int         nTop       = option.rect.top() + (option.rect.height() - m_nHeight) / 2;

            for (int i = 0; i < nCount; ++i)
            {
                QStyleOptionButton button;
                button.rect = QRect(option.rect.left() + option.rect.width() - m_nWidth * (i + 1) + m_nSpacing * i - 12, nTop, m_nWidth, m_nHeight);

                // 鼠标位于按钮之上
                if (!button.rect.contains(m_mousePoint))
                {
                    continue;
                }

                bRepaint = true;
                switch (event->type())
                {
                    case QEvent::MouseMove: // 鼠标滑过
                    {
                        m_nBtnType  = 0;
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
                    case QEvent::MouseButtonPress: // 鼠标按下
                    {
                        m_nBtnType = 1;
                        break;
                    }
                    case QEvent::MouseButtonRelease: // 鼠标释放
                    {
                        Q_EMIT S_BtnDeleteListItem_Clicked(buttonList.at(i), ruleListItem);
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }

    return bRepaint;
}

YL_ManRobotRuleListItem *UIYL_ManRobotRule_KeyWord_ListViewItemDelegate::ItemFromIndex(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return NULL;
    }

    YL_ManRobotRuleListItem *item = static_cast<YL_ManRobotRuleListItem *>(index.internalPointer());
    return item;
}

void UIYL_ManRobotRule_KeyWord_ListViewItemDelegate::DoDrawItemBgColor(QPainter *painter, const QStyleOptionViewItem &option, YL_ManRobotRuleListItem *ruleListItem) const
{
    if (option.state & QStyle::State_Selected)
    {
        ruleListItem->SetNodeState(Enum_YL_NodeState_Select);
    }
    else if (option.state & QStyle::State_MouseOver)
    {
        ruleListItem->SetNodeState(Enum_YL_NodeState_Hover);
    }
    else
    {
        ruleListItem->SetNodeState(Enum_YL_NodeState_Normal);
    }
    painter->fillRect(option.rect, ruleListItem->GetItemBgColor());
}

void UIYL_ManRobotRule_KeyWord_ListViewItemDelegate::DoDrawManRobotRuleLeafNode(YL_ManRobotRuleListItem *item, YL_ManRobotRuleLeafNodeItemData *nodeItem, const QStyleOptionViewItem &option, QPainter *painter) const
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

void UIYL_ManRobotRule_KeyWord_ListViewItemDelegate::DoDrawNameText(QPainter *painter, const QStyleOptionViewItem &option, QRect rect, QString text) const
{
    QStyleOptionViewItem tempOption = option;
    tempOption.displayAlignment     = Qt::AlignLeft | Qt::AlignVCenter;
    tempOption.textElideMode        = Qt::ElideMiddle;

    QFontMetrics fontMetrics = painter->fontMetrics();
    text                     = fontMetrics.elidedText(text, tempOption.textElideMode, rect.width());

    //绘制文本
    QApplication::style()->drawItemText(painter, rect, tempOption.displayAlignment, QApplication::palette(), true, text);
}

QStringList UIYL_ManRobotRule_KeyWord_ListViewItemDelegate::GetButtonListByData(YL_ManRobotRuleLeafNodeItemData *data) const
{
    QStringList retList;
    if (data == nullptr)
    {
        return retList;
    }

    //状态停用才能删除
    if (!data->State())
    {
        retList.push_back("Close");
    }

    return retList;
}

void UIYL_ManRobotRule_KeyWord_ListViewItemDelegate::paintButton(QPainter *painter, const QStyleOptionViewItem &option, YL_ManRobotRuleLeafNodeItemData *leafData) const
{
    QStringList buttonList = GetButtonListByData(leafData);
    int         nCount     = buttonList.count();
    int         nTop       = option.rect.top() + (option.rect.height() - m_nHeight) / 2;

    for (int i = 0; i < nCount; ++i)
    {
        // 绘制按钮
        QStyleOptionButton button;
        button.rect = QRect(option.rect.left() + option.rect.width() - m_nWidth * (i + 1) - m_nSpacing * i - 12, nTop, m_nWidth, m_nHeight);
        button.state |= QStyle::State_Enabled;
        button.iconSize = QSize(15, 15);

        if (button.rect.contains(m_mousePoint))
        {
            if (m_nBtnType == 0)
            {
                button.state |= QStyle::State_MouseOver;
            }
            else if (m_nBtnType == 1)
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
