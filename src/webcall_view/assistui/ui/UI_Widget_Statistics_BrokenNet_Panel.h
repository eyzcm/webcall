#pragma once

#include "ui_UI_Widget_Statistics_BrokenNet_Panel.h"
#include <QWidget>

class UI_Widget_Statistics_BrokenNet_Panel : public QWidget
{
    Q_OBJECT

public:
    UI_Widget_Statistics_BrokenNet_Panel(QWidget *parent = 0);
    ~UI_Widget_Statistics_BrokenNet_Panel();
    void SetErrorFlag(int flag);

private:
    Ui::UI_Widget_Statistics_BrokenNet_Panel ui;
};
