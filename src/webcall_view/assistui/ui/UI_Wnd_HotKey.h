#pragma once

#include "ui_UI_Wnd_HotKey.h"
#include <QDialog>

class UI_Wnd_HotKey : public QDialog
{
    Q_OBJECT

public:
    UI_Wnd_HotKey(QWidget *parent = 0);
    ~UI_Wnd_HotKey();

signals:
    void S_AddHotKey(QString key);

private:
    Ui::UI_Wnd_HotKey ui;
};
