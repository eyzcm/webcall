#ifndef UIYL_ManRobotRuleListView_H
#define UIYL_ManRobotRuleListView_H

#include "UIObserverImp.h"
#include "UIYL_ManRobotRuleListViewItemDelegate.h"
#include "UIYL_ManRobotRuleListViewModel.h"
#include "YL_ManRobotRuleListItem.h"
#include <QDrag>
#include <QDragEnterEvent>
#include <QListView>
#include <QMimeData>
#include <QObject>
#include <QStringList>

class UIYL_ManRobotRuleListView : public QListView, public IUIObserverImp
{
    Q_OBJECT
public:
    UIYL_ManRobotRuleListView(QWidget *parent = NULL);
    virtual ~UIYL_ManRobotRuleListView();

private:
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dropEvent(QDropEvent *event);

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

    void leaveEvent(QEvent *event);

public:
    virtual void Update(QString type, QObject *obj);
    virtual void RemoveUpdate(QString type, QString body);
    void         Init();
    void         InitData();

    void        CanDragDrop(bool val);
    QStringList GetIdList();
signals:
    void S_OnClickManRobotRule(QString id);
    void S_DragItemChanged();

private slots:
    void R_OnTreeItemClicked(const QModelIndex &index);
    void R_OnClickManRobotRuleSearchItem(QString id);
    void R_BtnClicked(QString type, YL_ManRobotRuleListItem *item);

private:
    void                                   SetCurrentItemData(QString id);
    UIYL_ManRobotRuleListViewModel        *m_UIYL_ManRobotRuleListViewModel;
    UIYL_ManRobotRuleListViewItemDelegate *m_UIYL_ManRobotRuleListViewItemDelegate;
    ///拖拽相关///////////////////////////////////////////////////////////////////////
    bool    m_IsLeftMouse;
    bool    m_IsDrag;
    bool    m_IsRightMouse;
    QString m_id;
};

#endif // UIYL_ManRobotRuleListView_H
