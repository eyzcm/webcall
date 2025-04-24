#pragma once

#include "ui_UI_Panel_OutputLog.h"
#include <QWidget>

class UI_Panel_OutputLog : public QWidget
{
    Q_OBJECT

public:
    UI_Panel_OutputLog(QWidget *parent = 0);
    ~UI_Panel_OutputLog();

public:
    void AddText(QString text);
    void ClearText();

private:
    Ui::UI_Panel_OutputLog ui;
};
