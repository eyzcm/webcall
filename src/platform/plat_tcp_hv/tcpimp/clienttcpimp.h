#ifndef CLIENTTCPIMP_H
#define CLIENTTCPIMP_H

#include <qobject.h>

#include "hloop.h"
#include "hssl.h"
#include "hmutex.h"

#include "hbase.h"
#include "herr.h"

#define TEST_SSL        0
#define TEST_UNPACK     0
#define TEST_RECONNECT  1

class ClientTcpImp:public QObject
{
	Q_OBJECT
public:

	ClientTcpImp(QObject * parent = nullptr );


	static bool   m_forceClose;

	int startConnect() {};
	static void reconnect_timer_cb(htimer_t* timer);
	static void on_close(hio_t* io);
	static void on_message(hio_t* io, void* buf, int len);
	static  void on_connect(hio_t* io);

	void tcp_client_new(hloop_t* loop ,const char * host, int port);
    void          tcp_client_run ();
    void          tcp_client_stop();
    void          tcp_client_free();

    // SSL/TLS
    int tcp_client_set_ssl_ctx( hssl_ctx_t ssl_ctx);
    int tcp_client_new_ssl_ctx( hssl_ctx_opt_t* opt);

    // reconnect
    int tcp_client_set_reconnect( reconn_setting_t* reconn);
    static int tcp_client_reconnect(ClientTcpImp* cli);

    void tcp_client_set_connnect_timeout( int timeout_ms);
    int  tcp_client_connect(const char* host, int port, int ssl);
    int  tcp_client_disconnect();
    bool tcp_client_is_connected();

    int  tcp_client_send( const void* buf, int len);

	void OnConnected();
	void OnDisconncted();
	void OnMessage(void * buf, int len);
	void OnNetWorkError(int code, QString errormsg);
	

signals:
	void S_connected();
	void S_disconnected();
	void S_message(void * buf, int len);
	void S_NetWorkError(int code, QString errormsg);

private:
	

	char host[256];
	int  port;
	int  connect_timeout; // ms
	// reconnect
	reconn_setting_t* reconn_setting;
	// flags
	unsigned char   ssl : 1; // Read Only
	unsigned char   alloced_ssl_ctx : 1; // intern
	unsigned char   connected : 1;
	// privdata
	hloop_t*    loop;
	hio_t*      io;
	htimer_t*   reconn_timer;
	// SSL/TLS
	hssl_ctx_t  ssl_ctx;
	// thread-safe
	hmutex_t    mutex_;

	
};

#endif // CLIENTTCPIMP_H
