#pragma once
#include "YL_ManRobotRuleLeafNodeItemData.h"
#include "YL_ManRobotRuleListItem.h"
#include <QPainter>
#include <QPushButton>
#include <QScopedPointer>
#include <QStyledItemDelegate>
#include <QWidget>

class UIYL_ManRobotRuleListViewItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    UIYL_ManRobotRuleListViewItemDelegate(QObject *parent = nullptr);
    ~UIYL_ManRobotRuleListViewItemDelegate();

public:
    void  paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected:
    virtual bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

signals:
    void S_BtnClicked(QString type, YL_ManRobotRuleListItem *item);

private:
    void                     DoDrawPixmap(QPainter *painter, const QStyleOptionViewItem &option, QRect rect, QPixmap &pixmap, QString pixname) const;
    void                     DoDrawText(QPainter *painter, const QStyleOptionViewItem &option, QRect rect, QString text) const;
    void                     DoDrawNameText(QPainter *painter, const QStyleOptionViewItem &option, QRect rect, QString text) const;
    YL_ManRobotRuleListItem *ItemFromIndex(const QModelIndex &index) const;

    void DoDrawManRobotRuleLeafNode(YL_ManRobotRuleListItem *item, YL_ManRobotRuleLeafNodeItemData *nodeItem, const QStyleOptionViewItem &option, QPainter *painter) const;

private:
    QStringList GetButtonListByData(YL_ManRobotRuleLeafNodeItemData *data) const;
    void        paintButton(QPainter *painter, const QStyleOptionViewItem &option, YL_ManRobotRuleLeafNodeItemData *leafData) const;

private:
    QPoint                      m_mousePoint; // 鼠标位置
    QScopedPointer<QPushButton> m_pButton;
    int                         m_nSpacing; // 按钮之间的间距
    int                         m_nWidth;   // 按钮宽度
    int                         m_nHeight;  // 按钮高度
    int                         m_nType;    // 按钮状态-1：划过 2：按下
};
