#include "permissionservice.h"
#include "utils/jsonhelper.h"
#include "session/el_sessionservice.h"
#include "utils/UtilsHelper.h"

PermissionService::PermissionService()
{

}


void    PermissionService::SetFunMenuList(const QString &body)
{
	if (m_menuInfoList.size() > 0) return;
    qDeleteAll(m_menuInfoList);
    m_menuInfoList.clear();
    Json::Value val;
    bool        isOk = JsonHelper::GetJsonValueFromBody(body, val);
    if (isOk)
    {
        if (val["data"].isArray())
        {
            QString ismedical = gSessionService->GetSystemConfigByKey("ismedical", "6");
            ECLIENTTYPE retType = UtilsHelper::GetClientTypeByString(ismedical);

            if(  retType == E_CLIENT_WEB)
			{
                /*
                UIMenuDataInfo * info = new UIMenuDataInfo();
                info->Code("webcall_web");

                info->Name(QString::fromUtf8(QObject::tr("消息").toStdString().c_str()));
				info->Url("F:/testim/test.html");
                info->Opentype(2);
                m_menuInfoList.append(info);
                */
               /* {
                    UIMenuDataInfo * info = new UIMenuDataInfo();
                    info->Code("webcall_web");

                    info->Name(QString::fromUtf8(QObject::tr("消息").toStdString().c_str()));
                    info->Url("https://test-prd18.easyliao.net/webcall/#/chat?access_token=${access_token}");
                    info->Opentype(2);
                    m_menuInfoList.append(info);

                }
				*/


				
			

            }else
            {
                {
				UIMenuDataInfo * info = new UIMenuDataInfo();
				info->Code("webcall_online");

                info->Name(QString::fromUtf8(QObject::tr("消息").toStdString().c_str()));

				info->Opentype(1);
				m_menuInfoList.append(info);
                }



			}

			{
				UIMenuDataInfo * info = new UIMenuDataInfo();
				info->Code("webcall_test");

				info->Name(QString::fromUtf8(QObject::tr("测试页面").toStdString().c_str()));
                info->Url("https://sem.easyliao.com/Download/testhtml/test.html");
                //info->Url("https://test-prd18.easyliao.net/webcall//#/chat/chatPage?access_token=${access_token}");
				info->Opentype(2);
				m_menuInfoList.append(info);

			}

            for (int i = 0; i < (int)val["data"].size(); i++)
            {
                UIMenuDataInfo *info = new UIMenuDataInfo();

				
					QMap<QString, QString> tempMap;
					bool                   isParse = JsonHelper::ParseValueToMap(val["data"][i], tempMap);
					if (isParse)
					{
						info->LoadMapInfo(tempMap);
						if (info->SubName() == "plat-client-menu")
						{
							m_menuInfoList.append(info);
						}
					}
				
            }

            qSort(m_menuInfoList.begin(), m_menuInfoList.end(), PermissionService::CompareMenuData);
        }
    }

}

QList<UIMenuDataInfo *> PermissionService::GetFunMenuList()
{
	if (m_menuInfoList.size() == 0)
	{
		//默认加上在线
		QString ismedical = gSessionService->GetSystemConfigByKey("ismedical", "6");
		ECLIENTTYPE retType = UtilsHelper::GetClientTypeByString(ismedical);
		if (retType == E_CLIENT_WEB)
		{
			/*
			UIMenuDataInfo * info = new UIMenuDataInfo();
			info->Code("webcall_web");

			info->Name(QString::fromUtf8(QObject::tr("消息").toStdString().c_str()));
			info->Url("F:/testim/test.html");
			info->Opentype(2);
			m_menuInfoList.append(info);
			*/
			{
				UIMenuDataInfo * info = new UIMenuDataInfo();
				info->Code("webcall_web");

				info->Name(QString::fromUtf8(QObject::tr("消息").toStdString().c_str()));
				info->Url("https://test-prd18.easyliao.net/webcall/#/chat?access_token=${access_token}");
				info->Opentype(2);
				m_menuInfoList.append(info);

            }

        }else
		{
			UIMenuDataInfo * info = new UIMenuDataInfo();
			info->Code("webcall_online");

            info->Name(QString::fromUtf8(tr("消息").toStdString().c_str()));
			info->Opentype(1);
			m_menuInfoList.append(info);


		}

        {
            UIMenuDataInfo * info = new UIMenuDataInfo();
            info->Code("webcall_test");

            info->Name(QString::fromUtf8(QObject::tr("测试页面").toStdString().c_str()));
            info->Url("https://sem.easyliao.com/Download/testhtml/test.html");
            //info->Url("https://test-prd18.easyliao.net/webcall//#/chat/chatPage?access_token=${access_token}");
            info->Opentype(2);
            m_menuInfoList.append(info);

        }

		/*
		{
			UIMenuDataInfo * info = new UIMenuDataInfo();
			info->Code("platrd:client:main:menu");

			info->Name(QString::fromUtf8("客户端主菜单"));
			info->Opentype(2);
			info->Url("http://test-prd18.easyliao.net/eom-doc-web/index#/note${check}");
			m_menuInfoList.append(info);
		}
		*/



	}

    return m_menuInfoList;
}


bool PermissionService::CompareMenuData(const UIMenuDataInfo *info1, const UIMenuDataInfo *info2)
{
    if ((info1->ParentId() == 0) && (info1->ParentId() < info2->ParentId()))
    {
        return true;
    }
    else if (info1->ParentId() == info2->ParentId())
    {
        if (info1->Score() < info2->Score())
            return true;
    }
    return false;
}
