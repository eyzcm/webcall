#pragma once

#include <QDragEnterEvent>
#include <QListView>
#include <QObject>
#include <QStringList>

#include "UIObserverImp.h"
#include "UIYL_ManRobotRule_KeyWord_ListViewItemDelegate.h"
#include "UIYL_ManRobotRule_KeyWord_ListViewModel.h"
#include "YL_ManRobotRuleListItem.h"
#include <QDrag>
#include <QMimeData>

class UIYL_ManRobotRule_KeyWord_ListView : public QListView, public IUIObserverImp
{
    Q_OBJECT
public:
    UIYL_ManRobotRule_KeyWord_ListView(QWidget *parent = nullptr);
    virtual ~UIYL_ManRobotRule_KeyWord_ListView();

public:
    virtual void Update(QString type, QObject *obj);
    virtual void RemoveUpdate(QString type, QString body);

public:
    void Init();
    void InitData();
    void SetListViewType(int type);

    void        CanDragDrop(bool val);
    QStringList GetIdList();

signals:
    void S_RuleKeyWordListItem_Clicked(QString id);
    void S_DragItemChanged();
    void S_SearchListItem_Clicked(QString id);
    void S_SetSearchResultNull(bool result);
    void S_ListViewHasData(bool hasData);

private slots:
    void R_ListItem_Clicked(const QModelIndex &index);
    void R_SearchListItem_Clicked(QString id);
    void R_BtnDeleteListItem_Clicked(QString type, YL_ManRobotRuleListItem *item);

private:
    void SetCurrentItemData(QString id);

    virtual void leaveEvent(QEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dropEvent(QDropEvent *event);

private:
    //其它(1:规则列表  2:规则搜索列表)
    int m_nListViewType;

    //拖拽相关
    bool    m_IsLeftMouse;
    bool    m_IsRightMouse;
    bool    m_IsDrag;
    QString m_id;

    //模型和代理
    UIYL_ManRobotRule_KeyWord_ListViewModel        *m_pModel;
    UIYL_ManRobotRule_KeyWord_ListViewItemDelegate *m_pDelegate;
};
