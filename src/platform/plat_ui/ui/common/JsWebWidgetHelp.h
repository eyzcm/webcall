#ifndef JsWebWidgetHelp_H
#define JsWebWidgetHelp_H

#include "ui/common/BaseJsCommHandle.h"
#include "plat_ui_global.h"

class PLAT_UI_EXPORT JsWebWidgetHelp : public BaseJsCommHandle
{
    Q_OBJECT

public:
    JsWebWidgetHelp();
    ~JsWebWidgetHelp();

public:
    void SetInitData();

    void ResultData( const QString seqNo,QString body  );
    virtual void                LoadFinished();

	void SetFirstUrl(  QString url );


private:
    QString GetColleageData( QString text );
    QString SendMsg(QString msg);

signals:
	void S_RefreshWeb();

public slots:
    void invoke(int seqNo,QString servername,QString reqMsg);
    QString GetColleageList(QString filterText);
    QString GetAccountInfo();
    void  InjectScript();


	void R_SysOpenWeb();
	void R_WebRefresh();
	void R_OpenDevTool();
    void R_StartTcpEvent();


signals:
    void s_resultReady( int callNo, QString  retobj  );






private:
    QString m_visitorStaticId;
    QString m_vid;
    QString m_token;
    QString m_userId;
    QString m_companyId;
    QString m_version;
    QString m_ip;
	QString						m_url;

    int   m_tcpEventLoad;

};

#endif // JsWebWidgetHelp_H
