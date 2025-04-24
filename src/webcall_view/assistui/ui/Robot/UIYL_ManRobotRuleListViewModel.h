#ifndef UIYL_ManRobotRuleListViewModel_H
#define UIYL_ManRobotRuleListViewModel_H

#include "data/MUIManRobotRuleData.h"
#include "YL_ManRobotRuleLeafNodeItemData.h"
#include "YL_ManRobotRuleListItem.h"
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QStringList>
#include <QVariant>

//以下为自定义model需要实现的一些虚函数，将会被Qt在查询model数据时调用
// headerData: 获取表头第section列的数据
// data: 核心函数，获取某个索引index的元素的各种数据
//      role决定获取哪种数据，常用有下面几种：
//      DisplayRole（默认）：就是界面显示的文本数据
//      TextAlignmentRole：就是元素的文本对齐属性
//      TextColorRole、BackgroundRole：分别指文本颜色、单元格背景色
// flags: 获取index的一些标志，一般不怎么改
// index: Qt向你的model请求一个索引为parent的节点下面的row行column列子节点的元素，在本函数里你需要返回该元素的正确索引
// parent：获取指定元素的父元素
// rowCount: 获取指定元素的子节点个数（下一级行数）
// columnCount: 获取指定元素的列数

class UIYL_ManRobotRuleListViewModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit UIYL_ManRobotRuleListViewModel(QStringList headers, QObject *parent = 0);
    ~UIYL_ManRobotRuleListViewModel();

public:
    QVariant      headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant      data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex   index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex   parent(const QModelIndex &index) const override;
    int           rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int           columnCount(const QModelIndex &parent = QModelIndex()) const override;

    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

public:
    YL_ManRobotRuleListItem *itemFromIndex(const QModelIndex &index) const;
    YL_ManRobotRuleListItem *root();

    QStringList GetIdList();

public:
    void AddManRobotRuleData(MUIManRobotRuleData *objData);
    void ClearAll();
    void MoveItem(YL_ManRobotRuleListItem *curItem, YL_ManRobotRuleListItem *hoverItem);
    void DeleteManRobotRuleItem(QString id);

public:
    YL_ManRobotRuleLeafNodeItemData *GetMainTreeLeafNodeItemDataById(QString id);
    YL_ManRobotRuleListItem         *GetMainTreeItemById(QString id);

    bool FindCustomerTreeLeafNodeItemDataById(QString id);
    bool FindMainTreeItemById(QString id);
    void RemoveCacheItemById(QString id);

private:
    QStringList                                      m_headers;  //表头内容
    YL_ManRobotRuleListItem                         *m_rootItem; //根节点
    QMap<QString, YL_ManRobotRuleLeafNodeItemData *> m_manRobotRuleLeafNodeItemDataList;
    QMap<QString, YL_ManRobotRuleListItem *>         m_manRobotRuleItemList;
};

#endif // UIYL_ManRobotRuleListViewModel_H
