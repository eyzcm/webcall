#include "UICardItemData.h"
#include "control/CommonSignalService.h"
#include "service/DbUiWebcallService.h"
#include "model/UICommonDataModel.h"
#include "utils/utilshelper.h"
#include <QDateTime>

UICardItemData::UICardItemData(QWidget *parent) : QWidget(parent)
{
    m_required = 0;
    ui.setupUi(this);
    m_isLoadComboxData = false;
    connect(ui.comboBox_tag, &UI_SummaryTag_ComboBox_Widget::S_currentTextChanged, this, &UICardItemData::R_currentIndexChanged);
    connect(ui.m_searchCBox, &UISearchCBox::S_setText, this, &UICardItemData::R_setText);
    Init();
}

UICardItemData::~UICardItemData()
{
}

void UICardItemData::InitData(MCardColData *objData)
{
    m_ValueChange.insert("InitData", false);
    this->ColName(objData->ColName());
	this->Required(objData->Required());
    this->ColType(objData->ColType());
    this->Id(objData->Id());
    this->SelfText(objData->SelfText());

    this->Itemtype(objData->Itemtype());
    this->Index(objData->Index());
    Q_FOREACH (CommInfo *obj, objData->ItemList())
    {
        if (obj)
        {
            CommInfo *tempObj = new CommInfo(*obj);
            m_itemList.push_back(tempObj);
        }
    }


    QString strText;

    QLocale locale;
    if (locale.language() == QLocale::Chinese && gUISystemConfigManager->GetSystemConfigByKey("language", "0") == "0")
    {
        strText = SelfText();
    }
    else
    {
        if (Itemtype() == "1")
        {
            strText = SelfText();
        }
        else
        {
            strText = ColName();
        }
    }
    QString strElideText = GetElideText(strText, ui.m_label->font(), 60);

    ui.m_label->setText(strElideText);
    ui.m_label->setToolTip(strText);

    if (ColType() == "0")
    {
        if (ColName() == "sex")
        {
            ui.stackedWidget->setCurrentIndex(1);
            ui.m_comboBox->addItem(tr("男"), "0");
            ui.m_comboBox->addItem(tr("女"), "1");
            ui.m_comboBox->addItem(tr("未知"), "2");
            ui.m_comboBox->HideEdit();
            this->m_colType = "1";
        }
        else
        {
            ui.stackedWidget->setCurrentIndex(0);
            if (ColName() == "area")
            {
                ui.m_toolButton->setVisible(true);
                ui.m_toolButton->setToolTip(tr("手机号归属地区"));
            } else if(ColName() == "mobile")
            {
                ui.notes_label->show();
            }
            else
            {
                ui.m_toolButton->setVisible(false);
            }
        }
    }
    else if (ColType() == "1")
    {
        QString colName = objData->ColName().toUpper();
        if (gUIExtendConfigManager->IsCardInfoExtByKey(colName))
        {
            ui.stackedWidget->setCurrentIndex(4);
            QList<CommInfo *> cardInfoExtlist = UICommonDataModel::GetInstance()->GetCardInfoListByKey(colName);
            ui.m_searchCBox->Init(cardInfoExtlist);
            UICommonDataModel::GetInstance()->ClearCardInfoList(cardInfoExtlist);
        }
        else
        {
            LoadComboxItemData();
        }
    }
    else if (objData->ColType() == "2")
    {
        ui.stackedWidget->setCurrentIndex(2);
    }
    else if (objData->ColType() == "3")
    {
        ui.stackedWidget->setCurrentIndex(3);
        ui.m_dateTimeEdit->setDateTime(QDateTime::currentDateTime());
        ui.m_dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    }
    else if (objData->ColType() == "4")
    {
        LoadComboxItemData();
    }
    else if (objData->ColType() == "5")
    {
        if (ColName() == "ntag")
        {
            ui.comboBox_tag->InitSummaryTagList();
            ui.stackedWidget->setCurrentIndex(5);
        }
    }
    else if (objData->ColType() == "6")
    {
        if (objData->ItemList().size() > 0)
        {
            QString val = objData->ItemList().at(0)->GetValueByKey("itemName");
            if (val == "1")
            {
                ui.m_ylCheckBox->setWindowTitle("1");
                ui.m_ylCheckBox->setChecked(true);
            }
            else
                ui.m_ylCheckBox->setChecked(false);
        }
        ui.stackedWidget->setCurrentIndex(6);
    }
    else if (objData->ColType() == "7")
    {
        LoadComboxItemData(1);

    }
    else
    {
        Q_ASSERT(false);
    }
    m_ValueChange.insert("InitData", true);
}

