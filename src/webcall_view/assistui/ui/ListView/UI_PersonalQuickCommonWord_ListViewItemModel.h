#pragma once

#include "data/MPersonalQuickCommonWordData.h"
#include "data\YLData\YL_PersonalQuickTreeViewItem_CommonWord.h"
#include <QAbstractItemModel>
#include <QColor>
#include <QModelIndex>
#include <QStringList>
#include <QVariant>

class UI_PersonalQuickCommonWord_ListViewItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    UI_PersonalQuickCommonWord_ListViewItemModel(QStringList headers, QObject *parent = nullptr);
    ~UI_PersonalQuickCommonWord_ListViewItemModel();

public:
    QVariant      headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant      data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex   index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex   parent(const QModelIndex &index) const override;
    int           rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int           columnCount(const QModelIndex &parent = QModelIndex()) const override;

public:
    YL_PersonalQuickTreeViewItem_CommonWord *itemFromIndex(const QModelIndex &index) const;
    YL_PersonalQuickTreeViewItem_CommonWord *root();

public:
    void AddPersonalQuickCommonWord(MPersonalQuickCommonWordData *objData);
    void ClearAll();
    void RemoveItemById(QString id);

private:
    QStringList                              m_headers;  //表头内容
    YL_PersonalQuickTreeViewItem_CommonWord *m_rootItem; //根节点
};
