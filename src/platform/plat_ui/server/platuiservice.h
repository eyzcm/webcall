#ifndef PlatUiService_H
#define PlatUiService_H

#include <QObject>
#include <QList>
#include "interface/View_interface.h"
#include "interface/uiyl_inputtextpanelinterface.h"
#include "interface/uiyl_chattreeinterface.h"
#include "interface/uiyl_centercontentinterface.h"
#include "interface/uiyl_chattoolbarinterface.h"
#include "interface/uiyl_attachmenttabinterface.h"
#include "interface/uiyl_pluginconstructioninterface.h"
#include "interface/uiyl_treeviewinterface.h"


#include "plat_ui_global.h"

class PLAT_UI_EXPORT PlatUiService:public QObject
{
    Q_OBJECT
public:
public:
    ~PlatUiService(){};
    static PlatUiService *GetInstance();

private:
    PlatUiService();
    class Garbo
    {
    public:
        ~Garbo();
    };
    static PlatUiService *m_instance;
public:

    void AddService( QString servername ,   void  * service );

    void AddAttachmentTab( UIYL_AttachmentTabInterface * view );

    View_interface * GetViewService();

    UIYL_InputTextPanelInterface * GetInputUi();
    UIYL_ChatTreeInterface * GetChatTree();
    UIYL_CenterContentInterface *  GetCenterContent();
    UIYL_ChatToolBarInterface *  GetChatToolBar();

    QList<UIYL_AttachmentTabInterface *> GetAttachmentTabList();

    void AddTreeViewTab(UIYL_TreeViewInterface * view);
    QList<UIYL_TreeViewInterface *> GetTreeViewList();

    void DoLoadUi(int type);
    UIYL_PluginConstructionInterface *  GetPluginConstruction();



signals:
    void S_LoadUi(int type);
private:
    View_interface * m_interface;
    UIYL_InputTextPanelInterface * m_inputPanel;
    UIYL_ChatTreeInterface * m_leftTreePanel;
    UIYL_CenterContentInterface * m_centerContent;

    UIYL_ChatToolBarInterface * m_chatToolBar;

    QList<UIYL_AttachmentTabInterface *> m_attachmentTabs; //右侧tab可扩展列表

    QList<UIYL_TreeViewInterface *>  m_leftTreeTabs;


    UIYL_PluginConstructionInterface * m_constructionPluginSerice;

};

#define gViewService PlatUiService::GetInstance()->GetViewService()


#endif // PlatUiService_H