void UICardItemData::ColType(QString val)
{
    m_colType = val;
}

QString UICardItemData::ColType() const
{
    return m_colType;
}

void UICardItemData::Id(QString val)
{
    m_id = val;
}

QString UICardItemData::Id() const
{
    return m_id;
}

void UICardItemData::ColName(QString val)
{
    m_colName = val;
}

QString UICardItemData::ColName() const
{
    return m_colName;
}


void UICardItemData::Required(int val)
{
    m_required = val;
}

int UICardItemData::Required() const
{
    return m_required;
}

void UICardItemData::SelfText(QString val)
{
    m_selfText = val;
}

QString UICardItemData::SelfText() const
{
    return m_selfText;
}

void UICardItemData::Itemtype(QString val)
{
    m_itemtype = val;
}

QString UICardItemData::Itemtype() const
{
    return m_itemtype;
}

QList<CommInfo *> UICardItemData::ItemList() const
{
    return m_itemList;
}

void UICardItemData::ItemList(QList<CommInfo *> val)
{
    m_itemList = val;
}

void UICardItemData::AddItem(const CommInfo &info)
{
    CommInfo *item = new CommInfo(info);
    m_itemList.append(item);
}
void UICardItemData::Index(int val)
{
    m_nIndex = val;
}

int UICardItemData::Index() const
{
    return m_nIndex;
}

void UICardItemData::Init()
{
    m_ValueChange.insert("Init", false);
    ui.notes_label->hide();
    ui.m_toolButton->setVisible(false);
    connect(ui.m_comboBox, SIGNAL(S_ShowPopup()), this, SLOT(R_ShowPopup()));
    connect(ui.m_mulComboBox, SIGNAL(S_ShowPopup()), this, SLOT(R_ShowMulPopup()));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_AddTagFinished(QString, QString)), this, SLOT(R_AddTagFinished(QString, QString)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_DoGetSummaryCategoryFinished(QString, QString, QString)), this, SLOT(R_OnInitSummaryTagList(QString, QString, QString)));
    ui.m_dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui.m_dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    m_ValueChange.insert("Init", true);
}

void UICardItemData::ClearContent()
{
    m_ValueChange.insert("ClearContent", false);
    ui.m_lineEdit->setText("");
    //if (this->m_colName == "mobile")
    {
        this->SetItemOrangeBorder(false);
    }

    ui.m_searchCBox->setText("");
    ui.m_textEdit->setText("");
    ui.m_ylCheckBox->setChecked(false);

    ui.m_dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    if (m_colName == "sex")
    {
        ui.m_comboBox->setCurrentIndex(2);
    }
    else if (m_colName == "ntag")
    {
        ui.comboBox_tag->ClearSearchText();
        ui.comboBox_tag->SetSelectItem("0");
    }
    else
    {
        ui.m_comboBox->setCurrentIndex(-1);
        ui.m_mulComboBox->setCurrentText("");
    }
    m_ValueChange.insert("ClearContent", true);
}

