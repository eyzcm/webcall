#include "platuiservice.h"



PlatUiService::PlatUiService()
{
    m_interface = nullptr;
    m_inputPanel = nullptr;
    m_chatToolBar = nullptr;
    m_centerContent = nullptr;
    m_constructionPluginSerice = nullptr;
}


void PlatUiService::AddService( QString servername ,  void  * service )
{
    if( servername == "View_interface" )
        m_interface = ( View_interface *)service;

    if( servername == "UIYL_InputTextPanelInterface")
        m_inputPanel = ( UIYL_InputTextPanelInterface *)service;
    if( servername == "UIYL_ChatTreeInterface")
        m_leftTreePanel = ( UIYL_ChatTreeInterface *)service;
    if( servername == "UIYL_CenterContentInterface")
        m_centerContent = ( UIYL_CenterContentInterface *)service;
    if( servername == "UIYL_ChatToolBarInterface")
        m_chatToolBar = ( UIYL_ChatToolBarInterface *)service;
    if( servername == "UIYL_PluginConstructionInterface")
        m_constructionPluginSerice = ( UIYL_PluginConstructionInterface *)service;





}

View_interface * PlatUiService::GetViewService()
{
    if( m_interface == nullptr )
    {
         ;//m_interface = new IProtocolInterface();
    }
    return m_interface;
}


PlatUiService *PlatUiService::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new PlatUiService;
    }
    return m_instance;
}

UIYL_InputTextPanelInterface * PlatUiService::GetInputUi()
{
    return m_inputPanel;
}

PlatUiService *PlatUiService::m_instance = nullptr;

PlatUiService::Garbo::~Garbo()
{
    if (PlatUiService::m_instance)
    {
        delete PlatUiService::m_instance;
        PlatUiService::m_instance = NULL;
    }
}

UIYL_ChatTreeInterface * PlatUiService::GetChatTree()
{
    return m_leftTreePanel;
}

void PlatUiService::DoLoadUi(int type)
{
    Q_EMIT S_LoadUi(type);
}

UIYL_CenterContentInterface *  PlatUiService::GetCenterContent()
{
    return m_centerContent;
}

UIYL_ChatToolBarInterface *  PlatUiService::GetChatToolBar()
{
    return m_chatToolBar;
}

void PlatUiService::AddAttachmentTab( UIYL_AttachmentTabInterface * view )
{
    m_attachmentTabs.append(view);
}

QList<UIYL_AttachmentTabInterface *> PlatUiService::GetAttachmentTabList()
{
    return m_attachmentTabs;
}

UIYL_PluginConstructionInterface *  PlatUiService::GetPluginConstruction()
{
    return m_constructionPluginSerice;
}

void PlatUiService::AddTreeViewTab(UIYL_TreeViewInterface * view)
{
    m_leftTreeTabs.append(view);
}
QList<UIYL_TreeViewInterface *> PlatUiService::GetTreeViewList()
{
       return m_leftTreeTabs;
}
