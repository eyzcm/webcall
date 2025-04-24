#pragma once

#include "utils/EnumDefine.h"
#include "ui/common/UI_WebView_ShieldBackSpace.h"
#include "js/JsPanelAIHelper.h"
#include "ui_UI_Pane_AI_SelfBuild.h"
#include <QResizeEvent>
#include <QWidget>

class UI_Pane_AI_SelfBuild : public QWidget
{
    Q_OBJECT

public:
    UI_Pane_AI_SelfBuild(QWidget *parent = 0);
    ~UI_Pane_AI_SelfBuild();
    void InitData(QString val);

private:
    void                        resizeEvent(QResizeEvent *event);
    Ui::UI_Pane_AI_SelfBuild    ui;
    UI_WebView_ShieldBackSpace *m_webView;
    JsPanelAIHelper            *m_jsobj;
    QString                     m_url;
};
