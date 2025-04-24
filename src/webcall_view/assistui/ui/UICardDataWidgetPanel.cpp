#include "UICardDataWidgetPanel.h"

#include "utils/DefineHelper.h"
#include "utils/EL_Util_Time.h"
#include "UICardItemData.h"
#include "UISearchCBox.h"
#include "control/CommonSignalService.h"
#include "control/IUIController.h"
#include "control/geasyliaoframework.h"
#include "data/CommInfo.h"
#include "data/MUICardInfoData.h"
#include "service/DbUiWebcallService.h"
#include "model/UICommonDataModel.h"
#include "protocol_biz/webcallprotocolservice.h"
#include "utils/utilshelper.h"
#include <QComboBox>
#include <QDateTimeEdit>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QList>
#include <QSpacerItem>
#include <QTextEdit>
#include "service/util_service.h"
#include "utils/ConstDefine.h"
#include "service/DbUiWebcallService.h"
#include "session/el_sessionservice.h"
#include "model/yl_model/UIYL_ChatTreeModel.h"

UICardDataWidgetPanel::UICardDataWidgetPanel(QWidget *parent /*= 0*/)
{
    ui.setupUi(this);
    ui.m_extendWdiget->setVisible(gUIExtendConfigManager->HasExtendCardOrderCols());
    m_timer = new QTimer(this);
    m_timer->setInterval(5000);
    connect(m_timer, &QTimer::timeout, this, &UICardDataWidgetPanel::R_timeout);
    Init();
    InitExtendWidget();
}

UICardDataWidgetPanel::~UICardDataWidgetPanel()
{
}

void UICardDataWidgetPanel::Init()
{
    ui.Echowidget->hide();
    ui.choicewidget->show();
    ui.m_autosavecardCBox->setChecked(true);
    connect(CommonSignalService::GetInstance(), SIGNAL(S_CurrentTabPageChanged(E_TABTYPE, E_TABTYPE)), this, SLOT(R_CurrentTabPageChanged(E_TABTYPE, E_TABTYPE)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_SaveVisitorCard(bool)), this, SLOT(R_SaveVisitorCard(bool)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_OnEchoSummary(QString)), this, SLOT(R_OnEchoSummary(QString)));

    connect(this, SIGNAL(S_TempCacheMCardInfo(E_TABTYPE, const MUICardInfoData &)), CommonSignalService::GetInstance(), SIGNAL(S_TempCacheMCardInfo(E_TABTYPE, const MUICardInfoData &)));
    connect(this, SIGNAL(S_OnSaveVisitorCardBtnClick(E_TABTYPE, bool, const MUICardInfoData &, QString)), CommonSignalService::GetInstance(), SIGNAL(S_OnSaveVisitorCardBtnClick(E_TABTYPE, bool, const MUICardInfoData &, QString)));

    m_cardStaticId = "";
    m_chatid       = "";
    m_visitorId    = "";
    m_ip           = "";
    m_ipv6         = "";

    connect(ui.m_autosavecardCBox, SIGNAL(clicked(bool)), this, SLOT(OnAutoSaveCard(bool)));
    connect(ui.m_editBtn, SIGNAL(clicked()), this, SLOT(OnEditClicked()));
    connect(ui.m_saveBtn, SIGNAL(clicked()), this, SLOT(R_OnSaveVisitorCardBtnClick()));
    connect(ui.m_newCardSaveBtn, SIGNAL(clicked()), this, SLOT(R_OnNewCardSaveVisitorCardBtnClick()));
    QString newcard = UICommonDataModel::GetInstance()->GetSystemConfigByKey("newcard", "0");
    if (newcard == "1")
    {
        ui.m_newCardSaveBtn->setVisible(true);
    }
    else
    {
        ui.m_newCardSaveBtn->setVisible(false);
    }

    gSessionService->HasPermissionByKey("chatSummaryLabel");
    connect(gIProtocolAgent, SIGNAL(S_GetUserPermissionListResult(QString, QString, QString)), this, SLOT(R_GetUserPermissionListResult(QString, QString, QString)));

    // CZC 接收是否显示“新建名片标签”按钮
    connect(CommonSignalService::GetInstance(), SIGNAL(S_OtherSettingChanged()), this, SLOT(R_OtherSettingChanged()));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_GetCompanyGlobalConfResult(QString, QString, QString)), this, SLOT(R_GetCompanyGlobalConfResult(QString, QString, QString)));
}

void UICardDataWidgetPanel::RemoveUpdate(QString type, QString body)
{
}

