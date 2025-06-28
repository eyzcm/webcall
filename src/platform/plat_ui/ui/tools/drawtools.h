#ifndef DRAWTOOLS_H
#define DRAWTOOLS_H

#include <QObject>
#include <QPainter>
#include <QStyleOptionViewItem>
#include "utils/EnumDefine.h"

#include "data/YLData/YL_CustomerTreeItem.h"
#include "data/YLData/YL_MainTreeItem.h"
#include "data/YLData/YL_MainTreeLeafNodeItemData.h"
#include "data/YLData/YL_TreeViewItem_CommonWord.h"




class DrawTools:public  QObject
{
public:
    DrawTools();

    static void paintButton(QPainter *painter, const QStyleOptionViewItem &option, YL_MainTreeLeafNodeItemData *leafData) ;
    static void DoDrawPixmap(QPainter *painter, const QStyleOptionViewItem &option, QRect rect, QPixmap &pixmap, QString pixname = "") ;
    static void DoDrawText(QPainter *painter, const QStyleOptionViewItem &option, QRect rect, QString text, Qt::Alignment align = Qt::AlignLeft) const;
    static void DoDrawNameText(QPainter *painter, const QStyleOptionViewItem &option, QRect rect, QString text, EPRESENCESTATUS state, EYLUI_CustomerNameType drawType, Qt::TextElideMode em = Qt::ElideMiddle) const;
    static void DoTwinkleDrawNameText(QPainter *painter, const QStyleOptionViewItem &option, QRect rect, QString text, EPRESENCESTATUS state, EYLUI_CustomerNameType drawType, QString color) const;
    static void  DoDrawText_ByTwinkleColorOrKey(QPainter *painter, const QStyleOptionViewItem &option, QRect rect, EPRESENCESTATUS state, EYLUI_CustomerNameType type, QString strResource, QString startKey, QString endKey, QString twinkleColor) const;
    static void  DoDrawMainTreeLeafNode(YL_MainTreeItem *yl_mainTreeItemObj, YL_MainTreeLeafNodeItemData *nodeItem, const QStyleOptionViewItem &option, QPainter *painter, UIYL_MainTreeViewModel *model) const;
    static void  DrawManRobotTip(UIYL_MainTreeViewModel *model, YL_MainTreeLeafNodeItemData *nodeItem, const QStyleOptionViewItem &option, QPainter *painter) const;
    static void  DrawEnlargeSelectManRobotTip(UIYL_MainTreeViewModel *model, YL_MainTreeLeafNodeItemData *nodeItem, const QStyleOptionViewItem &option, QPainter *painter, int nStep) const;
    static void DrawManRobotTagTip(YL_MainTreeLeafNodeItemData *nodeItem, const QStyleOptionViewItem &option, QPainter *painter, QString type, int nStep) const;
     static void DrawEnlargeSelectManRobotTagTip(YL_MainTreeLeafNodeItemData *nodeItem, const QStyleOptionViewItem &option, QPainter *painter, QString type, int nStep) const;
     static void  DoDrawMainTreeNode(YL_MainTreeItem *yl_mainTreeItemObj, YL_MainTreeNodeItemData *nodeItem, const QStyleOptionViewItem &option, QPainter *painter) const;
     static void  DrawIconByType(YL_MainTreeLeafNodeItemData *nodeItem, const QStyleOptionViewItem &option, QPainter *painter, QString type, int nStep) const;
     static void  DrawEnlargeSelectIconByType(YL_MainTreeLeafNodeItemData *nodeItem, const QStyleOptionViewItem &option, QPainter *painter, QString type, int nStep) const;
     static void  DrawNodeLeafNoReplayTime(QPainter *painter, YL_MainTreeLeafNodeItemData *nodeItem, YL_MainTreeItem *yl_mainTreeItemObj, const QStyleOptionViewItem &option) const;
     static void  DrawNodeLeafNoReadIcon(QPainter *painter, YL_MainTreeLeafNodeItemData *nodeItem, const QStyleOptionViewItem &option) const;
     static void  DoCardIconToLeafNode(YL_MainTreeLeafNodeItemData *nodeItem, const QStyleOptionViewItem &option, QPainter *painter) const;
     static void  DrawNodeName(YL_MainTreeNodeItemData *nodeItem, YL_MainTreeItem *yl_mainTreeItemObj, const QStyleOptionViewItem &option, QPainter *painter, int nStep) const;
     static void  DrawNodeCustomerIcon(YL_MainTreeNodeItemData *nodeItem, const QStyleOptionViewItem &option, QPainter *painter, int nStep) const;
     static void  DrawNodeExpendIcon(YL_MainTreeNodeItemData *nodeItem, const QStyleOptionViewItem &option, QPainter *painter, int nStep) const;
     static void  DrawNodeNoReadIcon(YL_MainTreeNodeItemData *nodeItem, const QStyleOptionViewItem &option, QPainter *painter, YL_MainTreeItem *yl_mainTreeItemObj) const;
     static void  DrawNodeFilterIcon(YL_MainTreeNodeItemData *nodeItem, const QStyleOptionViewItem &option, QPainter *painter) const;


};

#endif // DRAWTOOLS_H
