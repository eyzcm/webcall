#include "el_commonword_popup_ui.h"
#include "utils/UICommUtilsHelper.h"
#include "UI_TreeViewItemModel_CommonWord.h"
#include "UI_Widget_SystemPrompt.h"
#include "control/CommonSignalService.h"
#include "control/geasyliaoframework.h"
#include "data\YLData\YL_TreeViewItemLeafNodeData_CommonWord.h"
#include "data\YLData\YL_TreeViewItemNodeData_CommonWord.h"
#include "service/DbUiwebcallService.h"
#include "utils/jsonhelper.h"
#include "model\YL_Model\UI_Model_CommonWord.h"
#include "protocol_biz/webcallprotocolservice.h"
#include "ui_el_commonword_popup_ui.h"
#include "data/UIShowSettingInfo.h"
#include "utils/utilshelper.h"

#include <model/YL_Model/UI_Model_CommonWord.h>

EL_CommonWord_Popup_UI::EL_CommonWord_Popup_UI(QWidget *parent) : MyWidgetBase(parent), ui(new Ui::EL_CommonWord_Popup_UI)
{
    ui->setupUi(this);
    SetDragEnable(false);
    SetShadowEnable(true);
    SetMoveWidgetHeight(50);

    m_webView    = nullptr;
    m_jshelper   = nullptr;
    m_item       = nullptr;
    m_gItemModel = nullptr;
    m_info       = nullptr;
    ui->Shortcutkey->installEventFilter(this);
    connect(ui->content, &el_TextEdit::S_GetInputValue, this, &EL_CommonWord_Popup_UI::R_GetInputValue);

    //添加常常用语
    connect(this, SIGNAL(S_AddCommonWord(YL_TreeViewItem_CommonWord *, UICommonWordInfo *)), CommonSignalService::GetInstance(), SIGNAL(S_CommonWord_AddCommonWord(YL_TreeViewItem_CommonWord *, UICommonWordInfo *)));
    //修改常用语
    connect(this, SIGNAL(S_ModifyCommonWord(YL_TreeViewItem_CommonWord *, UICommonWordInfo *)), CommonSignalService::GetInstance(), SIGNAL(S_CommonWord_ModifyCommonWord(YL_TreeViewItem_CommonWord *, UICommonWordInfo *)));
    //添加分组
    connect(this, SIGNAL(S_AddCommonWordDir(YL_TreeViewItem_CommonWord *, UICommonWordInfo *)), CommonSignalService::GetInstance(), SIGNAL(S_CommonWord_AddCommonWord(YL_TreeViewItem_CommonWord *, UICommonWordInfo *)));
    //修改分组
    connect(this, SIGNAL(S_ModifyCommonWordDir(YL_TreeViewItem_CommonWord *)), CommonSignalService::GetInstance(), SIGNAL(S_CommonWord_ModifyCommonWordDir(YL_TreeViewItem_CommonWord *)));
    //添加常用语完成
    connect(CommonSignalService::GetInstance(), &CommonSignalService::S_addCommonWordFinished, this, &EL_CommonWord_Popup_UI::R_addCommonWordFinished);
}

EL_CommonWord_Popup_UI::~EL_CommonWord_Popup_UI()
{
    delete ui;
    if (m_jshelper)
    {
        m_jshelper->deleteLater();
        delete m_jshelper;
    }
    if (m_webView)
    {
        m_webView->deleteLater();
        delete m_webView;
    }
}

void EL_CommonWord_Popup_UI::setTitle(QString title)
{
    ui->label_title->setText(title);
}