void UICardDataWidgetPanel::Update(QString type, QObject *obj)
{
    ///通用的协议信号///////////////////////////////////////////////////////////////////////
    if (StringCompare(type, "ADD_COL_DATA"))
    {
        MCardColData *objData = (MCardColData *)obj;
        AddCardCol(objData);
    }
    else if (StringCompare(type, "SHOW_CARDINFO"))
    {
        MUICardInfoData *objData = (MUICardInfoData *)obj;
        ShowCardInfo(objData);
    }
    else if (StringCompare(type, "CLEAR_CARDINFO"))
    {
        ClearCardInfo();
        CommonSignalService::GetInstance()->DoUpdateThirdCardTab(nullptr);
    }
    else if (StringCompare(type, "UPDATE_CARDINFO"))
    {
        MUICardInfoData *objData = (MUICardInfoData *)obj;
        UpdateCardInfo(objData);
        //更新第三方表单页
        if (objData)
            CommonSignalService::GetInstance()->DoUpdateThirdCardTab(objData);
    }
    else if (StringCompare(type, "SHOW_EXTENDCARDINFO"))
    {
        MChatItemData *objData = (MChatItemData *)obj;
        ShowExtendCardInfo(objData);
        //更新第三方表单页
        UpdateThirdFormData();
    }
    else if (StringCompare(type, "CLEAR_EXTENDCARDINFO"))
    {
        ClearExtendCardInfo();
        CommonSignalService::GetInstance()->DoUpdateThirdCardTab(nullptr);
    }
    else if (StringCompare(type, "SHOW_WS_MONITORCHAT_EXTENDCARDINFO"))
    {
        MMonitorChatData *objData = (MMonitorChatData *)obj;
        ShowExtendCardInfo(objData);
    }
    else if (StringCompare(type, "SHOW_WS_MONITORVISITOR_EXTENDCARDINFO"))
    {
        MMonitorVisitorData *objData = (MMonitorVisitorData *)obj;
        ShowExtendCardInfo(objData);
    }
    else if (StringCompare(type, "INITCOLDATA_END"))
    {
        InitDataEnd();
    }

    //【曹振超】对话列表和历史对话手动结束对话、系统自动结束对话，触发名片自动保存
    else if (StringCompare(type, "AutoSaveCard"))
    {
        MUICardInfoData *data = (MUICardInfoData *)obj;
        if (data)
        {
            bool isSame = IsSame_CompareSourceAndUIData(data);
            if (!isSame)
            {
                //界面数据和原始数据不一致，触发保存功能
                SaveVisitorCard(false);
            }
        }
    }
    //增加强制保存
    else if (StringCompare(type, "AutoForceSaveCard"))
    {
        //界面数据和原始数据不一致，触发保存功能
        SaveVisitorCard(false);
    }

    //增加强制保存
    else if (StringCompare(type, "AutoForceSaveCard"))
    {
        //界面数据和原始数据不一致，触发保存功能
        SaveVisitorCard(false);
    }

    else
    {
        // Q_ASSERT(false);
    }
}

void UICardDataWidgetPanel::OnEditClicked()
{
    CommonSignalService::GetInstance()->DoShowIeWndByUri(E_Uri_CenterManager, 0, gUIPersonalConfigManager->CardInfoUrl());
    gIProtocolAgent->DoUploadOperatorLog("online_client_conversation_44");
}

void UICardDataWidgetPanel::R_OnSaveVisitorCardBtnClick()
{
    SaveVisitorCard(false);
}

void UICardDataWidgetPanel::R_OnNewCardSaveVisitorCardBtnClick()
{
    SaveVisitorCard(true);
}

void UICardDataWidgetPanel::AddCardCol(MCardColData *objData)
{
    if (objData == nullptr)
    {
        return;
    }
    QMap<QString, UICardItemData *>::Iterator itor = m_uiCardItemDataList.find(objData->ColName());
    if (itor != m_uiCardItemDataList.end())
    {
        return;
    }

    if (objData->ColType() == "1")
    {
        QString colName = objData->ColName().toUpper();
        if (!gUIExtendConfigManager->IsCardInfoExtByKey(colName) && (objData->ItemList().size() == 0))
        {
            return;
        }
    }
    else if ((objData->ColType() == "4") && (objData->ItemList().size() == 0))
    {
        if (objData->ColName() == "extColumn8" || objData->ColName() == "extColumn9")
        {
        }
        else
        {
            return;
        }
    }

    UICardItemData *itemData = new UICardItemData();
    itemData->InitData(objData);
    connect(itemData, &UICardItemData::S_ValueChange, this, &UICardDataWidgetPanel::R_ValueChange);
    m_uiCardItemDataList.insert(objData->ColName(), itemData);
}

void UICardDataWidgetPanel::ClearCardCol()
{
    m_cardStaticId = "";
    m_chatid       = "";
    for (auto itor = m_uiCardItemDataList.begin(); itor != m_uiCardItemDataList.end(); itor++)
    {
        UICardItemData *objData = itor.value();
        if (objData)
        {
            objData->ClearContent();
        }
    }
}

void UICardDataWidgetPanel::TempCacheMcardInfo()
{
    if (m_cardStaticId.isEmpty())
    {
        return;
    }
    MUICardInfoData cardData;
    for (auto itor = m_uiCardItemDataList.begin(); itor != m_uiCardItemDataList.end(); itor++)
    {
        UICardItemData *objData = itor.value();
        if (objData)
        {
            QString val = objData->GetValue();
            cardData.SetUICardValueByKey(objData->ColName(), val);
            if (objData->ColName() == "mobile")
            {
                cardData.SetUICardValueByKey("phoneStatus", objData->GetphoneStatus());
            }
        }
    }
    cardData.VisitorStaticId(m_cardStaticId);
    cardData.ChatId(m_chatid);
    cardData.Vid(m_visitorId);
    Q_EMIT S_TempCacheMCardInfo(TabType(), cardData);
}

