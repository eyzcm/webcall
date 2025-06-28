#ifndef UIMESSAGEBOX_H
#define UIMESSAGEBOX_H

#include "MyWidgetBase.h"
#include <QDialog>
#include "plat_ui_global.h"

QT_BEGIN_NAMESPACE
namespace Ui { class UIMessageBox; }
QT_END_NAMESPACE


class PLAT_UI_EXPORT UIMessageBox : public MyWidgetBase
{
    Q_OBJECT

public:
    UIMessageBox(QWidget *parent = 0, int mode = 0);
    ~UIMessageBox();
    void closeEvent(QCloseEvent *event);
    int  exec();
    void done(int i);

public:
    void SetContentTip(QString tip);
    void SetCancelBtnVisible(bool isShow);

    void SetOkBtnText(QString text);
    void SetCancelBtnText(QString text);

signals:
    void S_ResultOK(bool isOk);
    void S_Closed();

private slots:
    void R_OKClicked();
    void R_CancelClicked();
    void R_Closed();

private:
    Ui::UIMessageBox * ui;
    int              reInt;
    QEventLoop      *m_eventLoop;
};

#endif // UIMESSAGEBOX_H