void EL_CommonWord_Popup_UI::Init(YL_TreeViewItem_CommonWord *item, EYLUI_CtrlName type, UI_TreeViewItemModel_CommonWord *gItemModel, bool haveGropu)
{
    setGrouphide(haveGropu); //是否显示分组

    m_item       = item;
    m_type       = type;
    m_gItemModel = gItemModel;

    if (m_type == Enum_YL_CtrlName_AddCommonWord) //添加常用语时,初始化
    {
        setTitle(tr("添加常用语"));
        QString pid;

        YL_TreeViewItemLeafNodeData_CommonWord *itemData = (YL_TreeViewItemLeafNodeData_CommonWord *)m_item->Ptr();
        if (itemData)
        {
            pid = itemData->Pid();
        }
        setGroupList(m_gItemModel, pid);
        if (UI_Model_CommonWord::GetInstance()->CurrentUserId() == "company") //公司下常用语,不做快捷键显示
        {
            ui->label_Shortcutkey->setVisible(false);
            ui->Shortcutkey->setVisible(false);
        }
        else
        {
            ui->label_Shortcutkey->setVisible(true);
            ui->Shortcutkey->setVisible(true);
        }

    }
    else if (m_type == Enum_YL_CtrlName_ModifyCommonWord) //修改常用语时,初始化
    {
        setTitle(tr("修改常用语"));
        QString                                 pid;
        YL_TreeViewItemLeafNodeData_CommonWord *itemData = (YL_TreeViewItemLeafNodeData_CommonWord *)m_item->Ptr();
        if (itemData)
        {
            ui->title->setText(itemData->Title());
            ui->Shortcutkey->setText(itemData->HotKey());
            ui->content->SetInputTxt(itemData->Content());
            pid = itemData->Pid();
        }

        if (UI_Model_CommonWord::GetInstance()->CurrentUserId() == "company") //公司下常用语,不做快捷键显示
               {
                   ui->label_Shortcutkey->setVisible(false);
                   ui->Shortcutkey->setVisible(false);
               }
               else
               {
                   ui->label_Shortcutkey->setVisible(true);
                   ui->Shortcutkey->setVisible(true);
               }


        // setGroupList(m_gItemModel, pid); //不支持修改分组
    }
    else if (m_type == Enum_YL_CtrlName_AddCommonWordGroup) //添加分组时,初始化
    {
        setTitle(tr("添加分组"));
    }
    else if (m_type == Enum_YL_CtrlName_ModifyCommonWordGroup) //修改分组时,初始化
    {
        setTitle(tr("修改分组"));
        YL_TreeViewItemNodeData_CommonWord *itemData = (YL_TreeViewItemNodeData_CommonWord *)m_item->Ptr();
        if (itemData)
        {
            ui->groupContent->setText(itemData->Title());
        }
    }
    else if (m_type == Enum_YL_CtrlName_BatchImport || m_type == Enum_YL_CtrlName_BatchExport) //导入导出
    {
        setTitle(tr("常用语导入/导出"));
        m_webView = new UI_WebView_ShieldBackSpace(this);
        ui->page_Import_export_layout->addWidget(m_webView);
        int showType = 0;
        if (m_type == Enum_YL_CtrlName_BatchExport)
        {
            showType = 1;
        }
        m_jshelper = new EL_CommonWord_JsHelper(showType);
        connect(m_jshelper, &EL_CommonWord_JsHelper::S_closeWnd, this, &EL_CommonWord_Popup_UI::on_m_closeBtn_clicked);
        m_jshelper->SetWebView(m_webView, "jsobj");
        m_jshelper->SetUrl(gEasyLiaoFramework->GetHtmlNameByKey("commonWord"));
    }
    reSetSize(m_type);
}

void EL_CommonWord_Popup_UI::setGroupList(UI_TreeViewItemModel_CommonWord *model, QString selecID)
{
    QVector<Struct_UI_SummaryTagItem_Data> vecItemData;

    QVector<YL_TreeViewItem_CommonWord *> childItemList = model->root()->ChildVecItem();
    for (int i = 0; i < childItemList.count(); ++i)
    {
        YL_TreeViewItem_CommonWord *childItem = childItemList.at(i);
        if (!childItem->IsLeaf())
        {
            YL_TreeViewItemNodeData_CommonWord *itemNode = (YL_TreeViewItemNodeData_CommonWord *)childItem->Ptr();
            if (itemNode)
            {
                Struct_UI_SummaryTagItem_Data itemData;
                itemData.m_strColor     = "#ffffff";
                itemData.m_strIndexSort = QString::number(i);
                itemData.m_strTitle     = itemNode->Title();
                itemData.m_strContent   = itemNode->Title();
                itemData.m_strId        = itemNode->Id();
                itemData.m_strType      = "0";
                vecItemData.push_back(itemData);
            }
        }
    }
    //只有在添加,修改常用语时,才需要相应 R_currentIndexChanged 来重新确认当前常用语的Pid
    disconnect(ui->group, SIGNAL(currentIndexChanged(int)), this, SLOT(R_currentIndexChanged(int)));
    connect(ui->group, SIGNAL(currentIndexChanged(int)), this, SLOT(R_currentIndexChanged(int)));
    ui->group->InitItemData(vecItemData);
    if (!selecID.isEmpty()) //默认选择分组内容
    {
        ui->group->SetSelectItem(selecID);
    }
    else
    {
        if (vecItemData.count() > 0)
        {
            ui->group->SetSelectItem(vecItemData.last().m_strId);
        }
    }
}

