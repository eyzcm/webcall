#pragma once
#include <QProxyStyle>
#include <QStyle>
#include <QStyleOption>
#include <QWidget>
#include "plat_ui_global.h"

class PLAT_UI_EXPORT MyIconStyle : public QProxyStyle
{
    Q_OBJECT

public:
    MyIconStyle();
    virtual ~MyIconStyle();
    virtual int pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const;
};
