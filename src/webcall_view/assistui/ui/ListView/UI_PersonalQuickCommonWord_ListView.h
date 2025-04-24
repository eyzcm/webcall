#pragma once

#include "UIObserverImp.h"
#include "UI_PersonalQuickCommonWord_ListViewItemDelegate.h"
#include "UI_PersonalQuickCommonWord_ListViewItemModel.h"
#include "data/MPersonalQuickCommonWordData.h"
#include "data/UICommonWordInfo.h"
#include <QListView>
#include <QObject>

class UI_PersonalQuickCommonWord_ListView : public QListView, public IUIObserverImp
{
    Q_OBJECT

public:
    UI_PersonalQuickCommonWord_ListView(QWidget* parent = nullptr);
    virtual ~UI_PersonalQuickCommonWord_ListView();

public:
    virtual void Update(QString type, QObject* obj);
    virtual void RemoveUpdate(QString type, QString body);

public:
    void Init();

signals:
    void S_ListItemDoubleClicked(MPersonalQuickCommonWordData* data);
    void S_RemovePersonalQuickCommonWord(QString id);
    void S_ModifyPersonalQuickCommonWord(MPersonalQuickCommonWordData* data);

private slots:
    void R_ListItemClicked(const QModelIndex& index);
    void R_BtnClicked(QString type, YL_PersonalQuickTreeViewItem_CommonWord* item);

    void R_TimeOut();

private:
    UI_PersonalQuickCommonWord_ListViewItemModel*    m_pListViewItemModel;
    UI_PersonalQuickCommonWord_ListViewItemDelegate* m_pListViewItemDelegate;

    QTimer*                                  m_pTimer;
    bool                                     m_pDoubleClicked;
    YL_PersonalQuickTreeViewItem_CommonWord* m_pClickedItem;
};
