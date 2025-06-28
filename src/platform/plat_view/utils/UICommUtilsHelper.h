#ifndef UICOMMUTILSHELPER_H
#define UICOMMUTILSHELPER_H

#include "utils/Plat_EnumDefine.h"
#include "plat_view_global.h"
#include <QDateTime>
#include <QObject>

class PLAT_VIEW_EXPORT UICommUtilsHelper : public QObject
{
    Q_OBJECT

public:
    UICommUtilsHelper(QObject* parent);
    ~UICommUtilsHelper();

public:
    static QString QColorToQString(QColor color, int apl);
    static int     CheckSysNoReply(int noReplay, int noSysReplay, QString msgtype);
    static int     ShowCommonBox(const QString& title, const QString& text);
    static int     ShowCommonBox(const QString& title, const QString& text, const QString& ok, const QString& can);

    static void    ShowCommonBoxToRBottom(const QString& title, const QString& text);
    static void    ShowCommonBoxToRBottomByTime(const QString& title, const QString& text, int nVal);
};

#endif // UICOMMUTILSHELPER_H