QString UICardItemData::GetValue()
{
    QString ret    = "";
    int     nIndex = ui.stackedWidget->currentIndex();

    if (nIndex == 0)
    {
        ret = ui.m_lineEdit->text().trimmed();
    }
    else if (nIndex == 1)
    {
        if (ColName() == "sex")
        {
            ret = QString::number(ui.m_comboBox->currentIndex());
        }
        else
        {
            if (ColType() == "1")
            {
                int                 row   = ui.m_comboBox->currentIndex();
                QStandardItemModel *model = (QStandardItemModel *)ui.m_comboBox->GetModel();
                if (model)
                {
                    QModelIndex    index = model->index(row, 0);
                    QStandardItem *item  = model->itemFromIndex(index);
                    if (item)
                    {
                        ret = item->toolTip();
                    }
                }
            }
            else if (ColType() == "4")
            {
                int                 row   = ui.m_comboBox->currentIndex();
                QStandardItemModel *model = (QStandardItemModel *)ui.m_comboBox->model();
                if (model)
                {
                    QModelIndex    index = model->index(row, 0);
                    QStandardItem *item  = model->itemFromIndex(index);
                    if (item)
                    {
                        ret = item->data().toString();
                    }
                }
            }
        }
    }
    else if (nIndex == 2)
    {
        ret = ui.m_textEdit->toPlainText().trimmed();
    }
    else if (nIndex == 3)
    {
        if (m_colName == "createTime" || m_colName == "editTime")
        {
            ui.m_dateTimeEdit->setDateTime(QDateTime::currentDateTime());
        }
        ret = ui.m_dateTimeEdit->text().trimmed();
    }
    else if (nIndex == 4)
    {
        ret = ui.m_searchCBox->text().trimmed();
    }
    else if (nIndex == 5)
    {
        ret = ui.comboBox_tag->GetCurrentItemId();
    }
    else if (nIndex == 6)
    {
        bool bret = ui.m_ylCheckBox->isChecked();
        if (bret)
        {
            ret = "1";
        }
        else
            ret = "0";
    }
    else if (nIndex == 7)
    {
        ret = ui.m_mulComboBox->currentText();
    }
    else
    {
        Q_ASSERT(false);
    }
    return ret;
}

QString UICardItemData::HideMobile(QString body, bool isHidePhone)
{
    QString ret    = body;
    QString mobile = UtilsHelper::GetMobileByString(body,gUIPersonalConfigManager->GetAutoMatchingRegular("mobileRegex"));
    if (!mobile.isEmpty())
    {
        if (isHidePhone)
        {
            QStringList strListHiddenPos = gUIPersonalConfigManager->MobileHiddenPos().split(",");
            if (strListHiddenPos.size() == 2)
            {
                int nStart = strListHiddenPos.at(0).toInt();
                int nEnd   = strListHiddenPos.at(1).toInt();
                if (nEnd > nStart)
                {
                    int     nEncryptionCount = nEnd - nStart + 1;
                    QString strReplace       = "";
                    for (int i = 0; i < nEncryptionCount; i++)
                    {
                        strReplace += "*";
                    }

                    QString strTempMobile = mobile;
                    strTempMobile         = strTempMobile.remove(QRegExp("\\s"));
                    QString strFormat     = QString("%1%2%3").arg(strTempMobile.left(nStart - 1)).arg(strReplace).arg(strTempMobile.right(12 - nStart - nEncryptionCount));

                    QList<int> listSpace;
                    for (int i = 0; i < mobile.size(); ++i)
                    {
                        if (mobile.at(i) == ' ')
                        {
                            listSpace.append(i);
                        }
                    }

                    foreach (int space, listSpace)
                    {
                        strFormat = QString("%1%2%3").arg(strFormat.left(space)).arg(" ").arg(strFormat.right(strFormat.size() - space));
                    }

                    ret.replace(mobile, strFormat);
                }
            }
        }
    }
    return ret;
}

