#pragma once

#include "data/MCardColData.h"
#include "ui_UICardItemData.h"
#include <QWidget>

class UICardItemData : public QWidget
{
    Q_OBJECT

public:
    UICardItemData(QWidget *parent = 0);
    ~UICardItemData();
    void    Init();
    void    InitData(MCardColData *objData);
    void    ClearContent();
    QString GetValue();
    void    SetValue(QString val, bool isHidePhone);
    void    SetphoneStatus(QString val);
    QString GetphoneStatus();

    // CZC:设置条目高度
    void SetItemHeight(int height);

    // CZC：设置红框显示
    void SetItemRedBorder(bool red);
    void SetItemOrangeBorder(bool orange);

public:
    QString ColType() const;
    void    ColType(QString val);

    QString Id() const;
    void    Id(QString val);

    void Required(int val);
    int Required() const;

    QString ColName() const;
    void    ColName(QString val);

    QString SelfText() const;
    void    SelfText(QString val);

    QString Itemtype() const;
    void    Itemtype(QString val);

    int  Index() const;
    void Index(int val);

    QList<CommInfo *> ItemList() const;
    void              ItemList(QList<CommInfo *> val);
    void              AddItem(const CommInfo &info);

    QString GetElideText(QString &text, QFont font, int maxWidth);

private slots:
    void R_ShowPopup();
    void R_ShowMulPopup();
    void R_OnInitSummaryTagList(QString code, QString reason, QString body);
    void R_AddTagFinished(QString id, QString uuid);
    void on_m_ylCheckBox_stateChanged(int arg1);
    void R_setText();
    void on_m_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime);
    void on_m_textEdit_textChanged();
    void on_m_lineEdit_textChanged(const QString &arg1);
    void R_currentIndexChanged(const QString &v);
    void on_m_comboBox_activated(int index);

signals:
    void S_ValueChange(QString CType);

private:
    int     GetSortIndex(const QList<qlonglong> &sourceints, qlonglong i);
    QString HideMobile(QString body, bool isHidePhone);
    void    LoadComboxItemData(int type = 0);
    bool    isaudoSave();

    private:
        Ui::UICardItemData ui;

        int                 m_required;
        QString             m_colType;
        QString             m_id;
        QString             m_colName;
        QString             m_selfText;
        QString             m_itemtype;
        QList<CommInfo *>   m_itemList;
        int                 m_nIndex;
        bool                m_isLoadComboxData;
        QMap<QString, bool> m_ValueChange; //手动修改值时才触发自动保存名片
};
