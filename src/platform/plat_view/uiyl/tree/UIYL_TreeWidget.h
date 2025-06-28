#ifndef UIYL_TreeWidget_H
#define UIYL_TreeWidget_H

#include "utils/Plat_EnumDefine.h"

#include "data/Im_Message.h"
#include "ui/switchbutton.h"
#include "ui_UIYL_TreeWidget.h"
#include <QMouseEvent>
#include <QWidget>
#include "interface/uiyl_chattreeinterface.h"

class UIYL_CustomerTreeSearchResultWidget;
class UIYL_MainTreeSearchResultWidget    ;
class UIYL_RecentTreeSearchResultWidget;
class UIYL_TreeViewInterface;

class UIYL_TreeWidget : public UIYL_ChatTreeInterface
{
    Q_OBJECT

public:
    UIYL_TreeWidget(QWidget *parent = 0);
    ~UIYL_TreeWidget();

    void Init();

    /* 曹振超：2019年8月20日 */
    void InitSwitchButton(SwitchButton *btn, bool isCheck);

protected:
    virtual bool eventFilter(QObject *obj, QEvent *e);

signals:
    void S_OnYLMainTreeSelectTabChanged(int oldTab, int newTab);
    void S_ShowNotice_Clicked();
    void S_BtnCloseNotice_Clicked();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

public slots:
    
    void R_LoginStateChanged(ELOGINSTATUS oldStatus, ELOGINSTATUS newStatus);


private slots:
    void R_LoadUi(int type );
    void R_MainSelectChat(QString vid);
    void R_OnFlashingTreeView(bool isFlashing);
    void R_StartChat(QString customerId);

    void R_OnClickMainVisitorSearchItem(QString vid, QString type);
    void R_ReceiveMessage(const Im_Message &msg);


    void R_OnTreeBtnClick();
    void R_OnTreeSearchLineTextChanged(const QString &text);

private:
    void HideWnd();
    void OnTreeSearchLineTextChanged(UIYL_TreeViewInterface * sender, const QString &text);
    void OnTreeBtnClick(UIYL_TreeViewInterface * treeview);

private:
    Ui::UIYL_TreeWidget ui;

    int                        m_treeTab;
    /*
    UIYL_CustomerTreeSearchResultWidget *m_customerTreeSearchResultWnd;
    UIYL_MainTreeSearchResultWidget     *m_mainTreeSearchResultWnd;
    UIYL_RecentTreeSearchResultWidget   *m_recentTreeSearchResultWnd;
*/
    QList<UIYL_TreeViewInterface *> m_treeList;


    bool                                 m_receverMsgFlashing;
    int                                  m_number;
};

#endif // UIYL_TreeWidget_H