void UICardItemData::SetValue(QString val, bool isHidePhone)
{
    m_ValueChange.insert("SetValue", false);
    int nIndex = ui.stackedWidget->currentIndex();

    if (nIndex == 0)
    {
        if (ColName() == "phoneStatus")
        {
            ui.notes_label->setText(val);
        }
        else
        {
			
            QString hideMobileStr = HideMobile(val, isHidePhone);
            ui.m_lineEdit->setText(hideMobileStr);
			
        }
    }
    else if (nIndex == 1)
    {
        if (ColName() == "sex")
        {
            if (val == "0")
            {
                ui.m_comboBox->setCurrentIndex(0);
            }
            else if (val == "1")
            {
                ui.m_comboBox->setCurrentIndex(1);
            }
            else
            {
                ui.m_comboBox->setCurrentIndex(2);
            }
        }
        else
        {
            if (ColType() == "1")
            {
                bool isOk  = false;
                int  count = ui.m_comboBox->count();
                for (int i = 0; i < count; i++)
                {
                    ui.m_comboBox->setCurrentIndex(i);

                    int                 row   = ui.m_comboBox->currentIndex();
                    QStandardItemModel *model = (QStandardItemModel *)ui.m_comboBox->model();
                    if (model)
                    {
                        QModelIndex    index = model->index(row, 0);
                        QStandardItem *item  = model->itemFromIndex(index);
                        if (item)
                        {
                            QString itemval = item->toolTip();
                            if (val == itemval)
                            {
                                isOk = true;
                                break;
                            }
                        }
                    }
                }
                if (!isOk)
                {
                    ui.m_comboBox->setCurrentIndex(0);
                }
            }
            else if (ColType() == "4")
            {
                bool isOk  = false;
                int  count = ui.m_comboBox->count();
                for (int i = 0; i < count; i++)
                {
                    ui.m_comboBox->setCurrentIndex(i);

                    int                 row   = ui.m_comboBox->currentIndex();
                    QStandardItemModel *model = (QStandardItemModel *)ui.m_comboBox->model();
                    if (model)
                    {
                        QModelIndex    index = model->index(row, 0);
                        QStandardItem *item  = model->itemFromIndex(index);
                        if (item)
                        {
                            QString itemval = item->data().toString();
                            if (val == itemval)
                            {
                                isOk = true;
                                break;
                            }
                        }
                    }
                }
                if (!isOk)
                {
                    ui.m_comboBox->setCurrentIndex(0);
                }
            }
        }
    }
    else if (nIndex == 2)
    {
        QString hideMobileStr = HideMobile(val, isHidePhone);
        ui.m_textEdit->setText(hideMobileStr);
    }
    else if (nIndex == 3)
    {
        if (m_colName == "createTime" || m_colName == "editTime")
        {
            ui.m_dateTimeEdit->setDateTime(QDateTime::currentDateTime());
        }
        else
        {
            ui.m_dateTimeEdit->setDateTime(QDateTime::fromString(val, "yyyy-MM-dd hh:mm:ss"));
        }
    }
    else if (nIndex == 4)
    {
        ui.m_searchCBox->setText(val);
    }
    else if (nIndex == 5)
    {
        if (ColType() == "5")
        {
            ui.comboBox_tag->SetSelectItem(val);
        }
    }
    else if (nIndex == 6)
    {
        if (val.isEmpty())
        {
            QString val = ui.m_ylCheckBox->windowTitle();
            if (val == "1")
                ui.m_ylCheckBox->setChecked(1);
            else
                ui.m_ylCheckBox->setChecked(0);
        }
        else
            ui.m_ylCheckBox->setChecked(val.toInt() == 1);
    }else if(nIndex == 7)
    {
        ui.m_mulComboBox->setCurrentText(val);
    }
    else
    {
        Q_ASSERT(false);
    }
    m_ValueChange.insert("SetValue", true);
}

void UICardItemData::SetphoneStatus(QString val)
{
    if (val.isEmpty())
    {
        val = tr("未检测");
    }
    ui.notes_label->setText(val);
}

QString UICardItemData::GetphoneStatus()
{
    return ui.notes_label->text();
}
void UICardItemData::SetItemHeight(int height)
{
    this->setFixedHeight(height);
}

void UICardItemData::SetItemRedBorder(bool red)
{
    int nIndex = ui.stackedWidget->currentIndex();
    if (nIndex == 0)
    {
        if (red)
        {
            ui.m_lineEdit->setStyleSheet("QLineEdit {border:1px solid #FF0000;color:#666666;}");
        }
        else
        {
            ui.m_lineEdit->setStyleSheet("QLineEdit {border:1px solid #dddddd;color:#666666;}");
        }
    }
}

