#include "UIYL_ChatAndCardPanel.h"
#include "utils/ConstStringHelper.h"
#include "control/geasyliaoframework.h"
#include "service/DbUiWebcallService.h"

UIYL_ChatAndCardPanel::UIYL_ChatAndCardPanel(QWidget* parent) : QWidget(parent)
{
    ui.setupUi(this);
    Init();
}

UIYL_ChatAndCardPanel::~UIYL_ChatAndCardPanel()
{
}

void UIYL_ChatAndCardPanel::Init()
{
    CommInfo* info = gUIPersonalConfigManager->GetUserOtherSetting();
    if (info)
    {
        //对话自动清除
        QString key    = "";
        bool    hasKey = info->HasKey(ConstStringHelper::m_cChatAutoClear_Second);
        if (hasKey)
        {
            key = info->GetValueByKey(ConstStringHelper::m_cChatAutoClear_Second);
        }
        else
        {
            key = info->GetValueByKey(ConstStringHelper::m_cChatAutoClear);
        }

        if ((key != "") && (key != "0"))
        {
            int second = 120;
            if (hasKey)
            {
                second = key.toInt();
            }
            else
            {
                second = key.toInt() * 60;
            }
            ui.m_cleatChatTime->setValue(second);
            ui.m_autoClearChatCheck->setChecked(true);
        }
        else
        {
            int second = 120;

            ui.m_cleatChatTime->setValue(second);
            ui.m_autoClearChatCheck->setChecked(false);
        }

        //同时抓取
        if (info->GetValueByKey(ConstStringHelper::m_cSameTime) == "1")
        {
            ui.m_sameTimeCheck->setChecked(true);
        }
        else
        {
            ui.m_sameTimeCheck->setChecked(false);
        }

        //自动抓取手机号码
        if (info->GetValueByKey(ConstStringHelper::m_cAutoMobile) == "1")
        {
            ui.m_autoGrabPhoneNumCheck->setChecked(true);
        }
        else
        {
            ui.m_autoGrabPhoneNumCheck->setChecked(false);
        }

        //自动抓取电话号码
        if (info->GetValueByKey(ConstStringHelper::m_cAutoTelephone) == "1")
        {
            ui.m_autoGrabTelePhoneNumCheck->setChecked(true);
        }
        else
        {
            ui.m_autoGrabTelePhoneNumCheck->setChecked(false);
        }

        //自动抓取QQ
        if (info->GetValueByKey(ConstStringHelper::m_cAutoQQ) == "1")
        {
            ui.m_autoGrabQQCheck->setChecked(true);
        }
        else
        {
            ui.m_autoGrabQQCheck->setChecked(false);
        }
        //自动抓取email
        if (info->GetValueByKey(ConstStringHelper::m_cAutoEmail) == "1")
        {
            ui.m_autoGrabEmailCheck->setChecked(true);
        }
        else
        {
            ui.m_autoGrabEmailCheck->setChecked(false);
        }

        //自动抓取email
        if (info->GetValueByKey(ConstStringHelper::m_cAutoEmail) == "1")
        {
            ui.m_autoGrabEmailCheck->setChecked(true);
        }
        else
        {
            ui.m_autoGrabEmailCheck->setChecked(false);
        }


        //自动抓取微信
        if (info->GetValueByKey(ConstStringHelper::m_cAutoWX) == "1")
        {
            ui.m_autoGrabWXCheck->setChecked(true);
        }
        else
        {
            ui.m_autoGrabWXCheck->setChecked(false);
        }

        //自动抓取地区
        if (info->GetValueByKey(ConstStringHelper::m_cAutoMobileArea) == "1")
        {
            ui.m_autoGrabMobileAreaCheck->setChecked(true);
        }
        else
        {
            ui.m_autoGrabMobileAreaCheck->setChecked(false);
        }

        //自动抓取地区
        if (info->GetValueByKey(ConstStringHelper::m_cAutoSaveInfo) == "1")
        {
            ui.m_autoSaveInfoCheck->setChecked(true);
        }
        else
        {
            ui.m_autoSaveInfoCheck->setChecked(false);
        }

        //新建名片
        if (info->GetValueByKey("newcard") == "1")
        {
            ui.m_promptCreateNewCardCheck->setChecked(true);
        }
        else
        {
            ui.m_promptCreateNewCardCheck->setChecked(false);
        }
    }
}
