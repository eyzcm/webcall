#include "utils/UICommUtilsHelper.h"
#include <QApplication>
#include <QDesktopWidget>


#include "ui/baseWnd.h"
#include "ui/uimessagebox.h"
#include "ui/uimessagetip.h"

UICommUtilsHelper::UICommUtilsHelper(QObject* parent) : QObject(parent)
{
}

UICommUtilsHelper::~UICommUtilsHelper()
{
}


int UICommUtilsHelper::ShowCommonBox(const QString& title, const QString& text)
{
    UIMessageBox* wnd = new UIMessageBox;
    wnd->setWindowTitle(title);
    wnd->SetContentTip(text);
    return wnd->exec();
}

int UICommUtilsHelper::ShowCommonBox(const QString& title, const QString& text, const QString& ok, const QString& can)
{
    UIMessageBox* wnd = new UIMessageBox;
    wnd->setWindowTitle(title);
    wnd->SetContentTip(text);
    wnd->SetOkBtnText(ok);
    wnd->SetCancelBtnText(can);
    wnd->SetCancelBtnVisible(true);
    return wnd->exec();
}


void UICommUtilsHelper::ShowCommonBoxToRBottom(const QString& title, const QString& text)
{
    UIMessageTip* wnd = new UIMessageTip;
    wnd->SetContentTip(text);

    BaseWnd* pBaseWnd = new BaseWnd;
    pBaseWnd->AddContentWidget(wnd);
    pBaseWnd->HideMaxBtn();
    pBaseWnd->HideMinBtn();
    pBaseWnd->SetWndTitle(title);
    pBaseWnd->DoSetWindowTitle(title);

    QRect rect = QApplication::desktop()->availableGeometry();
    QRect curRect;
    curRect.setX(rect.width() - 285);
    curRect.setY(rect.height() - 160);
    curRect.setWidth(280);
    curRect.setHeight(120);

    pBaseWnd->setGeometry(curRect);
    pBaseWnd->show();
}

void UICommUtilsHelper::ShowCommonBoxToRBottomByTime(const QString& title, const QString& text, int nVal)
{
    UIMessageTip* wnd = new UIMessageTip;
    wnd->SetContentTip(text);
    wnd->SetTimerInterval(nVal);

    BaseWnd* pBaseWnd = new BaseWnd;
    pBaseWnd->AddContentWidget(wnd);
    pBaseWnd->HideMaxBtn();
    pBaseWnd->HideMinBtn();
    pBaseWnd->SetWndTitle(title);
    pBaseWnd->DoSetWindowTitle(title);

    QRect rect = QApplication::desktop()->availableGeometry();
    QRect curRect;
    curRect.setX(rect.width() - 285);
    curRect.setY(rect.height() - 160);
    curRect.setWidth(280);
    curRect.setHeight(120);

    pBaseWnd->setGeometry(curRect);
    pBaseWnd->show();
}

int UICommUtilsHelper::CheckSysNoReply(int noReplay, int noSysReplay, QString msgtype)
{
    if (msgtype == "system_message_welcome")
        return 0;

    if (msgtype == "eventCustom")
        return 0;

    if (noReplay == 1 && msgtype != "system_message")
        return 1;

    if (noSysReplay == 1 && (msgtype == "system_message") || (msgtype == ""))
        return 1;

    return 0;
}

QString UICommUtilsHelper::QColorToQString(QColor color, int apl)
{
    QString strR     = QString::number(color.red());
    QString strG     = QString::number(color.green());
    QString strB     = QString::number(color.blue());
    QString strA     = QString::number(apl);
    QString strColor = strR + "," + strG + "," + strB + "," + strA;

    return strColor;
}
