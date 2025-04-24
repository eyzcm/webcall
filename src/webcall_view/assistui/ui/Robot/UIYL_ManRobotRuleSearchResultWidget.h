#ifndef UIYL_ManRobotRuleSearchResultWidget_H
#define UIYL_ManRobotRuleSearchResultWidget_H

#include "ui_UIYL_ManRobotRuleSearchResultWidget.h"
#include <QWidget>

class UIYL_ManRobotRuleSearchResultWidget : public QWidget
{
    Q_OBJECT

public:
    UIYL_ManRobotRuleSearchResultWidget(QWidget *parent = 0);
    ~UIYL_ManRobotRuleSearchResultWidget();

    void Init();
    void SearchText(QString text);

private slots:
    void R_SetSearchResultNull(bool result);

private:
    Ui::UIYL_ManRobotRuleSearchResultWidget ui;
};

#endif // UIYL_ManRobotRuleSearchResultWidget_H