void EL_CommonWord_Popup_UI::reSetSize(EYLUI_CtrlName t) //设置窗口大小
{
    QSize size;
    if (t == Enum_YL_CtrlName_ModifyCommonWord || t == Enum_YL_CtrlName_AddCommonWord) //常用语相关
    {
        size = QSize(580, 560);
        ui->stackedWidget->setCurrentWidget(ui->page_word);
        ui->content->setFocus();
    }
    else if (t == Enum_YL_CtrlName_AddCommonWordGroup || t == Enum_YL_CtrlName_ModifyCommonWordGroup) //分组相关
    {
        size = QSize(520, 220);
        ui->stackedWidget->setCurrentWidget(ui->page_group);
        ui->groupContent->setFocus();
    }
    else if (t == Enum_YL_CtrlName_BatchImport || t == Enum_YL_CtrlName_BatchExport) //导入导出相关
    {
        size = QSize(670, 525);
        ui->stackedWidget->setCurrentWidget(ui->page_Import_export);
    }
    else
    {
        return;
    }
    setMinimumSize(size);
    setMaximumSize(size);
    resize(size);
}

void EL_CommonWord_Popup_UI::setGrouphide(bool ishide)
{
    ui->label_group->setVisible(ishide);
    ui->group->setVisible(ishide);
    ui->addGroup->setVisible(ishide);
}

void EL_CommonWord_Popup_UI::on_m_closeBtn_clicked() //关闭按钮响应
{
    close();
    deleteLater(); //释放内存
}

void EL_CommonWord_Popup_UI::on_saveCommonWord_clicked() //保存按键响应
{
    if (ui->stackedWidget->currentWidget() == ui->page_word)
    {
        if (ui->title->text().isEmpty())
        {
            showToastMsg(tr("常用语标题不能为空!"));
        }
        else if (ui->group->isVisible() && ui->group->GetCurrentItemData().m_strContent.isEmpty())
        {
            showToastMsg(tr("常用语分组不能为空!"));
        }
        else
        {
            ui->content->GetInputTxt(0); //获取常用语内容
        }
    }
    else if (ui->stackedWidget->currentWidget() == ui->page_group)
    {
        if (ui->groupContent->text().isEmpty())
        {
            showToastMsg(tr("分组名称不能为空!"));
        }
        else if (ui->groupContent->text().trimmed().isEmpty())
        {
            showToastMsg(tr("分组不能只有空格!"));
        }
        else if (m_type == Enum_YL_CtrlName_AddCommonWordGroup)
        {
            m_info = gUICommonWordManager->CreateInfo();
            m_info->Title(ui->groupContent->text());
            m_info->IsDir(true);
            m_info->Id(UtilsHelper::GetUUid());
            m_info->Pid(m_item->ID());
            m_info->CurrentUserId(UI_Model_CommonWord::GetInstance()->CurrentUserId());
            gIProtocolAgent->DoaddCommonWord(m_info); //调用接口
        }
        else if (m_type == Enum_YL_CtrlName_ModifyCommonWordGroup)
        {
            YL_TreeViewItemNodeData_CommonWord *item = (YL_TreeViewItemNodeData_CommonWord *)m_item->Ptr();
            item->Title(ui->groupContent->text());

            Q_EMIT S_ModifyCommonWordDir(m_item);
            //关闭窗口
            on_m_closeBtn_clicked();
        }
    }
}

void EL_CommonWord_Popup_UI::showToastMsg(QString msg)
{
    UI_Widget_SystemPrompt *pWidget_SystemPtompt = new UI_Widget_SystemPrompt(this);
    pWidget_SystemPtompt->SetMsg(msg, Enum_YL_Toast_Type_Warning);
    pWidget_SystemPtompt->SetShowTime(2);
    pWidget_SystemPtompt->SetShowPos(this, Enum_YL_Toast_Parent_CenterCenter, this->mapFromGlobal(QPoint(0, 0)));
    pWidget_SystemPtompt->show();
}

