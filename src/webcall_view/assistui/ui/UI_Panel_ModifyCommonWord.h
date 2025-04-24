#pragma once

#include "utils/EnumDefine.h"
#include "UIObserverImp.h"
#include "data\YLData\YL_TreeViewItemLeafNodeData_CommonWord.h"
#include "data\YLData\YL_TreeViewItem_CommonWord.h"
#include "ui_UI_Panel_ModifyCommonWord.h"
#include "data/UICommonWordInfo.h"
#include <QWidget>

class UI_Panel_ModifyCommonWord : public QWidget, public IUIObserverImp
{
    Q_OBJECT

public:
    UI_Panel_ModifyCommonWord(QWidget *parent = 0);
    ~UI_Panel_ModifyCommonWord();

    void Init(YL_TreeViewItem_CommonWord *item);
    void ShowToastMsg(QString msg, EYLUI_ToastType type, int showTime, EYLUI_ToastShowPos showPos, QPoint tarPos);

signals:
    void S_ModifyCommonWord(YL_TreeViewItem_CommonWord *item, UICommonWordInfo *info);
    void S_ShowTipMessage(QString msg);

private slots:
    void R_OnModifyBtnClick();
    void R_OnRetrySettingBtnClick();
    void R_OnDeleteHotKey();
    void R_OnAddHotKey(QString key);
    void R_GetInputValue(QString str, int i);

private:
    Ui::UI_Panel_ModifyCommonWord ui;
    UICommonWordInfo             *m_dataInfo;
    YL_TreeViewItem_CommonWord   *m_pItem;
    QString                       title;
};
