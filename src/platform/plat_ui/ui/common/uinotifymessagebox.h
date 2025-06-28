#ifndef UINOTIFYMESSAGEBOX_H
#define UINOTIFYMESSAGEBOX_H


#include <QTimer>
#include <QWidget>
#include "plat_ui_global.h"

QT_BEGIN_NAMESPACE
namespace Ui { class UINotifyMessageBox; }
QT_END_NAMESPACE




class PLAT_UI_EXPORT UINotifyMessageBox : public QWidget
{
    Q_OBJECT

public:
    UINotifyMessageBox(QWidget* parent = 0,QString token=0);
    ~UINotifyMessageBox();

    void    SetContentTip(QString tip);
    void    SetLinkBtnText(QString txt);
    void    SetLinkUrl(const QString& url);
    void    SetCategory(const QString& category);
    void    SetUrlType(bool isCrm = false);
    void    IsCloseEnable(bool iscloseEnable);
    QString GetCategory();

signals:
    void S_Closed();
    void S_GotoUrlBegin(QString category);
    void S_OpenCrm(QString url);

private slots:
    void R_OKClicked();
    void R_OnTimeOut();

private:
    Ui::UINotifyMessageBox * ui;
    QString                m_url;
    QString                m_category;
    bool                   m_isCloseEnable;
    bool                   m_isCrm_UrlType;
    QTimer*                m_timer;

};
#endif // UINOTIFYMESSAGEBOX_H
