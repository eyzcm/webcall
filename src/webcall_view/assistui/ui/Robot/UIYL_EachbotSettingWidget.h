#ifndef UIYL_EachbotSettingWidget_H
#define UIYL_EachbotSettingWidget_H

#include "utils/EnumDefine.h"
#include "ui_UIYL_EachbotSettingWidget.h"
#include <QWidget>

class UIYL_EachbotSettingWidget : public QWidget
{
    Q_OBJECT

public:
    UIYL_EachbotSettingWidget(QWidget *parent = 0);
    ~UIYL_EachbotSettingWidget();

private slots:
    void R_OnOkClick();
    void R_OnCancelClick();
    void R_OnDefaultClick();

signals:
    void S_Closed();
    void S_ShowToastMsg_AISettingCenter(QString msg, int showTime, EYLUI_ToastShowPos showPos, EYLUI_ToastType type, QPoint tarPos);

private:
    Ui::UIYL_EachbotSettingWidget ui;
};

#endif // UIYL_EachbotSettingWidget_H
