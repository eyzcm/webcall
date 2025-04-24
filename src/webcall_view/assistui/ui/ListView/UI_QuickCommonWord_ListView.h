#pragma once

#include "UIObserverImp.h"
#include "UI_QuickCommonWord_ListViewItemDelegate.h"
#include "UI_QuickCommonWord_ListViewItemModel.h"
#include "data/UICommonWordInfo.h"
#include <QListView>
#include <QObject>

class UI_QuickCommonWord_ListView : public QListView, public IUIObserverImp
{
    Q_OBJECT

public:
    UI_QuickCommonWord_ListView(QWidget* parent = nullptr);
    virtual ~UI_QuickCommonWord_ListView();

public:
    virtual void Update(QString type, QObject* obj);
    virtual void RemoveUpdate(QString type, QString body);

public:
    void Init();

private slots:
    void R_ListItemClicked(const QModelIndex& index);
    void R_TimeOut();

signals:
    void S_ListItemClicked(MCommonWordData* data);
    void S_ListItemDoubleClicked(MCommonWordData* data);

private:
    UICommonWordInfo* Get_UICommonWordInfo_By_ItemLeafNodeData(YL_TreeViewItemLeafNodeData_CommonWord* itemData);
    MCommonWordData*  GetMCommonWordDataByItemLeafNodeData(YL_TreeViewItemLeafNodeData_CommonWord* itemData);

private:
    UI_QuickCommonWord_ListViewItemModel*    m_pListViewItemModel;
    UI_QuickCommonWord_ListViewItemDelegate* m_pListViewItemDelegate;

    QTimer*                     m_pTimer;
    bool                        m_pDoubleClicked;
    YL_TreeViewItem_CommonWord* m_pClickedItem;
};
