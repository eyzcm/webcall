#pragma once

#include "ui_UI_Panel_PersonalQuickCommonWord.h"
#include <QWidget>

class UI_Panel_PersonalQuickCommonWord : public QWidget
{
    Q_OBJECT

public:
    UI_Panel_PersonalQuickCommonWord(QWidget *parent = 0);
    ~UI_Panel_PersonalQuickCommonWord();

public:
    void Init();

    //////////////////////////////////////////////////////////////////////////
    // CZC:显隐常用语名片面板
signals:
    void S_ShowBtn_Collapse(bool show);

private:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    //////////////////////////////////////////////////////////////////////////

private:
    Ui::UI_Panel_PersonalQuickCommonWord ui;
};