void UICardDataWidgetPanel::SetControlValue(UIControl &control, QString &val)
{
    if (control.m_controlType == "0")
    {
        QLineEdit *line = (QLineEdit *)control.m_control;
        if (line)
        {
            line->setText(val);
        }
    }
    else if (control.m_controlType == "1")
    {
        if (gUIExtendConfigManager->IsCardInfoExtByKey(control.m_controlName))
        {
            UISearchCBox *cbox = (UISearchCBox *)control.m_control;
            cbox->setText(val);
        }
        else
        {
            QComboBox *cbox = (QComboBox *)control.m_control;
            if (cbox)
            {
                if (control.m_controlName == "sex")
                {
                    if (val == "0")
                    {
                        cbox->setCurrentIndex(0);
                    }
                    else if (val == "1")
                    {
                        cbox->setCurrentIndex(1);
                    }
                    else
                    {
                        cbox->setCurrentIndex(2);
                    }
                }
                else
                {
                    bool isOk  = false;
                    int  count = cbox->count();
                    for (int i = 0; i < count; i++)
                    {
                        cbox->setCurrentIndex(i);
                        QString itemval = cbox->currentText();
                        if (val == itemval)
                        {
                            isOk = true;
                            break;
                        }
                    }
                    if (!isOk)
                    {
                        cbox->setCurrentIndex(0);
                    }
                }
            }
        }
    }
    else if (control.m_controlType == "2")
    {
        QTextEdit *line = (QTextEdit *)control.m_control;
        if (line)
        {
            line->setText(val);
        }
    }
    else if (control.m_controlType == "3")
    {
        QDateTimeEdit *line = (QDateTimeEdit *)control.m_control;
        if (line)
        {
            line->setDateTime(QDateTime::currentDateTime());
        }
    }
    else if (control.m_controlType == "4")
    {
        QComboBox *cbox = (QComboBox *)control.m_control;
        if (cbox)
        {

            bool isOk  = false;
            int  count = cbox->count();
            for (int i = 0; i < count; i++)
            {
                cbox->setCurrentIndex(i);
                QString itemval = cbox->itemData(i).toString();
                if (val == itemval)
                {
                    isOk = true;
                    break;
                }
            }
            if (!isOk)
            {
                cbox->setCurrentIndex(0);
            }
        }
    }
    else if (control.m_controlType == "7")
    {
        QComboBox *cbox = (QComboBox *)control.m_control;
        if (cbox)
        {

            bool isOk  = false;
            int  count = cbox->count();
            for (int i = 0; i < count; i++)
            {
                cbox->setCurrentIndex(i);
                QString itemval = cbox->itemData(i).toString();
                if (val == itemval)
                {
                    isOk = true;
                    break;
                }
            }
            if (!isOk)
            {
                cbox->setCurrentIndex(0);
            }
        }
    }
    else
    {
        Q_ASSERT(false);
        GERRORLOG("控件类型错误。");
    }
}
QString UICardDataWidgetPanel::GetControlValue(UIControl &control)
{
    QString ret = "";
    if (control.m_controlType == "0")
    {
        QLineEdit *line = (QLineEdit *)control.m_control;
        if (line)
        {
            ret = line->text();
        }
    }
    else if (control.m_controlType == "1")
    {
        if (gUIExtendConfigManager->IsCardInfoExtByKey(control.m_controlName))
        {
            UISearchCBox *cbox = (UISearchCBox *)control.m_control;
            ret                = cbox->text();
        }
        else
        {
            QComboBox *cbox = (QComboBox *)control.m_control;
            if (cbox)
            {
                if (control.m_controlName == "sex")
                {
                    int index = cbox->currentIndex();
                    ret       = QString::number(index);
                }
                else
                {
                    ret = cbox->currentText().trimmed();
                }
            }
        }
    }
    else if (control.m_controlType == "2")
    {
        QTextEdit *line = (QTextEdit *)control.m_control;
        if (line)
        {
            ret = line->toPlainText().trimmed();
        }
    }
    else if (control.m_controlType == "3")
    {
        QDateTimeEdit *line = (QDateTimeEdit *)control.m_control;
        if (line)
        {
            line->setDateTime(QDateTime::currentDateTime());
            ret = line->text().trimmed();
        }
    }
    else if (control.m_controlType == "4")
    {
        QComboBox *cbox = (QComboBox *)control.m_control;
        if (cbox)
        {
            int     index   = cbox->currentIndex();
            QString itemval = cbox->itemData(index).toString();
            ret             = itemval;
        }
    }
    else if (control.m_controlType == "7")
    {
        QComboBox *cbox = (QComboBox *)control.m_control;
        if (cbox)
        {
            int     index   = cbox->currentIndex();
            QString itemval = cbox->itemData(index).toString();
            ret             = itemval;
        }
    }
    else
    {
        Q_ASSERT(false);
    }
    return ret;
}

void UICardDataWidgetPanel::ShowCardInfo(MUICardInfoData *objData)
{
    m_timer->stop(); //切换对话后 停止出发自动保存
    if (objData == nullptr)
    {
        return;
    }

    if (m_cardStaticId.isEmpty() || m_cardStaticId == objData->VisitorStaticId())
    {
        SetControlValueByMCardInfo(objData);
    }
    else
    {
        TempCacheMcardInfo();
        ClearCardCol();
        SetControlValueByMCardInfo(objData);
    } R_OnEchoSummary(objData->ChatId());
}