void UICardItemData::SetItemOrangeBorder(bool orange)
{
    int nIndex = ui.stackedWidget->currentIndex();
	switch (nIndex)
	{
	case 1:
		if (orange)
		{
			ui.m_comboBox->setStyleSheet("UI_CardItem_ComboBox {border:1px solid #F1BD6F;color:#666666;}");
		}
		else
		{
			ui.m_comboBox->setStyleSheet("UI_CardItem_ComboBox {border:1px solid #dddddd;color:#666666;}");
		}
		break;
	case 2:
		if (orange)
		{
			
			ui.m_dateTimeEdit->setStyleSheet("QDateTimeEdit {border:1px solid #F1BD6F;color:#666666;}");
		}
		else
		{
			ui.m_dateTimeEdit->setStyleSheet("QDateTimeEdit {border:1px solid #dddddd;color:#666666;}");
		}
		break;

	case 3:
		if (orange)
		{
			
			ui.m_searchCBox->setStyleSheet("UISearchCBox {border:1px solid #F1BD6F;color:#666666;}");
		}
		else
		{
			ui.m_searchCBox->setStyleSheet("UISearchCBox {border:1px solid #dddddd;color:#666666;}");
		}
		break;
	case 4:
		if (orange)
		{
			ui.comboBox_tag->setStyleSheet("QWidget {border:1px solid #F1BD6F;color:#666666;}");
		}
		else
		{
			ui.comboBox_tag->setStyleSheet("QWidget {border:1px solid #dddddd;color:#666666;}");
		}
		break;
	case 5:
		if (orange)
		{
			
			ui.m_ylCheckBox->setStyleSheet("QCheckBox {border:1px solid #F1BD6F;color:#666666;}");
		}
		else
		{
			ui.m_ylCheckBox->setStyleSheet("QCheckBox {border:1px solid #dddddd;color:#666666;}");
		}
		break;
	default:
        if (orange)
        {
            ui.m_lineEdit->setStyleSheet("QLineEdit {border:1px solid #F1BD6F;color:#666666;}");
        }
        else
        {
            ui.m_lineEdit->setStyleSheet("QLineEdit {border:1px solid #dddddd;color:#666666;}");
        }
    }
}
QString UICardItemData::GetElideText(QString &text, QFont font, int maxWidth)
{
    QFontMetrics fontWidth(font);
    QString      elideNote = fontWidth.elidedText(text, Qt::ElideRight, maxWidth);
    return elideNote;
}

void UICardItemData::R_ShowPopup()
{
    m_ValueChange.insert("R_ShowPopup", false);
    LoadComboxItemData();
    m_ValueChange.insert("R_ShowPopup", true);
}


void UICardItemData::R_ShowMulPopup()
{
    m_ValueChange.insert("R_ShowMulPopup", false);
    LoadComboxItemData(1);
    m_ValueChange.insert("R_ShowMulPopup", true);
}


int UICardItemData::GetSortIndex(const QList<qlonglong> &sourceints, qlonglong idnum)
{
    int first = 0, last = sourceints.length() - 1, mid;
    int counter = 0;
    while (first <= last)
    {
        counter++;
        mid        = (first + last) / 2; //确定中间元素
        qlonglong midnum = sourceints.at(mid);
        if (midnum > idnum)
        {
            last = mid - 1; // mid已经交换过了,last往前移一位
        }
        else if (midnum < idnum)
        {
            first = mid + 1; // mid已经交换过了,first往后移一位
        }
        else //判断是否相等
        {
            return -1;
        }
    }
    if (first > last)
    {
        return first;
    }
    return 0;
}

