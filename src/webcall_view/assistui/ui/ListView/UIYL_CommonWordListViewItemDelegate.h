#pragma once
#include "data\YLData\YL_TreeViewItemLeafNodeData_CommonWord.h"
#include "data\YLData\YL_TreeViewItem_CommonWord.h"
#include <QApplication>
#include <QPainter>
#include <QStyledItemDelegate>
#include <QTreeView>
#include <QWidget>

class UIYL_CommonWordListViewItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    UIYL_CommonWordListViewItemDelegate(QObject *parent = nullptr);
    ~UIYL_CommonWordListViewItemDelegate();

private:
    void  paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    //设置节点的鼠标事件状态（正常、悬浮、点击）
    void SetNodeState(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, YL_TreeViewItem_CommonWord *treeViewItemObj) const;

    //根据层级，获取 item 开始绘制的向左偏移量
    int GetOffsetX(YL_TreeViewItem_CommonWord *treeViewItemObj) const;

    //获取左侧顶头图标路径
    QString GetLeftIconPath(bool isLeaf, int nLevel, bool isExpend) const;

    //获取右侧功能按钮图标路径
    QString GetRightBtnIconPath(EYLUI_CtrlName ctrlType) const;

    //文字超过预定长度，根据格式转换文字长度
    QString FormatTextLength(QString strText, QRect rectText, QPainter *painter, const QStyleOptionViewItem &option) const;

    //根据类型绘制item
    void DrawItemNode(QPainter *painter, QRect rect, EYLUI_ResourceType resType, QString resource) const;

public:
    //设置当前批量编辑状态
    void SetBatchEditState(bool editState);

private:
    bool m_IsEditState;
};
