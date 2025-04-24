#pragma once

#include <QApplication>
#include <QFontMetrics>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QScopedPointer>
#include <QStyledItemDelegate>
#include <QToolTip>

#include "UIYL_ManRobotRule_KeyWord_ListViewModel.h"
#include "YL_ManRobotRuleLeafNodeItemData.h"
#include "YL_ManRobotRuleListItem.h"

class UIYL_ManRobotRule_KeyWord_ListViewItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    UIYL_ManRobotRule_KeyWord_ListViewItemDelegate(int listType, QObject *parent = nullptr);
    ~UIYL_ManRobotRule_KeyWord_ListViewItemDelegate();

signals:
    void S_BtnDeleteListItem_Clicked(QString type, YL_ManRobotRuleListItem *item);

private:
    void         paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize        sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

private:
    YL_ManRobotRuleListItem *ItemFromIndex(const QModelIndex &index) const;
    void                     DoDrawItemBgColor(QPainter *painter, const QStyleOptionViewItem &option, YL_ManRobotRuleListItem *ruleListItem) const;
    void                     DoDrawManRobotRuleLeafNode(YL_ManRobotRuleListItem *item, YL_ManRobotRuleLeafNodeItemData *nodeItem, const QStyleOptionViewItem &option, QPainter *painter) const;
    void                     DoDrawNameText(QPainter *painter, const QStyleOptionViewItem &option, QRect rect, QString text) const;

    QStringList GetButtonListByData(YL_ManRobotRuleLeafNodeItemData *data) const;
    void        paintButton(QPainter *painter, const QStyleOptionViewItem &option, YL_ManRobotRuleLeafNodeItemData *leafData) const;

private:
    int m_nListViewType; //规则列表类型（1:规则列表  2:搜索结果列表）

    int                         m_nWidth;   // 按钮宽度
    int                         m_nHeight;  // 按钮高度
    int                         m_nSpacing; // 按钮之间的间距
    int                         m_nBtnType; // 按钮状态:0-划过 1-按下
    QPoint                      m_mousePoint;
    QScopedPointer<QPushButton> m_pButton;
};