void EL_CommonWord_Popup_UI::R_GetInputValue(QString str, int i)
{
    Q_UNUSED(i);
    if (str.isEmpty())
    {
        showToastMsg(tr("常用语内容不能为空!"));
        return;
    }
    else if (str.trimmed().isEmpty())
    {
        showToastMsg(tr("常用语内容不能只有空格!"));
        return;
    }
    else if (str.count() > 2000)
    {
        showToastMsg(tr("内容富文本字符不能超过2000个，请调整后保存"));
        return;
    }
    else if (str.count() > 500)
    {
        QString tstr;
        tstr = UtilsHelper::ReplaceHtmlTag(str);
        tstr = tstr.replace("&nbsp;", "");
        tstr = tstr.trimmed();
        if (tstr.count() > 500)
        {
            showToastMsg(tr("内容字符不能超过500个，请调整后保存"));
            return;
        }
    }

    QString hotkey = ui->Shortcutkey->text().trimmed();
    m_info         = gUICommonWordManager->CreateInfo();
    m_info->Content(str);
    m_info->Title(ui->title->text());
    m_info->HotKey(hotkey);
    if (m_type == Enum_YL_CtrlName_AddCommonWord)
    {
        QString uuid = UtilsHelper::GetUUid();
        m_info->Id(uuid);
        m_info->IsDir(false);
        m_info->Pid(m_item->ID());
        m_info->CurrentUserId(UI_Model_CommonWord::GetInstance()->CurrentUserId());
        gIProtocolAgent->DoaddCommonWord(m_info); //调用接口
    }
    else if (m_type == Enum_YL_CtrlName_ModifyCommonWord)
    {
        Q_EMIT S_ModifyCommonWord(m_item, m_info);
        //关闭窗口
        on_m_closeBtn_clicked();
    }
}
//是否与个人配置中快捷键冲突
bool EL_CommonWord_Popup_UI::Is_HotkeyAlreadyExists(QString key)
{
    bool hasKey = false;

    UIShowSettingInfo *showInfo = gUIPersonalConfigYLService->ShowSettingInfo();
    if (showInfo)
    {
        key                         = key.remove(QRegExp("\\s")).toLower();
        QString strScreenShot       = showInfo->KeyScreenShot().remove(QRegExp("\\s")).toLower();
        QString strCommonWordSearch = showInfo->KeyCommonWordSearch().remove(QRegExp("\\s")).toLower();
        QString strOpenCard         = showInfo->KeyOpenCard().remove(QRegExp("\\s")).toLower();
        QString strActiveMainWnd    = showInfo->KeyActiveMainWnd().remove(QRegExp("\\s")).toLower();
        QString strChatChange       = showInfo->KeyChatChange().remove(QRegExp("\\s")).toLower();
        QString strSendImage        = "ctrl+shift+v";

        if ((key == strScreenShot) || (key == strCommonWordSearch) || (key == strOpenCard) || (key == strActiveMainWnd) || (key == strChatChange) || (key == strSendImage))
        {
            hasKey = true;
        }

        //清理指针
        delete showInfo;
        showInfo = nullptr;
    }

    return hasKey;
}

