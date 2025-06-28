#include "UI_NoticeBubblePrompt.h"

UI_NoticeBubblePrompt::UI_NoticeBubblePrompt(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_ShowModal, true);
    ui.setupUi(this);
    connect(ui.btn_close, SIGNAL(clicked()), this, SLOT(R_BtnClose_Clicked()));
}

UI_NoticeBubblePrompt::~UI_NoticeBubblePrompt()
{
}

void UI_NoticeBubblePrompt::R_BtnClose_Clicked()
{
    this->hide();
}
