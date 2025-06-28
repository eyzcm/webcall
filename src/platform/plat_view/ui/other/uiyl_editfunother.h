#ifndef UIYL_EDITFUNOTHER_H
#define UIYL_EDITFUNOTHER_H


class UIYL_EditFunOther
{
public:
    UIYL_EditFunOther();

public:
    MChatItemData *CurrentChatItem();
    void Init();
    void InitRegisterSignal();
signals:
    void S_OnSendFile(MChatItemData *dataObj, QString fileName, EMESSAGETYPE msgType);

private slots:

    void R_OnJoinInBtnClick();
    void R_OnTransInBtnClick();

    void R_OnChatItemSelectChanged(MChatItemData *oldObj, MChatItemData *currObj);
    void R_OnSelectedChatItemData(MChatItemData *currObj);
    void R_SendAutoRobotReplyMessage(MChatItemData *objData, QString message, bool isJoin, QString actionId, CommInfo otherInfo);




private:

    bool IsCheckSendMessage(MChatItemData *item);
    bool IsCheckSendMessage(MMonitorChatData *item);

    MChatItemData         *m_currentItem;


};

#endif // UIYL_EDITFUNOTHER_H
