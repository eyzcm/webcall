#pragma once

#include "ui/common/UI_WebView_ShieldBackSpace.h"

#include "ui_UI_Pane_NewTab.h"
#include <QResizeEvent>
#include <QTextEdit>
#include <QWidget>
#include <ui/js/JsPanelNewTabHelper.h>

class UI_Pane_NewTab : public QWidget
{
    Q_OBJECT

public:
    UI_Pane_NewTab(QWidget* parent = 0);
    ~UI_Pane_NewTab();
    void InitData();

public slots:
    void R_ShowSettingChanged();

private:
    void resizeEvent(QResizeEvent* event);

private slots:
    void R_OnUrlTimeOut();

private:
    Ui::UI_Pane_NewTab          ui;
    UI_WebView_ShieldBackSpace* m_webView;
    JsPanelNewTabHelper*        m_newTabObj;

    QTimer* m_urlTimer;
    QString m_url;
    bool    m_isModify;
};
