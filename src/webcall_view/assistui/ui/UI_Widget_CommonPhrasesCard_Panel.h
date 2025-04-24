#pragma once

#include "utils/EnumDefine.h"
#include "ui/common/UI_WebView_ShieldBackSpace.h"
#include "UI_Wnd_AddPersonalQuickCommonWord.h"
#include "switchbutton.h"
#include "js/JsCommonfilesHelper.h"
#include "ui_UI_Widget_CommonPhrasesCard_Panel.h"
#include "data/UIFormConfigInfo.h"
#include <QList>
#include <QListWidgetItem>
#include <QWidget>

enum Panel_Index
{
    E_Index_Common           = 0,
    E_Index_Card             = 1,
    E_Index_Common_files     = 2, //常用文件
    E_Index_lemonLovesBeauty = 3,
    E_Index_newTab           = 4,
    E_Index_ai_build         = 5,
    E_Index_ai_modify        = 6,
    E_Index_ExtTab           = 7
};

class UI_Widget_CommonPhrasesCard_Panel : public QWidget
{
    Q_OBJECT

public:
    UI_Widget_CommonPhrasesCard_Panel(QWidget* parent = 0);
    ~UI_Widget_CommonPhrasesCard_Panel();

public:
    void InitSelf();
    void InitSwitchPanel();
    void InitStackedWidgetPanel();
    void InitQuickCommonWordPanel();
    void InitPersonalQuickCommonWordPanel();
    void InitModel();
    void InitRegisterSignal();
    void InitSwitchButton(SwitchButton* btn, bool isCheck);
    void InitPanelVisible();
    void BtnTab_Clicked(int tabIndex);

public:
    void SetFontBold(SwitchButton* btn, bool bold);

public slots:
    void R_OnLeftClick();
    void R_OnRightClick();
    void R_LoginStateChanged(ELOGINSTATUS oldStatus, ELOGINSTATUS newStatus);
    void R_BtnCommonWord_Clicked();
    void R_BtnCard_Clicked();
    void R_BtnLemonLovesBeauty_Clicked();
    void R_BtnNewTab_Clicked();
    void R_BtnAIBuild_Clicked();
    void R_BtnAIModify_Clicked();
    void R_BtnTelescopic_Clicked();
    void R_ShowSettingChanged();
    void R_ShowAiTab(int type, QString val);

    void R_Btn_Clicked();

    //监听主树界面tab切换消息，当为同事对话时，设置当前选中为常用语，并且隐藏名片按钮
    void R_MainTreeSelectTabChanged(E_YL_TREE_TAB oldType, E_YL_TREE_TAB newTypw);
    void R_OnSelectedChatItemData(MChatItemData* chatdata);

    // void R_OnCommonWordTreeItemDoubleClick(MCommonWordData* dataObj);
    // void R_OnCommonWordTreeItemClick(MCommonWordData* dataObj);
    // void R_CurrentTabPageChanged(E_TABTYPE oldType, E_TABTYPE curType);
    // bool IsCheckCurrTab();

    // CZC_个人设置保存成功，显隐“柠檬爱美”
    void R_PersonMainSettingSaveSuccess();

    // CZC:个人设置-快捷键
    void R_Key_CommonWordSearch();
    void R_Key_OpenCard();

    void R_OnRebotCheckedClick();

    void R_RebotItemClicked(QListWidgetItem* item);
    void R_ShowRebotList(QList<CommInfo*>& list);

    void R_BtnAddPersonalQuick_Clicked();
    void R_BtnExpand_Clicked();
    void R_ModifyPersonalQuickCommonWord(MPersonalQuickCommonWordData* data);
    void R_RemovePersonalQuickCommonWord(QString id);
    void R_RemovePersonalQuickCommonWordResultOK(bool isOK);

    void R_GetUserFormConfigResult(QString code, QString reason, QString body);

private slots:
    void on_btn_Common_files_clicked();

private:
    virtual void resizeEvent(QResizeEvent* e);
    void         DoSearchRebotList();
    void         keyPressEvent(QKeyEvent* arg);

    void         InitSwitchTabButton(int tabIndex);
    void         AddNewTab(UIFormConfigInfo* info);

private:
    Ui::UI_Widget_CommonPhrasesCard_PanelClass ui;

    void    Telescopic_Show(bool extending, bool isSaveParam = false);
    void    Expand_Show(bool expand);
    bool    m_IsExtending;
    bool    m_isPersonalExpand;
    QString m_removeId;
    int     m_scroll_pos;
    void*   m_currChat;

    JsCommonfilesHelper*        m_jshelper;
    UI_WebView_ShieldBackSpace* m_webView;

    QList<SwitchButton*>        m_extSwitchButton;
    int                         m_currTab;
};