void UICardItemData::LoadComboxItemData(int type )
{
    if (m_isLoadComboxData)
        return;

    m_isLoadComboxData = true;
    if (((ColType() == "1") || (ColType() == "4") || (ColType() == "7")) && (ColName() != "sex"))
    {
        QString colName = ColName().toUpper();
        if (!gUIExtendConfigManager->IsCardInfoExtByKey(colName))
        {
            ui.stackedWidget->setCurrentIndex((type==1)?7:1);

            int nIndex = (type==1)?ui.m_mulComboBox->currentIndex():ui.m_comboBox->currentIndex();
            (type==1)?ui.m_mulComboBox->clear():ui.m_comboBox->clear();

            QList<qlonglong>     intlist;
            QList<QString> varlist;

            QStandardItemModel *model = (type==1)?ui.m_mulComboBox->GetModel():ui.m_comboBox->GetModel();
            foreach (CommInfo *valItem, m_itemList)
            {
                if (valItem)
                {
                    int     nWidth           = this->width() - 160;
                    QString strItemText      = valItem->GetValueByKey("itemName");
                    QString strElideItemText = GetElideText(strItemText, (type==1)?ui.m_mulComboBox->font():ui.m_comboBox->font(), nWidth);

                    QString idnum     = valItem->GetValueByKey("id");

                    int     sortindex = GetSortIndex(intlist, idnum.toLongLong());

                    if (sortindex >= 0)
                    {
                        varlist.insert(sortindex, (type==1)?strItemText:strElideItemText);
                        intlist.insert(sortindex, idnum.toLongLong());
                    }


                    QStandardItem *item = new QStandardItem(strElideItemText);
                    if (item)
                    {
                        if ((ColType() == "4")||(ColType() == "7"))
                        {
                            item->setData(idnum);
                        }
                        item->setToolTip(strItemText);
                        model->insertRow(sortindex, item);
                    }
                }
            }
            (type==1)?ui.m_mulComboBox->setCurrentIndex(nIndex):ui.m_comboBox->setCurrentIndex(nIndex);
            (type==1)?ui.m_mulComboBox->SaveItemInfo():ui.m_comboBox->SaveItemInfo();
        }
    }
    else if (ColName() == "sex")
    {
        (type==1)?ui.m_mulComboBox->HideEdit():ui.m_comboBox->HideEdit();
    }
}

void UICardItemData::R_OnInitSummaryTagList(QString code, QString reason, QString body)
{
    if (ColType() == "5")
    {
        m_ValueChange.insert("R_OnInitSummaryTagList", false);
        ui.comboBox_tag->InitSummaryTagList();
        m_ValueChange.insert("R_OnInitSummaryTagList", true);
    }
}

void UICardItemData::R_AddTagFinished(QString id, QString uuid)
{
    if (ColType() == "5")
    {
        m_ValueChange.insert("R_AddTagFinished", false);
        ui.comboBox_tag->AddTagFinished(id, uuid);
        m_ValueChange.insert("R_AddTagFinished", true);
    }
}

bool UICardItemData::isaudoSave()
{
    QList<bool> list = m_ValueChange.values();
    Q_FOREACH (bool b, list)
    {
        if (!b)
        {
            return false;
        }
    }
    return true;
}

void UICardItemData::on_m_ylCheckBox_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    if (isaudoSave())
        Q_EMIT S_ValueChange(ColType());
}

void UICardItemData::R_setText()
{
    if (isaudoSave())
        Q_EMIT S_ValueChange(ColType());
}

void UICardItemData::on_m_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    Q_UNUSED(dateTime);
    if (isaudoSave())
        Q_EMIT S_ValueChange(ColType());
}

void UICardItemData::on_m_textEdit_textChanged()
{
    if (isaudoSave())
        Q_EMIT S_ValueChange(ColType());
}

void UICardItemData::on_m_lineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    if (ColName() != "editTime")
    {
        if (isaudoSave())
            Q_EMIT S_ValueChange(ColType());
    }
}

void UICardItemData::R_currentIndexChanged(const QString &v)
{
    Q_UNUSED(v);
    if (isaudoSave())
        Q_EMIT S_ValueChange(ColType());
}

void UICardItemData::on_m_comboBox_activated(int index)
{
    Q_UNUSED(index);
    if (isaudoSave())
        Q_EMIT S_ValueChange(ColType());
}
