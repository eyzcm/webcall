#pragma once

#include "data\YLData\YL_TreeViewItemLeafNodeData_CommonWord.h"
#include "data\YLData\YL_TreeViewItem_CommonWord.h"
#include <QAbstractItemModel>
#include <QColor>
#include <QModelIndex>
#include <QStringList>
#include <QVariant>

class UIYL_CommonWordListViewModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    UIYL_CommonWordListViewModel(QStringList headers, QObject *parent = nullptr);
    ~UIYL_CommonWordListViewModel();

public:
    QVariant      headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant      data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex   index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex   parent(const QModelIndex &index) const override;
    int           rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int           columnCount(const QModelIndex &parent = QModelIndex()) const override;

public:
    YL_TreeViewItem_CommonWord *itemFromIndex(const QModelIndex &index) const;
    YL_TreeViewItem_CommonWord *root();

public:
    void AddCommonWord(MCommonWordData *objData);
    void ClearAll();

private:
    QStringList                 m_headers;  //表头内容
    YL_TreeViewItem_CommonWord *m_rootItem; //根节点
};
