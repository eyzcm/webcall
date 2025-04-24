#pragma once

#include <QMap>
#include <QWidget>

#include "datafactory/ConstructionData.h"
#include "UICardItemData.h"
#include "utils/UIObserver.h"
#include "UIObserverImp.h"
#include "ui/common/CommonStruct.h"
#include "data/MCardColData.h"
#include "data/MChatItemData.h"
#include "data/MMonitorVisitorData.h"
#include "data/MUICardInfoData.h"
#include "ui_UICardDataWidgetPanel.h"

class WheelEventFilter : public QObject {
    Q_OBJECT

protected:
    bool eventFilter(QObject *obj, QEvent *event) override {
        if (event->type() == QEvent::Wheel) {
            return true;  // 拦截滚轮事件
        }
        return QObject::eventFilter(obj, event);
    }
};

class NoScrollComboBox : public QComboBox {
    Q_OBJECT

public:
    NoScrollComboBox(QWidget *parent = nullptr) : QComboBox(parent) {}

protected:
    void wheelEvent(QWheelEvent *event) override {
        // 忽略滚轮事件，不做处理
        event->ignore();
    }
};

class UICardDataWidgetPanel : public QWidget, public IUIObserverImp
{

    Q_OBJECT

public:
    UICardDataWidgetPanel(QWidget* parent = 0);
    ~UICardDataWidgetPanel();

public:
    void Init();

public:
    virtual void RemoveUpdate(QString type, QString body);
    virtual void Update(QString type, QObject* obj);

private slots:
    // CZC 接收是否显示“新建名片标签”按钮
    void R_OtherSettingChanged();


public:
    void OtherSettingChanged();
    void UpdateThirdFormData();
    void showEchowidget(bool show);

signals:
    void S_OnSaveVisitorCardBtnClick(E_TABTYPE type, bool isNew, const MUICardInfoData& data,QString isAuto);
    void S_TempCacheMCardInfo(E_TABTYPE type, const MUICardInfoData& data);

private:
    void InitExtendWidget();
    void AddCardCol(MCardColData* objData);
    void ClearCardCol();
    void ClearCardInfo();
    void InitDataEnd();
    void ShowExtendCardInfo(MChatItemData* objData);
    void ShowExtendCardInfo(MMonitorChatData* objData);
    void ShowExtendCardInfo(MMonitorVisitorData* objData);
    void ShowExtendcardInfoByChatId(QString chatId);
    void ClearExtendCardInfo();
    void ShowCardInfo(MUICardInfoData* objData);
    void UpdateCardInfo(MUICardInfoData* objData);
    void SetControlValueByMCardInfo(MUICardInfoData* objData);
    void TempCacheMcardInfo();

    QString GetControlValue(UIControl& control);
    QString GetExtendControlValue(UIControl& control);
    void    SetControlValue(UIControl& control, QString& val);
    void    SetExtendControlValue(UIControl& control, QString& val);

    void    SaveVisitorCard(bool isNew, QString isAuto = "0");
    int    UpdateCardInfoData(MUICardInfoData& cardData, int savetype = 0); //savetype为1表示手动保存

    //【曹振超】判断原始数据和界面数据是否一致
    bool IsSame_CompareSourceAndUIData(MUICardInfoData* data);
    bool IsFilter_CompareCardData(QString strColName, QString strColText, QString strColContent, QString strColType, QMap<QString, QString> mapCardInfo);

    //【曹振超】判断字符串是否满足正则表达式
    QValidator::State CompareString_ByRegExp(QString& str);

private slots:
    void OnAutoSaveCard(bool val);
    void OnEditClicked();
    void R_OnSaveVisitorCardBtnClick();
    void R_OnNewCardSaveVisitorCardBtnClick();
    void R_OnSearchBtnClick();
    void R_CurrentTabPageChanged(E_TABTYPE oldType, E_TABTYPE curType);
    void R_SaveVisitorCard(bool isNew);
    void R_GetCompanyGlobalConfResult(QString code, QString reason, QString body);
    void R_GetUserPermissionListResult(QString code, QString reason, QString body);
    void R_ValueChange(QString CType);
    void R_timeout();
    void on_SummaryselecttoolButton_clicked();
    void R_OnEchoSummary(QString chatId);

private:
    Ui::UICardDataWidgetPanel ui;

    QMap<QString, UIControl>       m_orderCardInfoList;
    QMap<QString, UICardItemData*> m_uiCardItemDataList;

    QString m_cardStaticId;
    QString m_chatid;
    QString m_visitorId;
    QString m_ip;
    QString m_ipv6;
    QTimer* m_timer;
};
