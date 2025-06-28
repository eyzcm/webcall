#include "ColleagueService.h"
#include "utils/jsonhelper.h"
#include "session/el_sessionservice.h"
#include "utils/UtilsHelper.h"

colleagueService::colleagueService()
{

    m_maxUserInfoPage = 0;
    m_IsLoadUserInfoEnd = false;
}


void    colleagueService::SetDepartmentList(const QString &body)
{
    if (m_departmentList.size() > 0) return;
    qDeleteAll(m_departmentList);
    m_departmentList.clear();
    Json::Value val;
    bool        isOk = JsonHelper::GetJsonValueFromBody(body, val);
    if (isOk)
    {
        if (val["data"].isArray())
        {

            for (int i = 0; i < (int)val["data"].size(); i++)
            {
                PlatDepartmentInfo *info = new PlatDepartmentInfo();

				
					QMap<QString, QString> tempMap;
					bool                   isParse = JsonHelper::ParseValueToMap(val["data"][i], tempMap);
					if (isParse)
					{
						info->LoadMapInfo(tempMap);
                        m_departmentList.append(info);
					}
				
            }

        }
    }

}


void    colleagueService::SetUserInfoList(const QString &body,int pageno)
{
    
    if( pageno == 1)
    {
        qDeleteAll(m_userInfoList);
        m_userInfoList.clear();
        m_IsLoadUserInfoEnd = false;
    }

    Json::Value val;
    bool        isOk = JsonHelper::GetJsonValueFromBody(body, val);
    if (isOk)
    {
        if (val["data"].isObject())
        {

            
                
                    QMap<QString, QString> tempMap;
                    bool                   isParse = JsonHelper::ParseValueToMap(val["data"], tempMap);
                    if (isParse)
                    {
						Json::Value dataVal = val["data"];
                        m_maxUserInfoPage = tempMap["pages"].toInt();
						if (dataVal["records"].isArray())
						{
							Json::Value recordsVal = dataVal["records"];
							for (int i = 0; i < (int)dataVal["records"].size(); i++)
							{
								PlatUserInfo *info = new PlatUserInfo();
								QMap<QString, QString> itemMap;
								bool                   isParse = JsonHelper::ParseValueToMap(recordsVal[i], itemMap);
								info->LoadMapInfo(itemMap);
								m_userInfoList.append(info);
							}
						}


                    }
            
        }
    }
}

QList<PlatDepartmentInfo *> colleagueService::GetDepartmentList()
{
    return m_departmentList;
}


QList<PlatUserInfo *> colleagueService::GetUserInfoList()
{
    return m_userInfoList;
}
int                     colleagueService::GetMaxPage()
{
       return m_maxUserInfoPage;
}

bool  colleagueService::IsLoadUserInfoEnd()
{
    return m_IsLoadUserInfoEnd;
}

void  colleagueService::IsLoadUserInfoEnd(bool val)
{
    m_IsLoadUserInfoEnd = val;
    if(m_IsLoadUserInfoEnd  ) Q_EMIT S_LoadUserInfoEnd();
}

