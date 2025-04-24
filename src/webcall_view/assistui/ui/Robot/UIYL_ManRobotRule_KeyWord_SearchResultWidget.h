#pragma once

#include "ui_UIYL_ManRobotRule_KeyWord_SearchResultWidget.h"
#include <QWidget>

class UIYL_ManRobotRule_KeyWord_SearchResultWidget : public QWidget
{
    Q_OBJECT

public:
    UIYL_ManRobotRule_KeyWord_SearchResultWidget(QWidget *parent = nullptr);
    ~UIYL_ManRobotRule_KeyWord_SearchResultWidget();

    void Init();
    void SearchText(QString text);

private slots:
    void R_SetSearchResultNull(bool result);

private:
    Ui::UIYL_ManRobotRule_KeyWord_SearchResultWidget ui;
};
