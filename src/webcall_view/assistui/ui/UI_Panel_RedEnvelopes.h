#pragma once

#include "utils/EnumDefine.h"
#include "ui/common/UI_WebView_ShieldBackSpace.h"

#include "ui_UI_Panel_RedEnvelopes.h"
#include <QResizeEvent>
#include <QWidget>
#include <js/YLJS_LemonLovesBeauty_EditPanel_Helper.h>


class YLJS_LemonLovesBeauty_EditPanel_Helper;
class UI_Panel_RedEnvelopes : public QWidget
{
    Q_OBJECT

public:
    UI_Panel_RedEnvelopes(QWidget* parent = 0);
    ~UI_Panel_RedEnvelopes();

public:
    void InitData();

public slots:
    void R_RedEnvelopesLinks_Checked(QString databody, QString links);

    void R_LoginStateChanged(ELOGINSTATUS oldStatus, ELOGINSTATUS newStatus);

private:
    void resizeEvent(QResizeEvent* event);

private:
    Ui::UI_Panel_RedEnvelopes               ui;
    UI_WebView_ShieldBackSpace*             m_webView;
    YLJS_LemonLovesBeauty_EditPanel_Helper* m_lemonLovesBeautyJsObj;
};
