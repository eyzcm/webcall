#pragma once

#include "ui_UIYL_KeywordPromptSettingPanel.h"
#include <QObject>

class UIYL_KeywordPromptSettingPanel : public QWidget
{
    Q_OBJECT
    friend class UIYL_KeywordPromptSettingPanel;

public:
    UIYL_KeywordPromptSettingPanel(QWidget *parent = NULL);
    ~UIYL_KeywordPromptSettingPanel();

public:
    Ui::UIYL_KeywordPromptSettingPanel ui;
};
