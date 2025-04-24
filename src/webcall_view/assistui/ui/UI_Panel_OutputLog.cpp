#include "UI_Panel_OutputLog.h"

UI_Panel_OutputLog::UI_Panel_OutputLog(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);
    this->setAttribute(Qt::WA_DeleteOnClose, true);
}

UI_Panel_OutputLog::~UI_Panel_OutputLog()
{
}

void UI_Panel_OutputLog::AddText(QString text)
{

    ui.textEdit->append(text);
}

void UI_Panel_OutputLog::ClearText()
{

    ui.textEdit->clear();
}
