#ifndef UIYL_EditFunwidget_H
#define UIYL_EditFunwidget_H

#include <QAction>
#include <QEvent>
#include <QMenu>
#include <QObject>

#include "interface/uiwebviewinterface.h"
#include "ui/common/UI_Btn_Enter.h"

#include "data/MFontData.h"
#include "ui/other/JsInputRichEditHelper.h"
#include "ui/common/switchbutton.h"

#include "ui_UIYL_EditFunwidget.h"
#include "interface/uiyl_inputtextpanelinterface.h"
#include "interface/uiyl_chattoolbarinterface.h"
#include "plat_view_global.h"
class UIYL_EditExtWidget;

class PLAT_VIEW_EXPORT UIYL_EditFunwidget : public QWidget
{
    Q_OBJECT

public:
    UIYL_EditFunwidget(QWidget *parent = NULL);
    ~UIYL_EditFunwidget();
    void SendMessageByBody(QString body, QString actionId = "");
    void AddHtmlText(QString text);
    void InitUnfoldOrCollapse();
    void InitShowToolbarPanel();
    void LoadOtherUi();
    UIYL_InputTextPanelInterface * GetEditPanel();
    void SetReferredMsg(const QString referredMsg );
    int IsShowToolbar();
    void SetHotKey();
    void SetEnterMenuIcon(QString data);
    void ShowMask();

    void ProcessRefeererMsg(QString vid, QString searchName);
    void ProcessRefeererMsgEx(QString oldVid,QString vid,QString str, QString searchName);


signals:

    // CZC:输入功能区返回上下键，切换对话item条目
    void S_KeyUp();
    void S_KeyDown();
    void S_Panel_PredictiveInput_SizeChange();
    void S_ShowEditFunWidget(bool show);
    void S_Btn_AlternateRobotShowOrHide_Clicked();
    void S_TimeoutAutoRobotReplyMessage(QString vid, int flag);

public slots:
    void R_LoginStateChanged(ELOGINSTATUS oldStatus, ELOGINSTATUS newStatus);
    void R_GetInputValue(QString str, int i);



private slots:
    void R_LoadUi(int type);
	



    
    void R_MoveOnSendMenuButton(bool isOn);
    void R_OnSendBtnMenuClicked();
    void R_OnSendBtnClick();
    void R_OnSendBtnMenuClick();
    void R_Btn_ShowToolbarPanel_Clicked();

    void R_OnShortCutKey(QString key);
    void R_OnRichTextChanged(QString txt);
    void R_SetInputType(int type);
    void R_RightBtnClicked(QString selecetText, QString pos);
    void R_ConversationTextChangedEx(QString txt);
    void R_OnActionClicked();
    void R_Btn_UnfoldOrCollapse_Clicked();



private:
    void Init();
    void InitExWidget();
    void InitSwitchButton(SwitchButton *btn, QString tag);
    void InitModel();

    void InitSendButton();
    void InitSendButton(UI_Btn_Enter *objData);

    void CreateEnterMenu(QString data, UI_Btn_Enter *objData);

    bool eventFilter(QObject *obj, QEvent *e);
    void resizeEvent(QResizeEvent *event);
    void InitAlternateRobot();
    void InitManualTakeOver();

    void SetBtnShowToolbarStyle();

    void FormatString(QString &keys, QString str);
    void ShowAlternateRobot();



    QString        GetCurrentVid();


private:
    Ui::UIYL_EditFunwidget ui;
    UIYL_InputTextPanelInterface * m_webView_edit;
    UIYL_ChatToolBarInterface * m_my_chat_toolBarPanel;

    QString                m_tempSendMessage;
    QMap<QString, QString> m_inputTextMap;
    QMap<QString, QString> m_inputRefeererMap;
    QString                m_currentVid;

    QWidget               *m_mask;
    bool                   m_IsUnfold;
    int                    m_nCurTextSize;
    int                    m_IsShowToolbar; // 0 显示  1 不显示   历史与前端沟通结果.
    QAction               *m_actionCopy;
    QAction               *m_actionPaste;
    QMenu                 *m_pMenu;
    QString                m_strSelectText;
    QString                m_referredMsg;
    int                    m_inputType; // 1消息引用 0.普通



    UIYL_EditExtWidget *  m_extWidget;


};

#endif // UIYL_EditFunwidget_H
