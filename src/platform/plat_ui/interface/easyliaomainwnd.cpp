#include "easyliaomainwnd.h"

EasyLiaoMainWnd::EasyLiaoMainWnd(QWidget *parent)
    : QWidget(parent)
{
    ui = new Ui::EasyLiaoMainWnd;
    ui->setupUi(this);
}

EasyLiaoMainWnd::~EasyLiaoMainWnd()
{
}
