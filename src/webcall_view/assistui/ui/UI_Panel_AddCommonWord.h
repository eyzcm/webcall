#pragma once

#include "utils/EnumDefine.h"
#include "UIObserverImp.h"
#include "data\YLData\YL_TreeViewItem_CommonWord.h"
#include "ui_UI_Panel_AddCommonWord.h"
#include "data/UICommonWordInfo.h"
#include <QWidget>

class UI_Panel_AddCommonWord : public QWidget, public IUIObserverImp
{
    Q_OBJECT

public:
    UI_Panel_AddCommonWord(QWidget *parent = 0);
    ~UI_Panel_AddCommonWord();

public:
    void SetParentItem(YL_TreeViewItem_CommonWord *itemObj);
    void ShowToastMsg(QString msg, EYLUI_ToastType type, int showTime, EYLUI_ToastShowPos showPos, QPoint tarPos);

signals:
    void S_AddCommonWord(YL_TreeViewItem_CommonWord *parentItem, UICommonWordInfo *info);
    void S_ShowTipMessage(QString msg);

private slots:
    void R_OnAddCommonWordBtnClick();
    void R_OnRetrySettingBtnClick();
    void R_OnDeleteHotKey();
    void R_OnAddHotKey(QString key);
    void R_GetInputValue(QString str, int i);

private:
    Ui::UI_Panel_AddCommonWord  ui;
    QString                     m_id;
    QString                     m_customerID;
    YL_TreeViewItem_CommonWord *m_pItemObj;
    QString                     title;
};
