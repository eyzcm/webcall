#ifndef UIYL_ManRobotRuleSearchResultListView_H
#define UIYL_ManRobotRuleSearchResultListView_H

#include "UIObserverImp.h"
#include "UIYL_ManRobotRuleSearchResultListViewItemDelegate.h"
#include "UIYL_ManRobotRuleSearchResultListViewModel.h"
#include <QDrag>
#include <QDragEnterEvent>
#include <QListView>
#include <QMimeData>
#include <QObject>

class UIYL_ManRobotRuleSearchResultListView : public QListView, public IUIObserverImp
{
    Q_OBJECT
public:
    UIYL_ManRobotRuleSearchResultListView(QWidget *parent = NULL);
    virtual ~UIYL_ManRobotRuleSearchResultListView();

private:
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dropEvent(QDropEvent *event);

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

signals:
    void S_OnClickSearchItem(QString id);
    void S_SetSearchResultNull(bool result);

public:
    virtual void Update(QString type, QObject *obj);
    virtual void RemoveUpdate(QString type, QString body);

public:
    void Init();
    void InitData();

    void CanDragDrop(bool val);

private slots:
    void R_OnTreeItemClicked(const QModelIndex &index);

private:
    UIYL_ManRobotRuleSearchResultListViewModel        *m_UIYL_ManRobotRuleSearchResultListViewModel;
    UIYL_ManRobotRuleSearchResultListViewItemDelegate *m_UIYL_ManRobotRuleSearchResultListViewItemDelegate;
    ///拖拽相关///////////////////////////////////////////////////////////////////////
    bool m_IsLeftMouse;
    bool m_IsDrag;
    bool m_IsRightMouse;
};

#endif // UIYL_ManRobotRuleSearchResultListView_H
