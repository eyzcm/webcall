#pragma once
#include "model/UIBaseChatTreeModel.h"
#include "utils/UISingleton.h"
#include <QObject>

#include "plat_model_global.h"
class PLAT_MODEL_EXPORT UIChatTreeModel : public UIBaseChatTreeModel, public UISingleton<UIChatTreeModel>
{
    Q_OBJECT
    friend class UISingleton<UIChatTreeModel>;

private:
    UIChatTreeModel(void);
    virtual ~UIChatTreeModel(void);

public:
    void Init();

private slots:
    void R_OnContactSingleClick(MContactItemData* item);
    void R_OnContactDoubleClick(MContactItemData* item);
    void R_OnCommonTreeMenuClick(QString type, MContactItemData* item);

    void R_OnClickChatItemData(MChatItemData* obj, EUINODEMODELTYPE nodeType);
    void R_OnBeginDoubleClickChatItemData(MChatItemData* obj, EUINODEMODELTYPE nodeType);

private:
    void AddQueueVisitor(MContactItemData* item);
    void AddChatVisitor(MContactItemData* item);
    void AddMonitorVisitor(MContactItemData* item);
    void AddContactInnerChat(MContactItemData* item);
    void UpdateChatItem(MContactItemData* item);

    void AddQueueVisitor(MChatItemData* item);
    void AddChatVisitor(MChatItemData* item);
    void AddMonitorVisitor(MChatItemData* item);
    void AddContactInnerChat(MChatItemData* item);
    void UpdateChatItem(MChatItemData* item, EUINODEMODELTYPE noteType);

    MChatItemData* CreateMChatItemData(const MContactItemData& obj, EUSERTYPE eType, ECHATITEMSTATE eState);
};
