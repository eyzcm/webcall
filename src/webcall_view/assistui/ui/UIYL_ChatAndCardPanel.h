#pragma once

#include "ui_UIYL_ChatAndCardPanel.h"
#include <QObject>

class UIYL_ChatAndCardPanel : public QWidget
{
    Q_OBJECT

    friend class UIYL_SysConfigWidget;

public:
    UIYL_ChatAndCardPanel(QWidget *parent = NULL);
    ~UIYL_ChatAndCardPanel();

    bool CheckData();
    bool SaveData();

private:
    Ui::UIYL_ChatAndCardPanel ui;
    void                      Init();
};
