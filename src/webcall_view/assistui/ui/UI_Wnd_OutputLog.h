#pragma once

#include "utils/UISingleton.h"
#include "ui_UI_Wnd_OutputLog.h"
#include <QWidget>

class UI_Wnd_OutputLog : public QWidget, public UISingleton<UI_Wnd_OutputLog>
{
    Q_OBJECT

public:
    UI_Wnd_OutputLog(QWidget *parent = 0);
    ~UI_Wnd_OutputLog();

protected:
    virtual void closeEvent(QCloseEvent *e);

public:
    void AddText(QString text);
    void ClearText();
    void ShowType(bool isShow);

private slots:
    void R_TcpReqLog(QString msgtype, QString body);
    void R_TcpResultLog(QString msgtype, QString body);
    void R_HttpReqLog(QString urlstr, QString body);
    void R_HttpResultFinishedLog(QString code, QString reason, QString body);
    void R_HttpAiResultFinishedLog(QString code, QString reason, QString body);

private:
    int                  m_count;
    Ui::UI_Wnd_OutputLog ui;
    QString              m_cacheText;
    bool                 m_isDisp;
    QString              m_filterstr;
    int                  m_maxLogCacheCount;
    int                  m_maxLogTextLen;
    int                  m_initLoad;
};
