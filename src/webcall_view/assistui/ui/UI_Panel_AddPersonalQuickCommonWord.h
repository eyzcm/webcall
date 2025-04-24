#pragma once

#include "utils/EnumDefine.h"
#include "UIObserverImp.h"
#include "UI_Panel_BtnItemSetting.h"
#include "data/MPersonalQuickCommonWordData.h"
#include "ui_UI_Panel_AddPersonalQuickCommonWord.h"
#include <QWidget>

class UI_Panel_AddPersonalQuickCommonWord : public QWidget, public IUIObserverImp
{
    Q_OBJECT

public:
    UI_Panel_AddPersonalQuickCommonWord(QWidget* parent = 0);
    ~UI_Panel_AddPersonalQuickCommonWord();

public:
    void SetPersonalQuickItemData(MPersonalQuickCommonWordData* data);

    void ShowToastMsg(QString msg, EYLUI_ToastType type, int showTime, EYLUI_ToastShowPos showPos, QPoint tarPos);

    bool IsModify() const;
    void IsModify(bool val);

signals:
    void S_ShowTipMessage(QString msg);
    void S_AddPersonalQuickCommonWord(MPersonalQuickCommonWordData* item, bool isModify);
    void S_OnClose();
private slots:
    void R_OnClose();
    void R_OnAddCommonWordBtnClick();
    void R_OnRetrySettingBtnClick();
    void R_OnDeleteHotKey();
    void R_OnAddHotKey(QString key);
    void R_OnAddButtonItemSettingClick();
    void R_OnRemoveBtnClick(QString uuid);
    void R_GetInputValue(QString str, int i);

private:
    Ui::UI_Panel_AddPersonalQuickCommonWord ui;

    QMap<QString, UI_Panel_BtnItemSetting*> m_btnItemSettingList;
    QMap<int, QString>                      m_sortIdList;
    MPersonalQuickCommonWordData*           m_modifyItem;
    bool                                    isModify;
    int                                     m_nSortId;
    QString                                 title;
};
