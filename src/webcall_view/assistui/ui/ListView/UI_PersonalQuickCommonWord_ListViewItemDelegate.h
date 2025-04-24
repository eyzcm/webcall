#pragma once
#include "data\YLData\YL_PersonalQuickTreeViewItemLeafNodeData_CommonWord.h"
#include "data\YLData\YL_PersonalQuickTreeViewItem_CommonWord.h"
#include <QApplication>
#include <QPainter>
#include <QPushButton>
#include <QStyledItemDelegate>
#include <QTreeView>
#include <QWidget>

class UI_PersonalQuickCommonWord_ListViewItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    UI_PersonalQuickCommonWord_ListViewItemDelegate(QObject *parent = nullptr);
    ~UI_PersonalQuickCommonWord_ListViewItemDelegate();

private:
    void  paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected:
    virtual bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

signals:
    void S_BtnClicked(QString type, YL_PersonalQuickTreeViewItem_CommonWord *item);

private:
    void                                     DoDrawPixmap(QPainter *painter, const QStyleOptionViewItem &option, QRect rect, QPixmap &pixmap) const;
    void                                     DoDrawText(QPainter *painter, const QStyleOptionViewItem &option, QRect rect, QString text) const;
    void                                     DoDrawNameText(QPainter *painter, const QStyleOptionViewItem &option, QRect rect, QString text) const;
    YL_PersonalQuickTreeViewItem_CommonWord *ItemFromIndex(const QModelIndex &index) const;

    void DoDrawManRobotRuleLeafNode(YL_PersonalQuickTreeViewItem_CommonWord *item, YL_PersonalQuickTreeViewItemLeafNodeData_CommonWord *nodeItem, const QStyleOptionViewItem &option, QPainter *painter) const;

private:
    QStringList GetButtonListByData(YL_PersonalQuickTreeViewItemLeafNodeData_CommonWord *data) const;
    void        paintButton(QPainter *painter, const QStyleOptionViewItem &option, YL_PersonalQuickTreeViewItemLeafNodeData_CommonWord *leafData) const;

private:
    QPoint                      m_mousePoint; // 鼠标位置
    QScopedPointer<QPushButton> m_pButton;
    int                         m_nSpacing; // 按钮之间的间距
    int                         m_nWidth;   // 按钮宽度
    int                         m_nHeight;  // 按钮高度
    int                         m_nType;    // 按钮状态-1：划过 2：按下
};