void UICardDataWidgetPanel::R_OnEchoSummary(QString chatId)
{
    if (m_chatid == chatId)
    {
        //回显 总结标签
        MChatItemData *tempChat = UIYL_ChatTreeModel::GetInstance()->GetMChatItemDataBychatId(chatId);
        if (tempChat)
        {
            QString summary_type = tempChat->Summary_type();
            if (summary_type.isEmpty())
            {
                UIChatInfo *chatInfo = UICommonDataModel::GetInstance()->GetChatInfoByChatId(chatId); //重新登录后,拉取正在通话中的访客信息,对话总结标签数据位置
                if (chatInfo)
                {
                    UIChatUserInfo *visitorInfo = nullptr;
                    if (chatInfo->GetVisitorInfo())
                    {
                        visitorInfo = chatInfo->GetVisitorInfo();
                    }
                    if (visitorInfo && visitorInfo->OtherInfos())
                    {
                        if (summary_type.isEmpty())
                            summary_type = visitorInfo->OtherInfos()->GetValueByKey("summary_type");
                    }
                }
            }

            UISummarizeInfo *tagInfo = UICommonDataModel::GetInstance()->GetSummaryTagById(summary_type);
            if (tagInfo)
            {
                QString strCardColor = tagInfo->Color();
                QString strTitle     = tagInfo->Title();
                QColor  color(strCardColor);
                if (color.isValid())
                {
                    showEchowidget(true);
                    QString style = QString("background-color: %1;").arg(strCardColor);
                    ui.Echocolour->setStyleSheet(style);
                    ui.Echoname->setText(strTitle);
                    return;
                }
            }
        }
        showEchowidget(false);
    }
}

void UICardDataWidgetPanel::showEchowidget(bool show)
{
    if (show)
    {
        ui.Echowidget->show();
        ui.choicewidget->hide();
    }
    else
    {
        ui.Echowidget->hide();
        ui.choicewidget->show();
    }
}

void UICardDataWidgetPanel::SetControlValueByMCardInfo(MUICardInfoData *objData)
{
    if (objData == nullptr)
    {
        return;
    }
    m_cardStaticId = objData->VisitorStaticId();
    m_chatid       = objData->ChatId();
    m_visitorId    = objData->Vid();

    bool isHideMobile = false;
    // this->m_chatid
    UIChatInfo *chatinfo = UICommonDataModel::GetInstance()->GetChatByChatId(m_chatid);
    if (chatinfo)
    {
        isHideMobile = gUIPersonalConfigManager->IsHiddenMobile(chatinfo->CustomerID());
    }

    QMap<QString, QString> cardInfoMap = objData->GetCardInfo();
    m_ip                               = cardInfoMap["ip"];
    m_ipv6                             = cardInfoMap["ipv6"];

    for (auto itor = m_uiCardItemDataList.begin(); itor != m_uiCardItemDataList.end(); itor++)
    {
        UICardItemData *objUIData = itor.value();
        if (objUIData)
        {
            QString val = objData->GetUICardValueByKey(objUIData->ColName());

            if (!val.isEmpty())
            {
                objUIData->SetValue(val, isHideMobile);

                if (objUIData->ColName() == "mobile")
                {
                    // CZC：202012--手动保存名片时判断手机号格式，如果非有效手机号，输入框红框显示
                    if (UtilsHelper::IsMobilePhone(val))
                    {
                        objUIData->SetItemRedBorder(false);
                    }
                    else
                    {
                        objUIData->SetItemRedBorder(true);
                    }
                    objUIData->SetphoneStatus(objData->GetUICardValueByKey("phoneStatus"));
                }
            }
            else
            {
                int     idLoaded = 0;
                QString oldVal = objUIData->GetValue();
                if (!oldVal.isEmpty())
                {
                    idLoaded = 1;
                    objUIData->SetValue(val, isHideMobile);
                }

                if (objUIData->ColName() == "ntag")
                {
                    if (idLoaded == 0)
                        objUIData->SetValue(val, isHideMobile);
                }
                else if (objUIData->ColName() == "mobile")
                {
                    objUIData->SetItemRedBorder(false);
                    objUIData->SetphoneStatus("");
                }
            }
        }
    }
}

void UICardDataWidgetPanel::UpdateCardInfo(MUICardInfoData *objData)
{
    if (objData == nullptr)
    {
        return;
    }
    if (objData->VisitorStaticId() != m_cardStaticId)
    {
        return;
    }
    SetControlValueByMCardInfo(objData);
}

void UICardDataWidgetPanel::SaveVisitorCard(bool isNew, QString isAuto)
{
    m_timer->stop(); //停止出发自动保存

    if (!m_cardStaticId.isEmpty())
    {
        MUICardInfoData cardData;

        
        if( UpdateCardInfoData(cardData, (isAuto=="1")?0:1) == 1)
        {
                //提示
                CommonSignalService::GetInstance()->DoShowToastMsg(tr("黄框内名片字段建议填写~"), 2, Enum_YL_Toast_Parent_RightCenter, Enum_YL_Toast_Type_Error, QPoint());
        }
		Q_EMIT S_OnSaveVisitorCardBtnClick(TabType(), isNew, cardData, isAuto);
    }
}

