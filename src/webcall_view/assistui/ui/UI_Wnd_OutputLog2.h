#pragma once

#include "utils/UISingleton.h"
#include "ui_UI_Wnd_OutputLog2.h"
#include <QWidget>

class UI_Wnd_OutputLog2 : public QWidget, public UISingleton<UI_Wnd_OutputLog2>
{
    Q_OBJECT

public:
    UI_Wnd_OutputLog2(QWidget *parent = 0);
    ~UI_Wnd_OutputLog2();

protected:
    virtual void closeEvent(QCloseEvent *e);

public:
    void AddText(QString text);
    void ClearText();
    void ShowType(bool isShow);

private slots:
    void R_showText(QString str);

private:
    int                  m_count;
    Ui::UI_Wnd_OutputLog2 ui;
    QString              m_cacheText;
    bool                 m_isDisp;
    QString              m_filterstr;
    int                  m_maxLogCacheCount;
    int                  m_maxLogTextLen;
    int                  m_initLoad;
};
