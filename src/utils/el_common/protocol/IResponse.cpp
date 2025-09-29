#include "IResponse.h"

IResponse::IResponse(void)
{
}

IResponse::~IResponse(void)
{
}

void IResponse::Execute(QString data)
{
    STDPRINTF(data.toUtf8().data());
}

void IResponse::staticExecte(const QString &data)
{
    STDPRINTF(data.toUtf8().data());
}
