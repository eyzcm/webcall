#include "MyIconStyle.h"

MyIconStyle::MyIconStyle()
{
}
MyIconStyle::~MyIconStyle()
{
}

int MyIconStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    int s = QProxyStyle::pixelMetric(metric, option, widget);
    if (metric == QStyle::PM_SmallIconSize)
    {
        s = 50;
    }
    return s;
}
