#include "stdafx.h"
#include "UI_WebWidget.h"
#include "utils/DefineHelper.h"
#include "server/platuiservice.h"

#include <interface/uiwebviewinterface.h>

UI_WebWidget::UI_WebWidget(QWidget* parent) : QWidget(parent)
{
    m_menu = nullptr;
    ui.setupUi(this);
	m_newJsObj = nullptr;
    m_webView = new UiWebViewInterface(this);
    ui.horizontalLayout->addWidget(m_webView);
	UI_WebWidget * webui = this;
	QTimer::singleShot(500, this, [=]() {
		webui->Init();
	});
}


UI_WebWidget::~UI_WebWidget()
{

    if (m_newJsObj)
    {
        m_newJsObj->deleteLater();
        m_newJsObj = nullptr;
    }
}

void UI_WebWidget::Init()
{
    
	connect(m_webView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(R_CustomContextMenuRequested(const QPoint &)));
	connect(m_webView, SIGNAL(S_RefreshWeb()), this, SLOT(R_RefreshWeb()));
	CreateMenu();

}

void UI_WebWidget::RemoveUpdate(QString type, QString body)
{
}

void UI_WebWidget::Update(QString type, QObject* obj)
{

}

void UI_WebWidget::resizeEvent(QResizeEvent* e)
{
    if (m_newJsObj)
    {
        m_newJsObj->SizeChanged(e->size().width(), e->size().height());
    }
}

void UI_WebWidget::InitData()
{
    m_newJsObj = new JsWebWidgetHelp;
    m_newJsObj->SetWebView(m_webView, "jsObj");
}


void UI_WebWidget::SetUrl(QString url)
{
	m_webView->setUrl(QUrl(url));
	m_newJsObj->SetFirstUrl(url);
	
}

void UI_WebWidget::CreateMenu()
{
	
    m_menu = new QMenu(this);

	QMap<QString, QString>  map = { {"openview","默认浏览器打开"},
	{"refresh","刷新"},
	{"opentool","打开调试工具"}
	};
	
	for (QMap<QString, QString>::Iterator iter = map.begin(); iter != map.end(); iter++)
	{
		QString keystr = iter.key();
		QAction * openWebAction = new QAction(iter.value(), this);
		connect(openWebAction, &QAction::triggered, [=]() {
			if (keystr  == "openview")
			{
				m_newJsObj->R_SysOpenWeb();
			}
			if (keystr == "refresh")
			{
				R_RefreshWeb();
			}
			if (keystr == "opentool")
			{
				m_newJsObj->R_OpenDevTool();
				
			}
		});
		m_menu->addAction(openWebAction);

	}
     

}


void UI_WebWidget::R_CustomContextMenuRequested(const QPoint &pos)
{
    m_menu->exec(QCursor::pos());

}

void UI_WebWidget::R_RefreshWeb()
{
	m_webView->setUrl(m_webView->url());
}
