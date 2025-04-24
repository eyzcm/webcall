#ifndef UIYL_CommonWordTreeSearchResultWidget_H
#define UIYL_CommonWordTreeSearchResultWidget_H

#include "ui_UIYL_CommonWordTreeSearchResultWidget.h"
#include <QWidget>

class UIYL_CommonWordTreeSearchResultWidget : public QWidget
{
    Q_OBJECT

public:
    UIYL_CommonWordTreeSearchResultWidget(QWidget *parent = 0);
    ~UIYL_CommonWordTreeSearchResultWidget();

    void Init();
    void SearchText(QString text);

private slots:
    void R_SetSearchResultNull(bool result);

private:
    Ui::UIYL_CommonWordTreeSearchResultWidget ui;
};

#endif // UIYL_CommonWordTreeSearchResultWidget_H