int UICardDataWidgetPanel::UpdateCardInfoData(MUICardInfoData &cardData, int savetype )
{
    int ret = 0;
    for (auto itor = m_uiCardItemDataList.begin(); itor != m_uiCardItemDataList.end(); itor++)
    {
        UICardItemData *objData = itor.value();
        if (objData)
        {
            QString val = objData->GetValue();

            if (objData->ColName() == "mobile")
            {
                if (val.indexOf("*") != -1)
                {
                    UICardInfo *info = UICommonDataModel::GetInstance()->GetVisitorCardByUserId(m_cardStaticId);
                    if (info)
                    {
                        val = info->Mobile();
                        info->deleteLater();
                    }
                }

                // CZC : 202012--手动保存名片时判断手机号格式，如果非有效手机号并且非空字符串时，输入框红框显示
                if (UtilsHelper::IsMobilePhone(val) || val.isEmpty())
                {
                    objData->SetItemRedBorder(false);
                }
                else
                {
                    objData->SetItemRedBorder(true);
                }
                cardData.SetUICardValueByKey("phoneStatus", objData->GetphoneStatus());
            }
            else if (objData->ColName() == "note")
            {
                if (val.indexOf("*") != -1)
                {
                    UICardInfo *info = UICommonDataModel::GetInstance()->GetVisitorCardByUserId(m_cardStaticId);
                    if (info)
                    {
                        val = info->Note();
                        info->deleteLater();
                    }
                }
            }
			if (val.isEmpty())
			{
				//判断是否必填项
                if ( (savetype == 1) &&( objData->Required() == 1))
				{
                    objData->SetItemOrangeBorder(true);
                    ret = 1;
                }

            }else
            {
                if ( savetype == 1)
                    objData->SetItemOrangeBorder(false);
            }

            cardData.SetUICardValueByKey(objData->ColName(), val);
        }
    }

    cardData.VisitorStaticId(m_cardStaticId);
    cardData.ChatId(m_chatid);
    cardData.Vid(m_visitorId);

    QMap<QString, QString> cardInfoMap = cardData.GetCardInfo();
    cardInfoMap["ip"]                  = m_ip;
    cardInfoMap["ipv6"]                = m_ipv6;
    cardData.SetCardInfo(cardInfoMap);


    return ret;
}

//【曹振超】判断原始数据和界面数据是否一致
bool UICardDataWidgetPanel::IsSame_CompareSourceAndUIData(MUICardInfoData *data)
{
    bool                   isSame      = true;
    QMap<QString, QString> mapCardInfo = data->GetCardInfo();

    for (auto itCardItem = m_uiCardItemDataList.begin(); itCardItem != m_uiCardItemDataList.end(); ++itCardItem)
    {
        QString strColName    = itCardItem.key();
        QString strColText    = itCardItem.value()->SelfText();
        QString strColContent = itCardItem.value()->GetValue();
        QString strColType    = itCardItem.value()->ColType();

        bool isSameCardData = IsFilter_CompareCardData(strColName, strColText, strColContent, strColType, mapCardInfo);
        if (!isSameCardData)
        {
            //排除被过滤的字段后，判断字段内容是否被更改
            if (mapCardInfo.contains(strColName))
            {
                QString strCardInfoValue = mapCardInfo[strColName];
                if (strCardInfoValue != strColContent)
                {
                    isSame = false;
                    break;
                }
            }
        }
    }

    return isSame;
}

bool UICardDataWidgetPanel::IsFilter_CompareCardData(QString strColName, QString strColText, QString strColContent, QString strColType, QMap<QString, QString> mapCardInfo)
{
    bool isSame = false;

    //系统预置字段
    if (strColName.indexOf("extColumn") == -1)
    {
        //过滤所有的时间字段
        if ((strColName.indexOf("time") != -1) || (strColName.indexOf("Time") != -1))
        {
            isSame = true;
        }

        //过滤创建者、修改者
        else if ((strColName.indexOf("user") != -1) || (strColName.indexOf("User") != -1))
        {
            isSame = true;
        }

        //过滤标签
        else if (strColName == "ntag")
        {
            if ((mapCardInfo[strColName].isEmpty() || mapCardInfo[strColName] == "0") && (strColContent.isEmpty() || (strColContent == "0")))
            {
                isSame = true;
            }
        }
        else if (strColName == "mobile")
        {
            if (strColContent.indexOf("*") != -1)
            {
                isSame = true;
            }
        }
        else if (strColName == "note")
        {
            if (strColContent.indexOf("*") != -1)
            {
                isSame = true;
            }
        }

        //过滤性别
        else if (strColName == "sex")
        {
            if ((mapCardInfo[strColName].isEmpty() || mapCardInfo[strColName] == "2") && (strColContent == "2"))
            {
                isSame = true;
            }
        }

        //过滤其它下拉框
        else if ((strColType == "1") || (strColType == "4")|| (strColType == "7"))
        {
            isSame = true;
        }
    }

    //自定义字段
    else
    {
        //过滤所有的时间字段
        if (strColText.indexOf(tr("时间"), Qt::CaseInsensitive) != -1)
        {
            isSame = true;
        }

        //过滤所有字段的值是时间的
        else if (CompareString_ByRegExp(strColContent) == QValidator::Acceptable)
        {
            isSame = true;
        }

        //过滤其它下拉框
        else if ((strColType == "1") || (strColType == "4")|| (strColType == "7"))
        {
            isSame = true;
        }
    }

    return isSame;
}

