#pragma once

#include "ui/Common/UI_Servicestopmsg.h"
#include "ui_UIYL_TopMenuFunPanel.h"
#include <QObject>

class UIYL_TopMenuFunPanel : public QWidget
{
    Q_OBJECT
    friend class UIYL_SysConfigWidget;

public:
    UIYL_TopMenuFunPanel(QWidget *parent = NULL);
    ~UIYL_TopMenuFunPanel();

signals:
    void S_InitTopMenu();
private slots:
    void R_InitTopMenu();
    void R_personinfo_Clicked();
    void R_otherMenu_Clicked();

public:
    void Init();
    void InitTopMenu();

private:
    Ui::UIYL_TopMenuFunPanel ui;
    int                      m_initMenu;
};
