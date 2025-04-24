#pragma once

#include "ui_UI_Wnd_AddPersonalQuickCommonWord.h"
#include <QDialog>
#include <QWidget>

class UI_Wnd_AddPersonalQuickCommonWord : public QDialog
{
    Q_OBJECT

public:
    UI_Wnd_AddPersonalQuickCommonWord(QWidget *parent = 0);
    ~UI_Wnd_AddPersonalQuickCommonWord();

public:
    bool Modify() const;
    void Modify(bool val);

    void SetPersonalQuickItemData(MPersonalQuickCommonWordData *data);
private slots:
    void R_OnClose();

private:
    Ui::UI_Wnd_AddPersonalQuickCommonWord ui;
    bool                                  m_isModify;
};
