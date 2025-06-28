#ifndef UIYL_UserCheckPanel_H
#define UIYL_UserCheckPanel_H

#include "ui/Login_UI/UI_WebView_Login.h"
#include "ui/Login_UI/JsUserCheckHelper.h"

#include "ui_UIYL_UserCheckPanel.h"
#include <QDialog>

class UIYL_UserCheckPanel : public QWidget
{
    Q_OBJECT

public:
    UIYL_UserCheckPanel(QWidget *parent = 0);
    ~UIYL_UserCheckPanel();
    void         InitData();
    void         Init(int type, QString url, int isPwdSafed);
    virtual void closeEvent(QCloseEvent *);
    int          CheckModify();

protected:
    virtual void resizeEvent(QResizeEvent *e);

signals:
    void S_closeWnd();

public slots:
    void R_closeWnd();
    void R_quitSys();

private:
    Ui::UIYL_UserCheckPanel     ui;
	UI_WebView_Login *m_webView;
    JsUserCheckHelper          *m_userCheckJsObj;
    int                         m_isCheck;
    QString                     m_checkUrl;
    int                         m_isPwdSafed;
};

#endif // UIYL_UserCheckPanel_H
