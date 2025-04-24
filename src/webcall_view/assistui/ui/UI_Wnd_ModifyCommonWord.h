#pragma once

#include "data/YLData/YL_TreeViewItem_CommonWord.h"
#include "ui_UI_Wnd_ModifyCommonWord.h"
#include <QDialog>
#include <QWidget>

class UI_Wnd_ModifyCommonWord : public QDialog
{
    Q_OBJECT

public:
    UI_Wnd_ModifyCommonWord(YL_TreeViewItem_CommonWord* itemObj, QWidget* parent = 0);
    ~UI_Wnd_ModifyCommonWord();

private:
    Ui::UI_Wnd_ModifyCommonWord ui;
    YL_TreeViewItem_CommonWord* m_pItemObj;
};
