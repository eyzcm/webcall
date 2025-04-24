#pragma once
#include "ui_UI_Panel_BtnItemSetting.h"
#include <QWidget>

class UI_Panel_BtnItemSetting : public QWidget
{
    Q_OBJECT

public:
    UI_Panel_BtnItemSetting(QWidget* parent = 0);
    ~UI_Panel_BtnItemSetting();

public:
    void    SetUUID(QString uuid);
    QString GetButtonString();
    QString GetButtonColor();
    void    SetButtonString(QString btnName);
    void    SetButtonColor(QString color);
    void    SetItemRedBorder(bool red);

signals:
    void S_OnRemoveBtnClick(QString uuid);
    void S_ShowTipMessage(QString msg);
private slots:
    void R_OnRemoveBtnClick();
    void R_OnTextChanged(const QString& txt);

private:
    Ui::UI_Panel_BtnItemSetting ui;
    QMap<int, QString>          m_colorMap;
    QString                     m_id;
    bool                        m_isRed;
};
