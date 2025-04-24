#pragma once

#include "utils/EnumDefine.h"
#include "ui/common/UI_WebView_ShieldBackSpace.h"
#include "js/JsPanelAIHelper.h"
#include "ui_UI_Pane_AI_Modify.h"
#include <QResizeEvent>
#include <QWidget>

class UI_Pane_AI_Modify : public QWidget
{
    Q_OBJECT

public:
    UI_Pane_AI_Modify(QWidget *parent = 0);
    ~UI_Pane_AI_Modify();
    void InitData(QString val);

private:
    void                        resizeEvent(QResizeEvent *event);
    Ui::UI_Pane_AI_Modify       ui;
    UI_WebView_ShieldBackSpace *m_webView;
    JsPanelAIHelper            *m_jsobj;
    QString                     m_url;
};