//【曹振超】判断字符串是否满足正则表达式
QValidator::State UICardDataWidgetPanel::CompareString_ByRegExp(QString &str)
{
    int               pos = 0;
    QRegExp           reg("([0-9]{4})-([0-9]{2})-([0-9]{2}) ([0-9]{2}):([0-9]{2}):([0-9]{2})");
    QRegExpValidator  regVali(reg);
    QValidator::State state = regVali.validate(str, pos);

    return state;
}

void UICardDataWidgetPanel::R_OtherSettingChanged()
{
    OtherSettingChanged();
}

void UICardDataWidgetPanel::OtherSettingChanged()
{
    QString newcard = UICommonDataModel::GetInstance()->GetSystemConfigByKey("newcard", "0");
    if (newcard == "1")
    {
        ui.m_newCardSaveBtn->setVisible(true);
    }
    else
    {
        ui.m_newCardSaveBtn->setVisible(false);
    }
}

void UICardDataWidgetPanel::ClearCardInfo()
{
    if (!m_cardStaticId.isEmpty())
    {
        TempCacheMcardInfo();
    }
    ClearCardCol();
    showEchowidget(false);
}

void UICardDataWidgetPanel::InitExtendWidget()
{
    connect(ui.m_searchBtn, SIGNAL(clicked()), this, SLOT(R_OnSearchBtnClick()));

    int                      index    = 0;
    QList<ExtendOrderInfo *> tempList = UICommonDataModel::GetInstance()->ExtendCardOrderColsList();
    Q_FOREACH (ExtendOrderInfo *info, tempList)
    {
        if (info && info->Hide() == "0")
        {
            QHBoxLayout *hLayout = new QHBoxLayout(this);
            QSpacerItem *hSpacer = new QSpacerItem(5, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

            QLabel *lItem = new QLabel(this);
            lItem->setMinimumWidth(60);
            lItem->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

            lItem->setText(info->Name());
            hLayout->addSpacerItem(hSpacer);
            hLayout->addWidget(lItem);

            if (info->Type() == "1")
            {
                QLineEdit *lineEdit = new QLineEdit(this);
                if (!info->CardKey().isEmpty())
                {
                    lineEdit->setText("");
                }

                lineEdit->setMinimumHeight(24);
                // lineEdit->setFixedSize(200,30);
                lineEdit->setStyleSheet("QLineEdit {border:1px solid #dddddd;color:#333333;}");

                hLayout->addWidget(lineEdit);
                UIControl control;
                control.m_control     = lineEdit;
                control.m_controlName = info->Key();
                control.m_controlType = info->Type();
                m_orderCardInfoList.insert(info->Key(), control);
            }
            else if (info->Type() == "2")
            {
                QComboBox *cbox = new NoScrollComboBox(this);
                WheelEventFilter *filter = new WheelEventFilter();
                cbox->installEventFilter(filter);

                cbox->setStyleSheet("QComboBox {border:1px solid #e1e0e0;}");
                QList<CExtendInfoItem *> items = info->Items();
                foreach (CExtendInfoItem *valItem, items)
                {
                    if (valItem)
                    {
                        cbox->addItem(valItem->ItemName(), valItem->Id());
                    }
                }

                cbox->setMinimumHeight(24);
                cbox->setMinimumWidth(120);
                cbox->setCurrentIndex(0);
                cbox->setMaxVisibleItems(20);
                hLayout->addWidget(cbox);

                UIControl control;
                control.m_control     = cbox;
                control.m_controlName = info->Key();
                control.m_controlType = info->Type();
                m_orderCardInfoList.insert(info->Key(), control);
            }
            else if (info->Type() == "3")
            {
                QDateTimeEdit *lineEdit = new QDateTimeEdit(this);
                lineEdit->setDateTime(QDateTime::currentDateTime());
                // lineEdit->setFixedSize(200,30);
                lineEdit->setStyleSheet("QLineEdit {border:1px solid #dddddd;color:#333333;}");
                lineEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
                hLayout->addWidget(lineEdit);
                UIControl control;
                control.m_control     = lineEdit;
                control.m_controlName = info->Key();
                control.m_controlType = info->Type();
                m_orderCardInfoList.insert(info->Key(), control);
            }
            else if (info->Type() == "4")
            {
                QTextEdit *txtEdit = new QTextEdit(this);

                txtEdit->setStyleSheet("QTextEdit {border:1px solid #dddddd;color:#333333;}");
                // txtEdit->setFixedSize(215, 80);
                txtEdit->setAcceptRichText(false);

                txtEdit->setText("");
                hLayout->addWidget(txtEdit);
                UIControl control;
                control.m_control     = txtEdit;
                control.m_controlName = info->Key();
                control.m_controlType = info->Type();
                m_orderCardInfoList.insert(info->Key(), control);
            }
            else if (info->Type() == "7")
            {
                QTextEdit *txtEdit = new QTextEdit(this);

                txtEdit->setStyleSheet("QTextEdit {border:1px solid #dddddd;color:#333333;}");
                // txtEdit->setFixedSize(215, 80);
                txtEdit->setAcceptRichText(false);

                txtEdit->setText("");
                hLayout->addWidget(txtEdit);
                UIControl control;
                control.m_control     = txtEdit;
                control.m_controlName = info->Key();
                control.m_controlType = info->Type();
                m_orderCardInfoList.insert(info->Key(), control);
            }
            else
            {
                Q_ASSERT(false);
            }
            QSpacerItem *hSpacer2 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

            hLayout->addSpacerItem(hSpacer2);
            hLayout->setSpacing(10);
            hLayout->setContentsMargins(5, 0, 5, 0);

            ui.m_extendColsVerticalLayout->insertLayout(index, hLayout);
            index++;
        }
    }
    UICommonDataModel::GetInstance()->ClearExtendOrderInfoList(tempList);
}

void UICardDataWidgetPanel::R_OnSearchBtnClick()
{
    QMap<QString, QString> tempMap;
    for (auto itor = m_orderCardInfoList.begin(); itor != m_orderCardInfoList.end(); itor++)
    {
        QString   key     = itor.key();
        UIControl control = itor.value();
        QString   val     = GetExtendControlValue(control);
        tempMap.insert(key, val);
    }
    CommInfo *info = new CommInfo();
    info->AddCommInfo(tempMap);

    gUIExtendConfigManager->ExtendPostCardOrderInfo(info, m_visitorId, m_cardStaticId, m_chatid);
}

void UICardDataWidgetPanel::ShowExtendCardInfo(MChatItemData *objData)
{
    if (objData)
    {
        if (objData->UserType() == E_VISITOR || objData->UserType() == E_MONITOR)
        {
            if (objData->GetExtendData(CARDSAVE_NO_UPLOADOCPC) == "1")
            {
                ui.m_autosavecardCBox->setChecked(false);
            }
            else
            {
                ui.m_autosavecardCBox->setChecked(true);
            }
            ShowExtendcardInfoByChatId(objData->Chatid());
        }
    }
}

void UICardDataWidgetPanel::ShowExtendCardInfo(MMonitorChatData *objData)
{
    if (objData)
    {
        ShowExtendcardInfoByChatId(objData->ChatId());
    }
}

void UICardDataWidgetPanel::ShowExtendCardInfo(MMonitorVisitorData *objData)
{
    if (objData)
    {
        ShowExtendcardInfoByChatId(objData->GetUIVisitorInfoValueByKey("inviteChatId"));
    }
}

void UICardDataWidgetPanel::ClearExtendCardInfo()
{
    for (auto itor = m_orderCardInfoList.begin(); itor != m_orderCardInfoList.end(); itor++)
    {
        UIControl control = itor.value();
        if (control.m_controlType == "1")
        {
            QLineEdit *line = (QLineEdit *)control.m_control;
            if (line)
            {
                line->setText("");
            }
        }
        else if (control.m_controlType == "2")
        {
            QComboBox *cbox = (QComboBox *)control.m_control;
            if (cbox)
            {
                cbox->setCurrentIndex(0);
            }
        }
        else if (control.m_controlType == "3")
        {
            QDateTimeEdit *line = (QDateTimeEdit *)control.m_control;
            if (line)
            {
                line->setDateTime(QDateTime::currentDateTime());
            }
        }
        else if (control.m_controlType == "4")
        {
            QTextEdit *line = (QTextEdit *)control.m_control;
            if (line)
            {
                line->setText("");
            }
        }
        else if (control.m_controlType == "7")
        {
            QTextEdit *line = (QTextEdit *)control.m_control;
            if (line)
            {
                line->setText("");
            }
        }
        else
        {
            Q_ASSERT(false);
        }
    }
}

void UICardDataWidgetPanel::SetExtendControlValue(UIControl &control, QString &val)
{
    if (control.m_controlType == "1")
    {
        QLineEdit *line = (QLineEdit *)control.m_control;
        if (line)
        {
            line->setText(val);
        }
    }
    else if (control.m_controlType == "2")
    {
        QComboBox *cbox = (QComboBox *)control.m_control;
        if (cbox)
        {
            bool isOk  = false;
            int  count = cbox->count();
            for (int i = 0; i < count; i++)
            {
                cbox->setCurrentIndex(i);
                QString itemval = cbox->currentText();
                if (val == itemval)
                {
                    isOk = true;
                    break;
                }
            }
            if (!isOk)
            {
                cbox->setCurrentIndex(0);
            }
        }
    }
    else if (control.m_controlType == "3")
    {
        QDateTimeEdit *line = (QDateTimeEdit *)control.m_control;
        if (line)
        {
            line->setDateTime(QDateTime::currentDateTime());
        }
    }
    else if (control.m_controlType == "4")
    {
        QTextEdit *line = (QTextEdit *)control.m_control;
        if (line)
        {
            line->setText(val);
        }
    }
    else if (control.m_controlType == "7")
    {
        QTextEdit *line = (QTextEdit *)control.m_control;
        if (line)
        {
            line->setText(val);
        }
    }
    else
    {
        Q_ASSERT(false);
    }
}

QString UICardDataWidgetPanel::GetExtendControlValue(UIControl &control)
{
    QString ret = "";
    if (control.m_controlType == "1")
    {
        QLineEdit *line = (QLineEdit *)control.m_control;
        if (line)
        {
            ret = line->text();
        }
    }
    else if (control.m_controlType == "2")
    {
        QComboBox *cbox = (QComboBox *)control.m_control;
        if (cbox)
        {
            ret = cbox->currentText().trimmed();
        }
    }

    else if (control.m_controlType == "3")
    {
        QDateTimeEdit *line = (QDateTimeEdit *)control.m_control;
        if (line)
        {
            line->setDateTime(QDateTime::currentDateTime());
            ret = line->text().trimmed();
        }
    }
    else if (control.m_controlType == "4")
    {
        QTextEdit *line = (QTextEdit *)control.m_control;
        if (line)
        {
            ret = line->toPlainText().trimmed();
        }
    }
    else if (control.m_controlType == "7")
    {
        QTextEdit *line = (QTextEdit *)control.m_control;
        if (line)
        {
            ret = line->toPlainText().trimmed();
        }
    }
    else
    {
        Q_ASSERT(false);
    }
    return ret;
}

void UICardDataWidgetPanel::R_CurrentTabPageChanged(E_TABTYPE oldType, E_TABTYPE curType)
{
    CurrentType(curType);
}

void UICardDataWidgetPanel::ShowExtendcardInfoByChatId(QString chatId)
{
    UIChatInfo *infoObj = UICommonDataModel::GetInstance()->GetChatInfoByChatId(chatId);
    if (infoObj && infoObj->GetVisitorInfo() && infoObj->GetVisitorInfo()->OtherInfos())
    {
        CommInfo *commObj = infoObj->GetVisitorInfo()->OtherInfos();
        if (commObj)
        {
            for (auto itor = m_orderCardInfoList.begin(); itor != m_orderCardInfoList.end(); itor++)
            {
                QString   key     = itor.key();
                QString   val     = commObj->GetValueByKey(key);
                UIControl control = itor.value();
                SetExtendControlValue(control, val);
            }
        }
    }
}

void UICardDataWidgetPanel::InitDataEnd()
{
    int count = ui.m_visitorColsVerticalLayout->count();
    if (count != 0)
    {
        QLayoutItem *child;
        while ((child = ui.m_visitorColsVerticalLayout->takeAt(0)) != 0)
        {
            delete child;
        }
    }
    QVBoxLayout *layOut = new QVBoxLayout();

    for (auto itor = m_uiCardItemDataList.begin(); itor != m_uiCardItemDataList.end(); itor++)
    {
        UICardItemData *itemData = itor.value();

        bool isAdd = false;
        for (int i = 0; i < layOut->count(); i++)
        {
            UICardItemData *val = (UICardItemData *)layOut->itemAt(i)->widget();
            if (itemData->ColType() == "2")
            {
                itemData->SetItemHeight(80);
            }

            if (val->Index() > itemData->Index())
            {
                isAdd = true;
                layOut->insertWidget(i, itemData);
                break;
            }
        }
        if (!isAdd)
        {
            layOut->addWidget(itemData);
        }
    }
    {
        ET("time_add_card_panel: ");
        ui.m_visitorColsVerticalLayout->addLayout(layOut);
    }
}

void UICardDataWidgetPanel::R_SaveVisitorCard(bool isNew)
{
    SaveVisitorCard(isNew);
}

void UICardDataWidgetPanel::R_GetCompanyGlobalConfResult(QString code, QString reason, QString body)
{
    QString cardparam = gUIPersonalConfigManager->CardInfoUrl();
    if (cardparam.isEmpty())
    {
        ui.m_editBtn->setVisible(false);
    }
    else
    {
        ui.m_editBtn->setVisible(true);
    }
}

void UICardDataWidgetPanel::R_GetUserPermissionListResult(QString code, QString reason, QString body)
{ //是否显示"总结标签"按钮
    if (!gSessionService->HasPermissionByKey("chatSummaryLabel"))
    {
        ui.Summarywidget->hide();
    }
}


void UICardDataWidgetPanel::R_ValueChange(QString CType) //触发5秒自动保存
{
    Q_UNUSED(CType);
    if (!m_cardStaticId.isEmpty()) //访客静态ID为空,代表当前不在有效对话中.无需保存名片
    {
        m_timer->stop();
        m_timer->start();
    }
}

void UICardDataWidgetPanel::R_timeout()
{
    SaveVisitorCard(false, "1"); //自动保存
}


void UICardDataWidgetPanel::UpdateThirdFormData()
{
    MUICardInfoData cardData;
    UpdateCardInfoData(cardData);
    CommonSignalService::GetInstance()->DoUpdateThirdCardTab(&cardData);
}

void UICardDataWidgetPanel::OnAutoSaveCard(bool val)
{
    if (!m_chatid.isEmpty() || !m_cardStaticId.isEmpty())
    {
        QMap<QString, QString> aiMap;
        aiMap.insert(CARDSAVE_NO_UPLOADOCPC, val ? "0" : "1");

        gIProtocolAgent->DoExecSendAiControlCmd(m_chatid, aiMap);
        // todo 9725 上报

        UIVisitorUploadDataInfo *info = gSessionService->CreateUploadData();
        info->ChatId(m_chatid);
        info->VisitorStaticId(m_cardStaticId);

        info->SecType("2012");
        info->ThirdType("2012001");
        info->AddExtsMap("status", val ? "1" : "2");
        info->AddExtsMap("userId", gSessionService->UserInfo()->UserId());
        UICommonDataModel::GetInstance()->UpdateVisitorUpdateData(info);
        gIProtocolAgent->DoVisitorUploadData(info);
    }
}

void UICardDataWidgetPanel::on_SummaryselecttoolButton_clicked()
{
    CommonSignalService::GetInstance()->DoOnChatSummaryBtnBtnClick(); //总结标签
}
