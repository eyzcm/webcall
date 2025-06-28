#ifndef UIMESSAGETIP_H
#define UIMESSAGETIP_H


#include <QTimer>
#include <QWidget>
#include "plat_view_global.h"

QT_BEGIN_NAMESPACE
namespace Ui { class UIMessageTip; }
QT_END_NAMESPACE


class PLAT_VIEW_EXPORT UIMessageTip : public QWidget
{
    Q_OBJECT

public:
    UIMessageTip(QWidget* parent = 0);
    ~UIMessageTip();
    void SetContentTip(QString tip);
    void SetTimerInterval(int val);
signals:
    void S_Closed();
private slots:
    void R_OKClicked();
    void R_OnTimeOut();

private:
    Ui::UIMessageTip * ui;

    QTimer* m_timer;
};

#endif // UIMESSAGETIP_H