bool EL_CommonWord_Popup_UI::eventFilter(QObject *obj, QEvent *e)
{
	if (ui->Shortcutkey->hasFocus() && (e->type() == QEvent::KeyPress || e->type() == QEvent::InputMethod))
	{
		QString    oldstr = ui->Shortcutkey->text();
		QKeyEvent *event = static_cast<QKeyEvent *>(e);
		if (((event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier) || ((event->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier) || ((event->modifiers() & Qt::AltModifier) == Qt::AltModifier))
		{
			if (((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_A)) || ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_S)) || ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_X))
				|| ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_C)) || ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_V)) || ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_Z))
				|| ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_B)))
			{
				showToastMsg(tr("无法设置系统快捷键,请重新设置!"));
				return true;
			}

			QString ret = "";
			if (event->modifiers() & Qt::ControlModifier)
			{
				ret = "Ctrl";
				if (event->modifiers() & Qt::AltModifier)
				{
					ret = ret + " + Alt";
					if (event->modifiers() & Qt::ShiftModifier)
					{
						ret = ret + " + Shift";
					}
				}
				else
				{
					if (event->modifiers() & Qt::ShiftModifier)
					{
						ret = ret + " + Shift";
					}
				}
			}
			else
			{
				if (event->modifiers() & Qt::AltModifier)
				{
					ret = "Alt";
					if (event->modifiers() & Qt::ShiftModifier)
					{
						ret = ret + " + Shift";
					}
				}
				else
				{
					ret = "Shift";
				}
			}

			int nVal = event->key();
			if ((ret.startsWith("Ctrl") || ret.startsWith("Alt") || ret.startsWith("Shift")) && ((nVal >= 0x30 && nVal <= 0x39) || (nVal >= 0x40 && nVal <= 0x5a) || (nVal >= 0x60 && nVal <= 0x69) || (nVal >= 0x01000030 && nVal <= 0x0100003b)))
			{
				ret = ret + " + " + char(nVal);
				QString                                 selfHotKey;
				YL_TreeViewItemLeafNodeData_CommonWord *itemData = (YL_TreeViewItemLeafNodeData_CommonWord *)m_item->Ptr();
				if (itemData)
				{
					selfHotKey = itemData->HotKey();
				}
				if (gUICommonWordManager->FindHotkey(ret) && selfHotKey != ret)
				{
					ui->Shortcutkey->setText(oldstr);
					showToastMsg(tr("热键已存在，请重新输入!"));
				}
				else if (Is_HotkeyAlreadyExists(ret))
				{
					ui->Shortcutkey->setText(oldstr);
					showToastMsg(tr("热键已存在，请重新输入!"));
				}
				else
				{
					ui->Shortcutkey->setText(ret);
				}
				return true;
			}
			else
			{
				ui->Shortcutkey->setText("");
				return true;
			}
		}
		else
		{
			showToastMsg(tr("输入框禁止编辑!"));
			return true;
		}
	}
	return QWidget::eventFilter(obj, e);
}

void EL_CommonWord_Popup_UI::on_addGroup_clicked() //添加分组
{
    EL_CommonWord_Popup_UI *w = new EL_CommonWord_Popup_UI(0);
    w->Init(m_item, Enum_YL_CtrlName_AddCommonWordGroup, m_gItemModel);
    w->ShowCenter_Model();
    //添加分组完成
    disconnect(CommonSignalService::GetInstance(), &CommonSignalService::S_AddCommonWordcomplete, this, &EL_CommonWord_Popup_UI::R_AddCommonWordcomplete);
    connect(CommonSignalService::GetInstance(), &CommonSignalService::S_AddCommonWordcomplete, this, &EL_CommonWord_Popup_UI::R_AddCommonWordcomplete);
}

void EL_CommonWord_Popup_UI::R_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    YL_TreeViewItem_CommonWord *item = m_gItemModel->GetTreeViewItemById(ui->group->GetCurrentItemData().m_strId);
    if (item)
    {
        m_item = item;
    }
}

void EL_CommonWord_Popup_UI::R_AddCommonWordcomplete()
{
    setGroupList(m_gItemModel);
}

void EL_CommonWord_Popup_UI::R_addCommonWordFinished(QString code, QString reason, QString body)
{
    Q_UNUSED(code);
    Json::Value val;
    bool        isOk = JsonHelper::GetJsonValueFromBody(body, val);
    if (isOk)
    {
        if (val.isObject())
        {
            QString succeed = JsonHelper::GetValueByKey("succeed", val);
            if (succeed == "1")
            {
                Json::Value data = val["data"];
                if (isOk && data.isObject())
                {
                    QString oldid = reason;
                    QString newid = JsonHelper::GetValueByKey("id", data);
                    //更新常用语id
                    if (m_info && m_info->Id() == oldid)
                    {
                        m_info->Id(newid);
                        if (m_info->IsDir())
                        {
                            Q_EMIT S_AddCommonWordDir(m_item, m_info);
                        }
                        else
                        {
                            Q_EMIT S_AddCommonWord(m_item, m_info);
                        }
                        //关闭窗口
                        on_m_closeBtn_clicked();
                    }
                }
            }
            else
            {
                showToastMsg(tr("常用语新增失败! 原因 :") + reason);
            }
        }
    }
}

void EL_CommonWord_Popup_UI::on_groupContent_textChanged(const QString &arg1)
{
    ui->groupContentnum->setText(QString("%1/20").arg(arg1.count()));
}

void EL_CommonWord_Popup_UI::on_title_textChanged(const QString &arg1)
{
    ui->titlenum->setText(QString("%1/50").arg(arg1.count()));
}
