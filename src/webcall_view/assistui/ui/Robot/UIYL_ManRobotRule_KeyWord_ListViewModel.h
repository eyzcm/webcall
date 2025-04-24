#pragma once

#include <QAbstractItemModel>
#include <QColor>
#include <QModelIndex>
#include <QStringList>
#include <QToolTip>
#include <QVariant>

#include "data/MUIManRobotRuleData.h"
#include "YL_ManRobotRuleLeafNodeItemData.h"
#include "YL_ManRobotRuleListItem.h"

class UIYL_ManRobotRule_KeyWord_ListViewModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit UIYL_ManRobotRule_KeyWord_ListViewModel(QStringList headers, QObject *parent = nullptr);
    ~UIYL_ManRobotRule_KeyWord_ListViewModel();

public:
    QVariant      headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex   index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex   parent(const QModelIndex &index) const override;
    int           rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int           columnCount(const QModelIndex &parent = QModelIndex()) const override;

public:
    YL_ManRobotRuleListItem *root();
    YL_ManRobotRuleListItem *itemFromIndex(const QModelIndex &index) const;
    QVariant                 data(const QModelIndex &index, int role) const override;
    bool                     removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    bool FindListItemById(QString id);
    bool FindListLeafNodeItemDataById(QString id);

    QStringList                      GetIdList();
    YL_ManRobotRuleListItem         *GetListItemById(QString id);
    YL_ManRobotRuleLeafNodeItemData *GetListLeafNodeItemDataById(QString id);

public:
    void Add_RuleItem(MUIManRobotRuleData *objData);
    void Move_RuleItem(YL_ManRobotRuleListItem *curItem, YL_ManRobotRuleListItem *hoverItem);
    void Delete_RuleItem(QString id);
    void Clear_RuleItem();
    void RemoveCache_RuleItem_ById(QString id);

private:
    QStringList              m_headers;  //表头内容
    YL_ManRobotRuleListItem *m_rootItem; //根节点

    QMap<QString, YL_ManRobotRuleListItem *>         m_mapRuleItemList;
    QMap<QString, YL_ManRobotRuleLeafNodeItemData *> m_mapRuleLeafNodeItemData;
};
