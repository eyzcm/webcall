#include "DbUiBaseService.h"

DbUiBaseService::DbUiBaseService(void)
{
    m_maindb = nullptr;
}

DbUiBaseService::~DbUiBaseService(void)
{
}

void DbUiBaseService::MainDbService(DbUiService *servcie)
{
    m_maindb = servcie;
}

DbUiService *DbUiBaseService::MainDbService()
{
    return m_maindb;
}

QString      DbUiBaseService::ServiceId()
{
    return "";
}
