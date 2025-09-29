#include "IResponse.h"


IResponse::IResponse(void)
{
}


IResponse::~IResponse(void)
{
}

void IResponse::Execute( QString data)
{
	STDPRINTF(data.toUtf8().data());
	QString info = QString("IResponse::Execute type :%1\r\nbody:\r\n%2\r\n").arg(objectName()).arg(data);
}
