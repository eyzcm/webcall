#ifndef UIForceLogoutBox_H
#define UIForceLogoutBox_H

#include "ui_UIForceLogoutBox.h"
#include <QDialog>

class UIForceLogoutBox : public QWidget
{
    Q_OBJECT

public:
    UIForceLogoutBox(QWidget *parent = 0);
    ~UIForceLogoutBox();

    void closeEvent(QCloseEvent *e);
    void hideEvent(QHideEvent *e);

public:
    void SetMsgText(QString text);

signals:
    void S_ResultOK();
    void S_Closed();
    void S_HideWndAndDelCenterWidget();

private slots:
    void R_OKClicked();

private:
    Ui::UIForceLogoutBox ui;
};

#endif // UIForceLogoutBox_H
