#pragma once

#include "ui_UI_Panel_CardSavePrompt.h"
#include <QTimer>
#include <QWidget>

class UI_Panel_CardSavePrompt : public QWidget
{
    Q_OBJECT

    //对外接口
public:
    void SetPromptType(bool success = true);

public:
    UI_Panel_CardSavePrompt(QWidget* parent = 0);
    ~UI_Panel_CardSavePrompt();

    void show();
    void hide();

private slots:
    void R_Timout();

private:
    QTimer* m_timer;

    bool m_isHide;

private:
    Ui::UI_Panel_CardSavePrompt ui;
};
