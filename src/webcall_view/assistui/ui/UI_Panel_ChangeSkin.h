#pragma once

#include "utils/EnumDefine.h"
#include "UI_Btn_Skin.h"
#include "switchbutton.h"
#include "ui_UI_Panel_ChangeSkin.h"
#include "data/UISkinInfo.h"
#include <QMouseEvent>
#include <QPushButton>
#include <QScrollArea>
#include <QTextCharFormat>
#include <QVBoxLayout>
#include <QWidget>

class UI_Panel_ChangeSkin : public QWidget
{
    Q_OBJECT

public:
    UI_Panel_ChangeSkin(QWidget *parent = 0);
    ~UI_Panel_ChangeSkin();

public:
    void Init();
    void GetCustomStructByDb(QMap<QString, UISkinInfo *> &mapSkin);
    void CreateChangeBtns();
    void CreateStackedWidget(SwitchButton *btn);
    void CreateSkinBtns(QGridLayout *btnLayout, int row, int col, UISkinInfo *info);
    void InitSwitchButton(SwitchButton *btn, bool isCheck);
    void SetDefaultSkin(QString strName);

private slots:
    void R_ChangeBtn_Clicked();
    void R_SkinBtn_Clicked();
    void R_ResetCurrentSkin();

    void R_LoginStateChanged(ELOGINSTATUS oldStatus, ELOGINSTATUS newStatus);

signals:
    void S_SkinBtn_Clicked(QString fullPath);

private:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private:
    Ui::UI_Panel_ChangeSkin                ui;
    QMap<int, QMap<QString, UISkinInfo *>> m_mapSkinInfo;
    QMap<int, QString>                     m_mapGroupName;
    QMap<QString, QVector<UISkinInfo *>>   m_mapSkinInfoByGroupName;
    QMap<SwitchButton *, QWidget *>        m_mapChangeBtnBindWidget;
    QMap<QString, UI_Btn_Skin *>           m_mapSkinBtnByImgName;
    UISkinInfo                            *m_pCurSkinInfo;
    UI_Btn_Skin                           *m_CurBtn;
};
