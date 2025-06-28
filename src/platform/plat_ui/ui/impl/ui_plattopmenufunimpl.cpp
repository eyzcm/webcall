#include "ui_plattopmenufunimpl.h"
#include "ui_ui_plattopmenufunimpl.h"

UI_PlatTopMenuFunImpl::UI_PlatTopMenuFunImpl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UI_PlatTopMenuFunImpl)
{
    ui->setupUi(this);
}

UI_PlatTopMenuFunImpl::~UI_PlatTopMenuFunImpl()
{
    delete ui;
}
