#pragma once

#include "utils/EnumDefine.h"
#include "ui/common/UI_WebView_ShieldBackSpace.h"
#include "js/YLJS_LemonLovesBeauty_RightPanel_Helper.h"
#include "ui_UI_Panel_LemonLovesBeauty.h"
#include <QResizeEvent>
#include <QWidget>

class UI_Panel_LemonLovesBeauty : public QWidget
{
    Q_OBJECT

public:
    UI_Panel_LemonLovesBeauty(QWidget* parent = 0);
    ~UI_Panel_LemonLovesBeauty();

public:
    void InitData();

public slots:
    void R_ProjectLinks_Checked(QString databody, QString links);
    void R_LoginStateChanged(ELOGINSTATUS oldStatus, ELOGINSTATUS newStatus);
    void R_GetViewEnv();

private:
    void resizeEvent(QResizeEvent* event);

private:
    Ui::UI_Panel_LemonLovesBeauty            ui;
    UI_WebView_ShieldBackSpace*              m_webView;
    YLJS_LemonLovesBeauty_RightPanel_Helper* m_lemonLovesBeautyJsObj;
};
