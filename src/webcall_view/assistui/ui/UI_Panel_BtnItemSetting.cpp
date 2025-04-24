#include "UI_Panel_BtnItemSetting.h"
#include <QListView>

UI_Panel_BtnItemSetting::UI_Panel_BtnItemSetting(QWidget *parent)
{
    ui.setupUi(this);

    ui.comboBox->setView(new QListView());
    m_id    = "";
    m_isRed = false;
    connect(ui.m_removeBtn, SIGNAL(clicked()), this, SLOT(R_OnRemoveBtnClick()));
    connect(ui.lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(R_OnTextChanged(const QString &)));
    m_colorMap.insert(0, "#ff761f");
    m_colorMap.insert(1, "#ffc158");
    m_colorMap.insert(2, "#60e27e");
    m_colorMap.insert(3, "#31e2d0");
    m_colorMap.insert(4, "#ff6b6b");
    m_colorMap.insert(5, "#fc0039");
    m_colorMap.insert(6, "#ff6caa");
    m_colorMap.insert(7, "#c48ff9");
    m_colorMap.insert(8, "#58befc");
    m_colorMap.insert(9, "#58befc");
    m_colorMap.insert(10, "#00a3a4");
    m_colorMap.insert(11, "#2e86de");
    m_colorMap.insert(12, "#c05e7d");
    m_colorMap.insert(13, "#4296ff");
    m_colorMap.insert(14, "#0cc0d5");
    SetItemRedBorder(false);
}

UI_Panel_BtnItemSetting::~UI_Panel_BtnItemSetting()
{
}

void UI_Panel_BtnItemSetting::R_OnRemoveBtnClick()
{
    Q_EMIT S_OnRemoveBtnClick(m_id);
}

QString UI_Panel_BtnItemSetting::GetButtonString()
{
    return ui.lineEdit->text().trimmed();
}

QString UI_Panel_BtnItemSetting::GetButtonColor()
{
    int nIndex = ui.comboBox->currentIndex();
    return m_colorMap[nIndex];
}

void UI_Panel_BtnItemSetting::SetUUID(QString uuid)
{
    m_id = uuid;
}

void UI_Panel_BtnItemSetting::SetButtonString(QString btnName)
{
    ui.lineEdit->setText(btnName);
}

void UI_Panel_BtnItemSetting::SetButtonColor(QString color)
{
    int n = 0;
    for (auto itor = m_colorMap.begin(); itor != m_colorMap.end(); itor++)
    {
        if (itor.value() == color)
        {
            break;
        }
        n++;
    }
    ui.comboBox->setCurrentIndex(n);
}

void UI_Panel_BtnItemSetting::R_OnTextChanged(const QString &txt)
{
    if (m_isRed)
    {
        SetItemRedBorder(false);
    }

    if (txt.length() > 10)
    {
        ui.lineEdit->setText(txt.left(10));
        QString msg = tr("按钮文案不能超过10个字符。");
        Q_EMIT S_ShowTipMessage(msg);
    }
}

void UI_Panel_BtnItemSetting::SetItemRedBorder(bool red)
{
    m_isRed = red;
    if (red)
    {
        ui.lineEdit->setStyleSheet("QLineEdit {border:1px solid #FF0000;color:#666666;}");
    }
    else
    {
        ui.lineEdit->setStyleSheet("QLineEdit {border:1px solid #dddddd;color:#666666;}");
    }
}
