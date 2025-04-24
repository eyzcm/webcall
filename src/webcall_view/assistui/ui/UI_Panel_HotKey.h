#pragma once

#include "UIObserverImp.h"
#include "ui_UI_Panel_HotKey.h"
#include <QKeyEvent>
#include <QWidget>

class UI_Panel_HotKey : public QWidget, public IUIObserverImp
{
    Q_OBJECT

public:
    UI_Panel_HotKey(QWidget *parent = 0);
    ~UI_Panel_HotKey();

public:
    void InitHotkey(QString key);

signals:
    void S_AddHotKey(QString key);

private slots:
    void R_OnCancelBtnClick();
    void R_OnDeleteBtnClick();

private:
    bool Is_HotkeyAlreadyExists(QString key);

    bool eventFilter(QObject *obj, QEvent *e);

private:
    Ui::UI_Panel_HotKey ui;
    bool                m_HasHotKey;
};
