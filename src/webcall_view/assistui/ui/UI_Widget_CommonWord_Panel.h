#pragma once

#include "UIYL_CommonWordTreeSearchResultWidget.h"
#include "data/MContactItemData.h"
#include "el_topscrollarea.h"
#include "ui_UI_Widget_CommonWord_Panel.h"
#include <QListView>
#include <QPushButton>
#include <QTreeView>
#include <QWidget>

class UI_Widget_CommonWord_Panel : public QWidget
{
    Q_OBJECT

public:
    UI_Widget_CommonWord_Panel(QWidget *parent = 0);
    ~UI_Widget_CommonWord_Panel();
    virtual void showEvent(QShowEvent *event);

public:
    void Init();
    void InitModel();
    void SetSearchEditGetFocus();
    void RefreshUIbyMContactItemData(MContactItemData *contact);
    //批量编辑模式
public slots:
    void R_SetEditWidgetVisible(bool visible); //设置编辑模式面板显隐
    void R_AllCheck_Clicked();
    void R_DeleteCommonWord();
    void R_AllCheckState(bool check);  //设置全选按钮状态
    void R_SureBatchDelete(bool sure); //确定批量删除常用语
    void R_SetBtnDeleteState(bool show);

private slots:
    void R_GetUserPermissionListResult(QString code, QString reason, QString body);
    void R_OnCommonWordTreeSearchLineTextChanged(const QString &text);
    void R_ShowColleagueList();
    void R_CustomSelectChanged();
    void R_RefreshommonWordShowMode(); //刷新常用语显示模式,默认页面   or   treeView页面
    void R_ScrollArea_hide();
    void R_OnActionClicked(QAction *act); //菜单共用槽函数
    void on_companyCommonword_clicked();
    void on_tbtn_cancel_clicked();
    void on_addCommonWord_clicked();
    void on_batchImport_clicked();
    void on_findtoolButton_clicked();
    void on_tbtn_showt_clicked();

private:
    QString GetStringByFontMetrics(QString text);
    void    resizeEvent(QResizeEvent *event);

private:
    Ui::UI_Widget_CommonWord_PanelClass    ui;
    QMap<QString, MContactItemData *>      m_mapCustomer;
    UIYL_CommonWordTreeSearchResultWidget *m_commonWordTreeSearchResultWidget;
    EL_TopScrollArea                      *m_Sarea;   //下拉列表
    MContactItemData                      *m_contact; //当前选择的非个人常用语
    bool                                   m_TextChanged; //查询常用语是否完成
    QMenu                                 *m_showtypeMenu;
};
