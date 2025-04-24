#ifndef UIYL_CommonWordListTreeView_H
#define UIYL_CommonWordListTreeView_H

#include "UIObserverImp.h"
#include "UIYL_CommonWordListViewItemDelegate.h"
#include "UIYL_CommonWordListViewModel.h"
#include "data/MChatItemData.h"
#include <QListView>
#include <QObject>

class UIYL_CommonWordListTreeView : public QListView, public IUIObserverImp
{
    Q_OBJECT
public:
    UIYL_CommonWordListTreeView(QWidget* parent = NULL);
    virtual ~UIYL_CommonWordListTreeView();

public:
    virtual void Update(QString type, QObject* obj);
    virtual void RemoveUpdate(QString type, QString body);

public:
    void Init();

private slots:
    void R_OnTreeItemClicked(const QModelIndex& index);
    void R_OnTreeItemdoubleClicked(const QModelIndex& index);

signals:
    void S_OnClickSearchCommonWordItem(QString id);
    void S_OnDoubleClickSearchCommonWordItem(QString id);
    void S_SetSearchResultNull(bool result);

private:
    UIYL_CommonWordListViewModel*        m_UIYL_CommonWordListTreeViewModel;
    UIYL_CommonWordListViewItemDelegate* m_UIYL_CommonWordListTreeViewItemDelegate;
};

#endif // UIYL_CommonWordListTreeView_H
