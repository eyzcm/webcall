#include "VisitorEvaluationCmd.h"

VisitorEvaluationCmd::VisitorEvaluationCmd(void) : TcpCmd()
{
}

VisitorEvaluationCmd::~VisitorEvaluationCmd(void)
{
}

void VisitorEvaluationCmd::MakeCmd()
{
    Body("<OPINION>");
    QString strUriFromat = "/Message?type=eventMessage&chatId=%1";
    QString strUri       = strUriFromat.arg(ChatId());
    Uri(strUri);
}
